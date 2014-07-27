#include <getopt.h>
#include "function/UserFunctionPack.hpp"

char *name;
std::string code;

int help(){
	using namespace std;
	cerr <<"사용법 : "<<name<<" [옵션] [내용]"<<endl
	<<"내용에서 확장자를 분리해냅니다. 분리할 확장자가 존재하지 않는 경우, 아무것도 출"<<endl
	<<"력하지 않습니다. 종료값 0은 성공, 종료값 1은 실패를 의미합니다."<<endl
	<<endl
	<<"  -d, --downscale          출력되는 확장자를 소문자로 표현합니다."<<endl
	<<"  --                       옵션의 끝을 나타냅니다. 이 뒤의 인자는 옵션으로 해석"<<endl
	<<"                           하지 않습니다."<<endl
	<<"  -h, --help               이 도움말을 표시하고 끝냅니다."<<endl
	<<"  -v, --version            버전 정보를 출력하고 끝냅니다."<<endl
	;
	exit(0);
}

int version(){
	using namespace std;
	cerr <<"<<업데이트 내역>>"<<endl
	<<endl
	<<"1.0 : 소문자화 기능 추가"<<endl
	<<"      도움말과 버전 페이지를 추가"<<endl
	;
	exit(0);
}

inline void stdincheck(){
	//표준입력이 존재하면, 그 값을 인자로 삼은뒤 작업시작. 없다면 도움말을 호출
	if ( ! ufp::readstdin(code) )
		help();
}

int main(int argc, char *argv[]){
	//변수 선언 및 초기화
	name = argv[0];
	bool downscale = false;

	if( argc > 1 ) {
		//옵션처리
		int param_opt, option_index = 0, bash = 0;
		static struct option long_options[] =
		{
			{"downscale",	no_argument,	0, 	'd'},
			{"help",		no_argument,	0,	'h'},
			{"version",		no_argument,	0,	'v'},
			{0,				0,				0,	0}
		};
		while( -1 != ( param_opt = getopt_long(argc, argv, "dhv", long_options, &option_index) ) ) {
			switch(param_opt) {
				case 'd':
					downscale = true;
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

		//처리후에도 인자가 존재하는가? 그렇다면 작업시작. 없다면 표준입력을 체크
		if(optind < argc)
			code = argv[optind];
		else
			stdincheck();
	} else
		stdincheck();

	if ( code.empty() )
		return 1;

	if ( ufp::ex_ext(code, code, downscale) ) {
		std::cout <<code<<std::endl;
		return 0;
	} else
		return 1;
}