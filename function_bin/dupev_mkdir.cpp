#include "function/UserFunctionPack.hpp"

int main(int argc, char *argv[]){
	std::string code;

	if( argc > 1 )
		code = argv[1];
	else if ( ! ufp::readstdin(code) ) {
		std::cerr <<"인자가 필요합니다!"<<std::endl;
		return 1;
	}

	if ( code.empty() ) {
		std::cerr <<"인자가 비어있습니다!"<<std::endl;
		return 1;
	}

	std::string result;
	if ( ufp::dupev_mkdir(code, result) ) {
		std::cout <<result<<std::endl;
		return 0;
	} else {
		std::cerr <<"폴더 생성에 실패하였습니다!"<<std::endl;
		return 1;
	}
}