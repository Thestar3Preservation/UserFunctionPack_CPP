#include <getopt.h>
#include "function/UserFunctionPack.hpp"

char* name;
std::string code;

int help(){
	using namespace std;
	cerr <<"사용법 : "<<name<<" [옵션] [값]"<<endl
	<<"url를 Decoding함."<<endl
	<<endl
	<<"아무런 옵션이 지정되지 않았을 경우 UTF-8옵션으로 실행됩니다."<<endl
	<<"      --utf8               표준. %[a-z0-9]{2}식의 퍼센트 이스케이프된 내용을 디"<<endl
	<<"                           코딩함."<<endl
	<<"      --utf16              비표준. %u[a-z0-9]{4}식의 퍼센트 이스케이프된 내용을"<<endl
	<<"                           디코딩함."<<endl
	<<"  -a, --all                utf8과 utf16방식으로 디코딩함. 잘못 디코딩될수 있음."<<endl
	<<"                           혼합된 경우 수동으로 분석하는것을 추천."<<endl
	<<"  --                       옵션의 끝을 나타냅니다. 이 뒤의 인자는 옵션으로 해석"<<endl
	<<"                           하지 않습니다."<<endl
	<<"  -h, --help               이 도움말을 표시하고 끝냅니다."<<endl
	<<"  -v, --version            버전 정보를 출력하고 끝냅니다."<<endl
	<<endl
	<<"bash로 작성된 url_decoding 함수의 C++버전입니다. c++0x 규격으로 작성되었습니다."<<endl
	;
	exit(0);
}

int version(){
	using namespace std;
	cerr <<"<<업데이트 내역>>"<<endl
	<<endl
	<<"1.0 : bash 스크립트로 구현된 url_decoding 함수의 기능을 C++을 이용하여 독립된 프로그램으로 구현"<<endl
	;
	exit(0);
}

inline void stdincheck(){
	//표준입력이 존재하면, 그 값을 인자로 삼은뒤 작업시작. 없다면 도움말을 호출
	if ( ! ufp::readstdin(code) )
		help();
}

int main(int argc, char *argv[]){
	name = argv[0];
	ufp::option opt = "utf8";

	if( argc > 1 ) {
		//옵션처리
		int param_opt, option_index = 0, type16 = 0, type8 = 0, typeall = 0;
		static struct option long_options[] =
		{
			{"utf8",		no_argument,		&type8,		1},
			{"utf16",		no_argument,		&type16,	1},
			{"all",			no_argument,		0,			'a'},
			{"help",		no_argument,		0,			'h'},
			{"version",		no_argument,		0,			'v'},
			{0,				0,					0,			0}
		};
		while( -1 != ( param_opt = getopt_long(argc, argv, "ahv", long_options, &option_index) ) ) {
			switch(param_opt) {
				case 'a':
					typeall = 1;
					break;
				case 'h':
					help();
					break;
				case 'v':
					version();
					break;
				case '?':
					return 1;
					break;
			}
		}
		int optmulch = 0;
		if ( type8 == 1 ) {
			opt = "utf8";
			optmulch++;
		}
		if ( type16 == 1 ) {
			opt = "utf16";
			optmulch++;
		}
		if ( typeall == 1 ) {
			opt = "all";
			optmulch++;
		}
		if(optmulch > 1) {
			std::cerr <<"옵션을 하나만 지정하십시오!"<<std::endl;
			exit(1);
		}

		//처리후에도 인자가 존재하는가? 그렇다면 작업시작. 없다면 표준입력을 체크
		if(optind < argc)
			code = argv[optind];
		else
			stdincheck();
	} else
		stdincheck();

	if ( code.empty() )
		return 0;

	std::cout <<ufp::url_decoding(code, opt)<<std::endl;
}