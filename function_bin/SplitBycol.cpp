#include <getopt.h>
#include <sys/ioctl.h>
#include "function/UserFunctionPack/SplitBycol.hpp"

char *name;
std::string code;
int col = 80;

int help(){
	using namespace std;
	cerr <<"사용법 : "<<name<<" [옵션] [내용]"<<endl
	<<"문자열이 차지하는 지정된 길이 마다 newline을 삽입"<<endl
	<<endl
	<<"긴 옵션에서 꼭 필요한 인수는 짧은 옵션에도 꼭 필요합니다."<<endl
	<<"  -c, --maxcol=[열]        나눌 길이를 지정합니다. auto로 지정할 경우 자동으로 "<<endl
	<<"                           현재 터미널의 칸 수를 찾아냅니다. 실패할 경우 80칸으"<<endl
	<<"                           로 적용됩니다."<<endl
	<<"      --bash               bash쉘에서 적용되는 제어문자를 고려하여 열을 계산합"<<endl
	<<"                           니다."<<endl
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
	<<"1.2 : bash 쉘의 제어문자를 고려하여 열을 계산하는 옵션을 추가"<<endl
	<<"1.1 : 인자의 유무와 표준입력의 존재 유무에 따른 처리를 분리시킴"<<endl
	<<"1.0 : col을 찾아내는 기능을 추가"<<endl
	<<"      도움말과 버전 페이지를 추가"<<endl
	<<"0.9 : 문자 길이를 찾아내는 코드가 적용되지 않아, 임의의 길이를 지정해둠."<<endl
	;
	exit(0);
}

inline void getcols(){
	struct winsize w;
	ioctl(2, TIOCGWINSZ, &w);
	col = w.ws_col;
	if ( col <= 0 ) {
		std::cerr <<"COLUMS값을 찾지 못하여, 기본값 80을 적용합니다."<<std::endl;
		col = 80;
	}
}

void stdincheck(){
	//표준입력이 존재하면, 그 값을 인자로 삼은뒤 작업시작. 없다면 도움말을 호출
	if ( ! ufp::readstdin(code) )
		help();
}

int main(int argc, char *argv[]){
	//변수 선언 및 초기화
	name = argv[0];
	ufp::option opt;

	if( argc > 1 ) {
		//옵션처리
		int param_opt, option_index = 0, bash = 0;
		bool setcol = false;
		static struct option long_options[] =
		{
			{"maxcol",		required_argument,	0,		'c'},
			{"bash",		no_argument,		&bash, 	1},
			{"help",		no_argument,		0,		'h'},
			{"version",		no_argument,		0,		'v'},
			{0,				0,					0,		0}
		};
		while( -1 != ( param_opt = getopt_long(argc, argv, "c:hv", long_options, &option_index) ) ) {
			switch(param_opt) {
				case 'c':
					setcol = true;
					if ( std::string("auto").compare(optarg) == 0)
						getcols();
					else {
						col = atoi(optarg);
						if ( col <= 0 ) {
							std::cerr <<"잘못된 COLUMS값입니다!"<<std::endl;
							return 1;
						}
					}
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

		if ( ! setcol ) {
			std::cerr <<"columns값이 필요합니다!"<<std::endl;
			return 1;
		}

		if ( bash == 1 )
			opt = "SkipBashControlCharacters";

		//처리후에도 인자가 존재하는가? 그렇다면 작업시작. 없다면 표준입력을 체크
		if(optind < argc)
			code = argv[optind];
		else
			stdincheck();
	} else
		stdincheck();

	if ( code.empty() )
		return 0;

	std::cout <<ufp::SplitBycol(code, col, opt)<<std::endl;
}