#include "FileTable.hpp"

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/statvfs.h>
#include <dirent.h>
#include <pcre++.h> //-lpcre++
#include <pcrecpp.h> //-lpcrecpp
#include <vector>
#include <fstream>
#include "FileIo.hpp"
#include "FileTable.hpp"
#include "UrlCoding.hpp"
#include "PathName.hpp"
#include "StringManipulation.hpp"
#include "unit/to_string.hpp"
#include "TextEncoding.hpp"
#include <unistd.h>
#include "unit/Date.hpp"
#include <functional>
#include <boost/filesystem.hpp> //-lboost_system -lboost_filesystem

namespace ufp {
	
	
	//중복회피를 하며, 폴더를 생성합니다. 리턴값은 성공여부를 나타냅니다.
	bool dupev_mkdir(const std::string &code, std::string &result){
		std::string path = dirname(code);
		std::string newname = dupev_name(basename(code), path, true);
		if ( ! mkdir(path + "/" + newname) )
			return false;
		result = newname;
		return true;
	}

	//파일 이동시, 중복을 방지합니다. dupevLCode 함수를 참조합니다.
	bool dupev_mv(const std::string &src1, const string &src2) {
		return dupevLCodef(mv, src1, src2);
	}

	//파일 복사시, 중복을 방지합니다. dupevLCode 함수를 참조합니다.
	bool dupev_cp(const std::string &src1, const std::string &src2) {
		return dupevLCodef(cp, src1, src2);
	}
	
	//대상을 삭제합니다.
	bool rm(const std::string &target){
		try {
			boost::filesystem::remove_all(target);
		} catch (boost::system::error_code ec) {
			return false;
		}
		return true;
	}
	
	//마운트된 위치를 적절히 파악하여, 해당 위치의 휴지통으로 파일을 이동시킵니다.
	bool trashput(std::string path) {
		//주어진 경로를 실제 파일의 경로로 변환한다.
		path = realpath(dirname(path)) + "/" + basename(path);

		//마운트 지점 정보를 가져온다.
		std::vector<std::string> prelist = pcrepp::Pcre("\n", PCRE_UTF8).split(freadall("/etc/mtab"));
		int mpcmax = prelist.size();
		for ( int i = 0; i < mpcmax; i++ )
			pcrecpp::RE("^[^ ]+ ([^ ]+)", PCRE_UTF8).PartialMatch(prelist[i], &prelist[i]);

		//prelist를 unescape한다.
		int jmax = prelist.size();
		for ( int j = 0; j <jmax; j++ ) {
	// 		std::ostringstream temp;
			std::string &s = prelist[j], temp;
			int max = s.size();
			for ( int i = 0; i < max; i++ ) {
				int leng = getutf8byte(s[i]);
				if ( leng == 0 && s[i] == '\\' ) {
					temp += (char)strtol(new char[4]{s[i+1], s[i+2], s[i+3], '\0'}, NULL, 8);
					i += 3;
				} else for ( int k = 0; k <= leng; k++ )
					temp += s[i+k];
				i += leng;
			}
			s = temp;
		}

		//prelist를 길이를 기준으로 오름차순으로 정렬한다.
		auto comp = [] (const string &a, const string &b) -> bool {
			return a.size() > b.size();
		};
		std::sort(prelist.begin(), prelist.end(), comp);

		//prelist에서 마운트 포인트와 부분일치하는 첫번째를 요소를 선택한다
		std::string mp;
		bool mpfind = false;
		for ( std::string i : prelist )
			if ( pcrecpp::RE("^" + g_quote(i) + "/", PCRE_UTF8 | PCRE_MULTILINE).PartialMatch(path) ) {
				mp = i;
				mpfind = true;
				break;
			}

		//결과에 따라 Trash폴더를 선택한다.
		std::string trashPath, trashPathHome = std::string(getenv("HOME")) + "/.local/share/Trash";
		if ( mpfind )
			trashPath = mp + "/.Trash-" + to_string(getuid());
		else
			trashPath = trashPathHome;

		//휴지통 이동 함수
		std::function<bool()> TrashPut = [&] () -> bool {
			//폴더가 존재하는지 확인하고, 없다면 폴더와 파일을 생성한다.
			if ( ! mkdir(trashPath + "/info") || ! mkdir(trashPath + "/files") )
				return false;

			//Trash 폴더에서 파일이 위치하는 곳의 위치를 등록한다.
			std::string trashPathFiles = trashPath + "/files";

			//저장될 위치의 중복 방지를 위한 함수를 적용한다.
			std::string newname = dupev_name(basename(path), trashPathFiles);

			// 루트에 생성되지 않은 파일의 경우 url으로 기록할 경로를 상대경로로 변환시킨다.
			std::string rpath = path;
			if ( mpfind )
				pcrecpp::RE("^" + g_quote(mp) + "/", PCRE_UTF8).Replace("", &rpath);

			//trashinfo를 기록한다.
			std::ofstream fout(trashPath + "/info/" + newname + ".trashinfo");
			fout <<"[Trash Info]"<<std::endl<<"Path="<<url_encoding(rpath, "path")<<std::endl<<"DeletionDate="<<ufp::date("%Y-%m-%dT%H:%M:%S");
			fout.close();

			//파일을 Trash 폴더로 이동한다.
			if ( mv(path, trashPathFiles + "/" + newname) )
				return true;

			//이동이 불가능할 경우 홈 디렉토리의 폴더로 이동하고, 그래도 실패한다면 오류를 반환한다.
			if ( ! mpfind )
				return false;
			trashPath = trashPathHome;
			mpfind = false;
			return TrashPut();
		};

		//작업을 수행하고 성공 여부를 반환한다.
		return TrashPut();
	}
	
	//디렉토리를 생성합니다. 쉘 명령 mkdir -p와 같습니다.
	bool mkdir(const std::string &path) {
		try {
			boost::filesystem::create_directories(path);
		} catch (boost::filesystem::filesystem_error er) {
			return false;
		}
		return true;
	}
	
	bool cp(const std::string &from, const std::string &to) {
		try {
			boost::filesystem3::copy(from, to);
		} catch (boost::filesystem3::filesystem_error er) {
			return false;
		}
		return true;
	}

	//파일을 이동합니다.
	bool mv(const std::string &from, std::string to) {
		//원본이 존재하지 않는다면 종료
		if ( ! boost::filesystem::exists(from) )
			return false;

		//목적지의 디렉토리가 존재하지 않는다면 종료
		std::string to_dirname = dirname(to);
		if ( ! boost::filesystem::exists(to_dirname) )
			return false;

		//파일 시스템이 같은지 확인하기 위한 초기화 작업 수행
		struct statvfs from_fs, to_fs;
		statvfs(from.c_str(), &from_fs);
		bool to_exists = boost::filesystem::exists(to);
		if ( to_exists )
			statvfs(to.c_str(), &to_fs);
		else
			statvfs(to_dirname.c_str(), &to_fs);

		//목적지가 존재할 경우
		if ( to_exists ) {
			to = realpath(to);
			if ( ! boost::filesystem::is_directory(to) )
				return false;
			to = to + "/" + basename(from);
		}

		//파일 시스템이 같은가?
		if ( from_fs.f_fsid == to_fs.f_fsid ) {
			//같은 파일 시스템이라면
			rename(from.c_str(), to.c_str());
		} else {
			//다른 파일 시스템이라면
			if ( boost::filesystem::is_directory(from) ) {
				//재귀적 복사
				cp(from, to);
				boost::filesystem::remove_all(from);
			} else {
				//그냥 복사
				cp(from, to);
				boost::filesystem::remove(from);
			}
		}

		//작업을 종료
		return true;
	}

	//대상 폴더의 목록을 읽어옵니다. 숨김 파일을 포함하여 존재하는 모든 목록을 가져옵니다.
	bool ls(const std::string &path, std::vector<std::string> &dirls) {
		DIR *dir_info = opendir(path.c_str());
		struct dirent *dir_entry;
		if (NULL == dir_info)
			return false;
		while(dir_entry = readdir(dir_info))
			dirls.push_back(dir_entry->d_name);
		closedir(dir_info);
		return true;
	}

	std::string pwd(){
		char *temp1 = get_current_dir_name();
		std::string temp2(temp1);
		delete []temp1;
		return temp2;
	}
	
	//현재 위치를 변경합니다.
	bool cd(const std::string &path){
		return chdir(path.c_str()) == 0;
	}
	
}