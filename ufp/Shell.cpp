#include "Shell.hpp"

#include "StringManipulation.hpp"
#include "FileIo.hpp"
#include <stdexcept>
#include <pstreams/pstream.h>

namespace ufp {
	
	//시스템 명령어를 사용합니다. 종료 코드를 반환합니다.
	int system(const std::string &cmd) {
		return ::system(cmd.c_str());
	}

	//시스템 명령을 실행하고, 두번째 인자로 부터 결과를 받아들입니다.
	int system(const std::string &cmd, std::string &ret, std::string type) {
		redi::ipstream proc(cmd); // redi::pstreams::pstderr
		if ( type == "out" )
			ret = readall(proc.out());
		else if ( type == "err" )
			ret = readall(proc.err());
		else
			throw std::logic_error("출력 형식을 올바르게 지정하십시오!");
		proc.close();
		return proc.rdbuf()->status();
	}

	//quote는 bash쉘에서 해석되는 문자를 콰우팅을 해줍니다. 인자의 내용은 콰우팅되어 있지 않아야 합니다.
	std::string s_quote(const std::string &code){
		return "'" + replaceAll(code, "'", "'\\''") + "'";
	}
	
	//ValueName에 해당되는 변수의 내용을 ReturnValue에 저장합니다. 만약, 변수가 존재하지 않는다면 false를 리턴합니다.
	bool GetEnv(const std::string &ValueName, std::string &ReturnValue){
		char *Temp = getenv(ValueName.c_str());
		if ( Temp == NULL ) {
			return false;
		} else {
			ReturnValue = Temp;
			return true;
		}
	}
	
}