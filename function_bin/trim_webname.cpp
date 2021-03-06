#include <getopt.h>
#include "function/UserFunctionPack.hpp"

char* name;
std::string code;

int help(){
	using namespace std;
	cerr <<"사용법 : "<<name<<" [옵션] [값]"<<endl
	<<"웹에서 다운받은 파일의 이름을 손질함. url coding 풀기, 사용불가능한 문자를 대체"<<endl
	<<"문자로 치환, 웹에서의 공백치환을 감지하고 경우에 따라 해제, 파일 이름을 다듬기."<<endl
	<<"UHC로 변환가능한 인코딩은 변환. 주의! 실제 파일이 아닌 입력받은 내용을 대상으로"<<endl
	<<"만 작업합니다. 하나의 대상만을 작업합니다. 인코딩 변환이 잘못될수도 있습니다!"<<endl
	<<endl
	<<"  -i, --noiconv            자동으로 인코딩 값을 찾아 UTF8로 처리합니다. 만약 이"<<endl
	<<"                           옵션이 켜진다면 모든 값을 UTF8로 처리합니다."<<endl
	<<"  -e, --extexist           값에 포함된 확장자를 고려하여 작업합니다."<<endl
	<<"  --                       옵션의 끝을 나타냅니다. 이 뒤의 인자는 옵션으로 해석"<<endl
	<<"                           하지 않습니다."<<endl
	<<"  -h, --help               이 도움말을 표시하고 끝냅니다."<<endl
	<<"  -v, --version            버전 정보를 출력하고 끝냅니다."<<endl
	<<endl
	<<"bash로 작성된 trim_webname 함수의 C++버전입니다. c++0x 규격으로 작성되었습니다."<<endl
	;
	exit(0);
}

int version(){
	using namespace std;
	cerr <<"<<업데이트 내역>>"<<endl
	<<endl
	<<"1.1 : iconv 비활성화 기능을 추가"<<endl
	<<"1.0 : bash 스크립트로 구현된 trim_webname 함수의 기능을 C++을 이용하여 독립된 프로그램으로 구현"<<endl
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
	ufp::option opt;

	if( argc > 1 ) {
		//옵션처리
		int param_opt, option_index = 0, type16 = 0, type8 = 0, typeall = 0;
		static struct option long_options[] =
		{
			{"extexist",	no_argument,	0,	'e'},
			{"noiconv",		no_argument,	0,	'i'},
			{"help",		no_argument,	0,	'h'},
			{"version",		no_argument,	0,	'v'},
			{0,				0,				0,	0}
		};
		while( -1 != ( param_opt = getopt_long(argc, argv, "iehv", long_options, &option_index) ) ) {
			switch(param_opt) {
				case 'i':
					opt |= "noiconv";
					break;
				case 'e':
					opt |= "extexist";
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

	if ( code.empty() ) {
		std::cout <<"Unknown"<<std::endl;
		return 0;
	}

	std::cout <<ufp::trim_webname(code, opt)<<std::endl;
}