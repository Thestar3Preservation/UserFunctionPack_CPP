#include "FileIo.hpp"

#include <pcrecpp.h> ///< 컴파일에 -lpcrecpp 플래그가 필요
#include "TextEncoding.hpp"

namespace ufp {
	
	/**표준입력으로 들어오는 모든 내용을 읽어옴. 
	 * @return 표준입력이 존재하지 않으면 false를 리턴.
	 * 들어오는 모든 값은 자동으로 마지막의 \\n이 제거됩니다.
	 */
	bool readstdin(
		std::string &str ///<내용을 받아올 변수
	) {
		if ( isatty(0) ) {
			return false;
		}
		std::getline(std::cin, str, CO_Eof);
		if ( str.empty() ) {
			return true;
		}
		int pos = str.length() - 1;
		if ( str.rfind("\n") == pos ) {
			str.erase(pos, 1);
		}
		return true;
	}

	/**입력 파일 스트림으로 부터 모든 내용을 받아 string으로 저장함.
	 * @return 파일 입력 스트림으로 부터 읽어온 내용
	 */
	std::string readall(
		std::ifstream &stream ///<파일 입력 스트림
	){
		std::string code;
		std::getline(stream, code, CO_Eof);
		return code;
	}
	/**입력 스트림으로 부터 모든 내용을 받아 string으로 저장함.
	 * @return 입력 스트림으로 부터 읽어온 내용
	 */
	std::string readall(
		std::istream &stream ///<입력 스트림
	){
		std::string code;
		std::getline(stream, code, CO_Eof);
		return code;
	}

	/**입력 스트림으로 부터 모든 내용을 받아 string으로 저장함. 
	 * 만약 UTF8 BOM이 포함되어있다면 자동으로 제거합니다.
	 * @return 읽어온 내용
	 */
	std::string freadall(
		const std::string &filepath ///<파일 경로
	) {
		std::ifstream fin(filepath);
		std:string temp = readall(fin);
		fin.close();
		pcrecpp::RE("^"+CO_Bom_Utf8, PCRE_UTF8).Replace("", &temp);
		return temp;
	}

}