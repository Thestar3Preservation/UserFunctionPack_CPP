#include "PathName.hpp"

#include <pcrecpp.h> //-lpcrecpp
#include <pcre++.h> //-lpcre++
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "TextEncoding.hpp"
#include <algorithm>
#include "StringManipulation.hpp"
#include "unit/to_string.hpp"
#include "UrlCoding.hpp"
#include "FileTable.hpp"
#include "Shell.hpp"
#include <boost/filesystem.hpp> //-lboost_system -lboost_filesystem

namespace ufp {
	
	//파일을 A에서 B위치로 중복되지 않게 어떠한 작업을 합니다. 인자 1은 원본을 의미하고, 인자 2는 대상을 의미합니다. 이것은 어떤 옵션도 포함되지 않은 순수한 이동 명령만을 대상으로 합니다. 이는 그것을 위한 인자2를 되돌려줍니다.
	bool dupevLCodef(std::function<bool(std::string, std::string)> fp, const std::string &src1, std::string src2, bool noTargetDirectory) {
		if ( boost::filesystem::exists(src2) ) {
			bool nosplite;
			struct stat st;
			if( lstat(src1.c_str(), &st) == 0 && S_ISDIR(st.st_mode) )
				nosplite = true;
			else
				nosplite = false;
			std::string target = realpath(src2);
			std::string targetname_1 = basename(src1);
			std::string targetname_2 = basename(src2);
			std::string targetdir = dirname(target);
			if ( dirname(realpath(src1)).compare(targetdir) == 0 && targetname_1.compare(targetname_2) == 0 )
				return true;
			if ( ! noTargetDirectory && lstat(target.c_str(), &st) == 0 && S_ISDIR(st.st_mode) )
				src2 = target + "/" + dupev_name(targetname_1, target, nosplite);
			else
				src2 = targetdir + "/" + dupev_name(targetname_2, targetdir, nosplite);
		}
		return fp(src1, src2);
	}
	bool dupevLCode(const std::string &command, const std::string &src1, const std::string &src2, bool noTargetDirectory) {
		auto cmd = [&](std::string src1, std::string src2)->bool{
			return system(command + " -- " + s_quote(src1) + " " + s_quote(src2));
		};
		return dupevLCodef(cmd, src1, src2, noTargetDirectory);
	}
	
	//윈도우와 리눅스에서 사용불가능한 문자를 대체
	std::string wl_replace(std::string code){
		std::string tr[] = {
			"?", "？",
			":", "：",
			"*", "＊",
			"\"", "＂",
			"<", "〈",
			">", "〉",
			"|", "│",
			"\\", "＼",
			"'", "＇",
			"/", "／",
			"$", "＄",
			"!", "！"
		};
		for(int i = 0; i < 24; i+=2)
			code = replaceAll(code, tr[i], tr[i+1]);
		return code;
	}
	
	//이름을 손질함. 앞뒤의 불필요한 공백문자를 제거. 줄바꿈, 탭등의 문자를 스페이스로 치환하는 등의 작업을 함.
	std::string trim_name(std::string code) {
		pcrecpp::RE_Options opt( PCRE_UTF8 );
		pcrecpp::RE("[:cntrl:]", opt).GlobalReplace(" ", &code);
		pcrecpp::RE("^-*(\\s| )*-*", opt).Replace("", &code);
		pcrecpp::RE("(\\s| )+$", opt).Replace("", &code);
		pcrecpp::RE(" ?(：|／) ?", opt).GlobalReplace("\\1", &code);
		return code.empty() ? "Unknown" : code;
	}

	//웹에서 다운받은 파일의 이름을 손질함. url coding 풀기, 사용불가능한 문자를 대체문자로 치환, 웹에서의 공백치환을 감지하고 경우에 따라 해제, 파일 이름을 다듬기. UHC로 변환가능한 인코딩은 변환. 주의! 실제 파일이 아닌 입력받은 내용을 대상으로만 작업합니다. 하나의 대상만을 작업합니다. 인코딩 변환이 잘못될수도 있습니다!
	std::string trim_webname(std::string code, option op) {
		std::string ext;
		
		//옵션 체크
		bool extexist = op.check("extexist");
		bool autoiconv = !op.check("noiconv");
		
		//확장자 분리
		if ( extexist ) {
			pcrecpp::RE("\\s+$", PCRE_UTF8).GlobalReplace("", &code);
			if ( ex_ext(code, ext) )
				code = code.substr(0, code.rfind("."));
			else
				extexist = false;
		}

		//퍼센트 인코딩을 디코딩
		code = url_decoding(code);

		//문자열을 인코딩을 검사하고 UTF-8이 아니라면 UHC타입에서 UTF-8로 변환하기
		if ( autoiconv ) {
			std::string charset;
			bool s = getEncoding(code, charset);
			if ( ! s || charset != "UTF-8" ) {
				code = iconv("UHC", "UTF-8").SetSrc(code).GetStr();
			}
		}

		//이름에 포함된 and표시 제거
		if ( code.find(" ") == string::npos ){
			if ( code.find("_") != string::npos || code.find("+") != string::npos ){
				code = replaceAll(replaceAll(code, "+", " "), "_", " ");
			}else if ( code.find(".") != string::npos ){
				code = replaceAll(code, ".", " ");
			}
		}

		//파일명에 사용불가능한 특수문자를 대체문자로 대체후, 제어문자를 제거, 이름 앞뒤의 공백문자 제거, 일부 문자의 공백을 축소( : 를 대체문자로 바꾸면, ：가 되어 한문자가 차지하는 칸이 늘어나게됨, 이런 칸늘어남을 조절해줌)함.
		if ( extexist ) {
			return trim_name( wl_replace(code) ) + "." + ext;
		}
		else{
			return trim_name( wl_replace(code) );
		}
	}
	
	//중복이름회피 함수. code는 비교할 이름, path나 list[]는 비교 위치 또는 비교 목록, nosplite는 true상태일 경우 확장자를 구분하지 않고 결과를 출력.
	std::string dupev_name(std::string code, const std::string &path, bool nosplite, std::vector<std::string> targetlist){
		if ( path.empty() ) {
			if ( targetlist.size() == 0 ) {
				std::cerr <<"목록이 존재하지 않습니다!"<<std::endl;
				return "";
			}
		} else {
			//비교 경로가 존재하고 있다면, 해당 경로의 목록을 작성
			if ( ls(path, targetlist) ) {
				if(targetlist.size() == 2)
					return(code);
			} else {
				std::cerr <<"화일 목록을 가져올수 없습니다!"<<std::endl;
				return "";
			}
		}

		//중복되는 대상이 존재하는지 확인
		bool check = false;
		char *temp = strdup(code.c_str());
		for (std::string i : targetlist)
			if( ! strcasecmp(temp, i.c_str()) ){
				check = true;
				break;
			}
		delete[] temp;

		//중복되는 대상이 존재하고 있지 않다면 이름을 그대로 돌려줌.
		if ( ! check )
			return code;

		//중복되는 대상이 존재하고 있다면, 중복을 회피하는 이름을 생성
		std::string name, ext;
		pcrecpp::RE_Options opt( PCRE_CASELESS | PCRE_UTF8 );
		if ( nosplite )
			name = code;
		else if ( ex_ext(code, ext, true) )
			name = ex_name(code);
		else {
			name = basename(code);
			nosplite = true;
		}
		std::string str = g_quote(name);
		pcrecpp::RE *rx;
		if (nosplite)
			rx = new pcrecpp::RE("^" + str + " \\(d([0-9]+)\\)$", opt);
		else
			rx = new pcrecpp::RE("^" + str + " \\(d([0-9]+)\\)\\." + ext + "$", opt);
		std::vector<int> countls;
		int tempi;
		for(std::string i : targetlist)
			if ( rx->FullMatch(i, &tempi) )
				countls.push_back(tempi);
		delete rx;
		int count;
		if( countls.size() != 0 ) {
			std::sort(countls.begin(), countls.end(), std::greater<int>());
			count = countls[0] + 1;
		} else
			count = 1;
		code = name + " (d" + to_string(count);
		code += nosplite ? ")" : ")." + ext;
		return code;
	}
	
	//상대경로를 절대경로로 변환합니다.
	std::string fullpath(const std::string &path){
		return ::realpath(NULL, (char*)path.c_str());
	}

	//주어진 경로의 실질적 위치를 반환합니다. 즉, 링크된 위치따라가서 실제 절대경로를 반환합니다.
	std::string realpath(const std::string &path) {
		return ::realpath(path.c_str(), NULL);
	}

	//basename이란 이름의 프로그램과 동일한 기능을 수행
	std::string basename(std::string code){
		int find;
		if( ( find = code.rfind("/") ) == string::npos )
			return code;
		else
			return code.substr(find + 1);
	}

	//dirname이란 이름의 프로그램과 동일한 기능을 수행
	std::string dirname(std::string code){
		int find;
		if( ( find = code.rfind("/") ) == string::npos )
			return ".";
		else
			return code.substr(0, find);
	}

	//확장자를 추출해냅니다. 경로화된 경우에도 확장자만 추출해줍니다. 추출할 내용이 존재하지 않는 경우에는 해당 내용에 대해 출력하지 않습니다. 성공한다면, sucess의 값은 true가 되고 아니면 false가 됩니다.
	bool ex_ext(std::string code, std::string &result, bool lower) {
		if( ! pcrecpp::RE(".+\\.([a-z0-9]+)$", PCRE_CASELESS | PCRE_UTF8).PartialMatch(code, &code) )
			return false;
		if ( lower ) {
			auto begin = code.begin();
			std::transform(begin, code.end(), begin, ::tolower);
		}
		result = code;
		return true;
	}

	//확장자를 제외한 파일명을 추출해냅니다. 경로화된 경우에도 파일명만 추출해줍니다. 추출할 내용이 존재하지 않는 경우에도 출력합니다.
	std::string ex_name(std::string code){
		int find;
		if( ( find = code.rfind("/") ) == string::npos ) {
			if( ( find = code.rfind(".") ) == string::npos || find == 0 ) {
				return code;
			} else {
				return code.substr(0, find);
			}
		} else {
			if( pcrecpp::RE("\\.[a-z0-9]+$", PCRE_CASELESS | PCRE_UTF8).PartialMatch(code) ) {
				return code.substr(find + 1, code.rfind(".") - find - 1);
			} else {
				return code.substr(find + 1);
			}
			pcrecpp::RE("([^/]+)(\\.[a-z0-9]+)?$", PCRE_CASELESS | PCRE_UTF8).PartialMatch(code, &code);
		}
		return code;
	}
	
}