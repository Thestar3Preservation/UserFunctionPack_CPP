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
		std::cout <<"Unknown"<<std::endl;
		return 0;
	}

	std::cout <<ufp::trim_name(code)<<std::endl;
}