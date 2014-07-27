#include <getopt.h>
#include "function/UserFunctionPack.hpp"
//bash로 작성된 dupev_name 함수의 C++버전입니다.
//-std=c++0x 규격으로 작성되었습니다.

char *name;

int help(){
	using namespace std;
	cerr <<"사용법 : "<<name<<" [옵션] [값1] [값2]"<<endl
		 <<"파일을 A에서 B위치로 중복되지 않게 주어진 명령을 수행(대소문자를 구분하지 않음)"<<endl
		 <<"인자 1은 원본을 의미하고, 인자 2는 대상을 의미합니다. cp를 command에 준다면 인"<<endl
		 <<"자 1에서 인자 2로 복사 작업을 수행하고, mv를 준다면 인자 1에서 인자 2로 이동 작"<<endl
		 <<"업을 수행합니다. 이것은 어떤 옵션도 포함되지 않은 순수한 이동 명령만을 대상으로"<<endl
		 <<" 합니다."<<endl
		 <<endl
		 <<"  -t, --no-target-directory  주어진 대상이름을 한 덩어리로 취급합니다."<<endl
		 <<"  --                         옵션의 끝을 나타냅니다. 이 뒤의 인자는 옵션으로 해"<<endl
		 <<"                             석하지 않습니다."<<endl
		 <<"  -h, --help                 이 도움말을 표시하고 끝냅니다."<<endl
		 <<"  -v, --version              버전 정보를 출력하고 끝냅니다."<<endl
		 <<endl
		 <<"bash로 작성된 dupevLCode 함수의 C++버전입니다. c++0x 규격으로 작성되었습니다."<<endl
		;
	exit(0);
}

int version(){
	using namespace std;
	cerr <<"<<업데이트 내역>>"<<endl
		 <<endl
		 <<"1.0 : bash 스크립트로 구현된 dupevLCode 함수의 기능을 C++을 이용하여 독립된 프로그램으로 구현"<<endl
		 <<"0.1 : 도움말과 버전 페이지를 추가"<<endl
		 <<"      디버깅 메시지를 출력하는 함수를 추가"<<endl
		;
	exit(0);
}

int main(int argc, char *argv[]){
	//변수 선언 및 초기화
	name = argv[0];
	string factor1, factor2;
	bool noTargetDirectory = false;

	if ( argc == 1 )
		help();

	if( argc > 2 ) {
		//옵션처리
		int param_opt, option_index = 0;
		static struct option long_options[] =
		{
			{"no-target-directory",	no_argument,		0,	't'},
			{"help",				no_argument,		0,	'h'},
			{"version",				no_argument,		0,	'v'},
			{0,						0,					0,	0}
		};
		while( -1 != ( param_opt = getopt_long(argc, argv, "thv", long_options, &option_index) ) ) {
			switch(param_opt) {
				case 't':
					noTargetDirectory = true;
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
		if( optind + 1 < argc ) {
			factor1 = argv[optind];
			factor2 = argv[optind+1];
			if ( factor1.empty() || factor2.empty() ) {
				std::cerr <<"인자가 비어있어선 안됩니다"<<std::endl;
				return 1;
			}
		} else {
			std::cerr <<"인자가 부족합니다!"<<std::endl;
			return 1;
		}
	} else {
		std::cerr <<"인자가 부족합니다!"<<std::endl;
		return 1;
	}

	//작업 명령이 전달되었는지 확인
	char *command = getenv("command");
	if ( command == NULL || string(command).empty() ) {
		std::cerr <<"명령어가 필요합니다!"<<std::endl;
		return 1;
	}

	return ufp::dupevLCode(command, factor1, factor2, noTargetDirectory);
}