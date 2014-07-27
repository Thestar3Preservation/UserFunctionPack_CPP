#include "function/UserFunctionPack.hpp"

int main(int argc, char *argv[]) {
	int RealArgumentCount = argc - 1; //인자에서 실제로 콰우팅할 문자열 수
	std::vector<std::string> StringList; //콰우팅할 문자열 목록
	if ( RealArgumentCount == 0 ) { //콰우팅할 인자가 없으면
		std::string Temp;
		if ( !ufp::readstdin(Temp) ) { //표준입력으로 부터 값을 읽어옴.
			std::cerr <<"콰우팅할 값을 입력하십시오!"<<std::endl; //표준입력이 존재하지 않을 경우
			return 1; //종료 코드를 1로 설정하고 중단
		} else {
			StringList.push_back(Temp); //값이 존재하면 콰우팅 리스트에 추가
		}
	} else { //콰우팅할 인자가 존재하면
		for (int i = 1; i < RealArgumentCount + 1; i++) { //인자에서 콰우팅할 대상들을 대상으로
			StringList.push_back(argv[i]); //콰우팅 리스트에 인자를 추가
		}
	}
	for (std::string i : StringList) { //콰우팅 리스트에서 각각의 콰우팅 대상 마다
		std::cout <<ufp::s_quote(i)<<std::endl; //shell-always 콰우팅을 수행한뒤 표준출력으로 내보냄.
	}
	return 0; //종료 코드를 0으로 설정하고 중단.
}
