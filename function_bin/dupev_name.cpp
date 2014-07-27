#include <getopt.h>
#include "UserFunctionPack.hpp"
//duplication_evasion : 중복 파일명을 회피하는 파일명을 출력해 줍니다.
//bash로 작성된 dupev_name 함수의 C++버전입니다.
//-std=c++0x 규격으로 작성되었습니다.

// 컴파일 명령
// g++ -std=c++0x dupev_name.cpp -o dupev_name -lpcrecpp

char *name;
std::string path, code;
bool usepath = false;

int help(){
	using namespace std;
	cerr <<"사용법 : "<<name<<" [옵션]... [비교명] [비교대상]..."<<endl
		 <<"비교명을 비교대상 목록과 비교분석하여 중복되지 않는 이름을 생성(대소문자를"<<endl
		 <<" 구분하지 않음)"<<endl
		 <<endl
		 <<"긴 옵션에서 꼭 필요한 인수는 짧은 옵션에도 꼭 필요합니다."<<endl
		 <<"  -p, --path=[경로]        비교 대상 목록을 주어진 경로의 목록으로 작성합니다."<<endl
		 <<"  -n, --nosplite           주어진 비교명을 한 덩어리로 취급합니다."<<endl
		 <<"  -i, --stdin              표준입력에서 비교명을 얻습니다."<<endl
		 <<"  --                       옵션의 끝을 나타냅니다. 이 뒤의 인자는 옵션으로 해석"<<endl
		 <<"                           하지 않습니다."<<endl
		 <<"  -h, --help               이 도움말을 표시하고 끝냅니다."<<endl
		 <<"  -v, --version            버전 정보를 출력하고 끝냅니다."<<endl
		 <<endl
		 <<"bash로 작성된 dupev_name 함수의 C++버전입니다. c++0x 규격으로 작성되었습니다."<<endl
		 ;
	exit(0);
}

int version(){
	using namespace std;
	cerr <<"<<업데이트 내역>>"<<endl
		 <<endl
		 <<"1.4 : 프로그램의 구조적으로 다시 설계함."<<endl
		 <<"      표준입력을 사용하는 경우, 옵션으로 표시하도록 함."<<endl
		 <<"1.3 : 인자의 유무와 표준입력의 존재 유무에 따른 처리를 분리시킴"<<endl
		 <<"1.2 : 일부 코드를 function 사용자 함수로 사용함"<<endl
		 <<"1.1 : 표준입력으로 들어오는 값에 줄바꿈 문자가 포함되었을때, 정상적으로 값을 가져오지 못하던 문제를 해결"<<endl
		 <<"1.0 : bash 스크립트로 구현된 dupev_name 함수의 기능을 C++을 이용하여 독립된 프로그램으로 구현"<<endl
		 <<"0.9 : 콰우팅 옵션을 제거"<<endl
		 <<"      regex 라이브러리 대신 pcrecpp라이브러리를 사용하도록 수정"<<endl
		 <<"0.8 : 경로로 부터 비교 대상 목록을 가져오도록 했을때, 오류를 발생시키던 문제 해결"<<endl
		 <<"0.7 : .과 ..를 고려하여 비교 목록을 사용하도록 수정"<<endl
		 <<"0.6 : vector 라이브러리를 사용하여 비교 목록을 작성하도록 변경"<<endl
		 <<"0.5 : 중복되는 대상이 존재하는지 확인하고 결과를 출력하는 기능을 구현"<<endl
		 <<"0.4 : 비교 경로가 존재하고 있다면, 해당 경로의 목록을 작성하도록 함"<<endl
		 <<"0.3 : 비교 대상 목록을 지정하는 기능을 구현"<<endl
		 <<"0.2 : 비교 대상 이름을 지정하고 검사하는 기능을 구현"<<endl
		 <<"0.1 : 긴 옵션 이름 처리 기능과 짧은 옵션 이름 처리 기능을 구현"<<endl
		 <<"      디버깅 메시지를 출력하는 함수를 추가"<<endl
		 <<"      도움말과 버전 페이지를 추가"<<endl
		 ;
	exit(0);
}

int main(int argc, char *argv[]){
	//변수 선언 및 초기화
	name = argv[0];
	std::vector<std::string> ls;
	bool nosplite = false;

	if ( argc > 1 ) {
		//옵션 처리
		int param_opt, option_index = 0;
		bool standardinput = false;
		static struct option long_options[] =
		{
			{"path",		required_argument,	0,	'p'},
			{"stdin",		no_argument,		0,	'i'},
			{"nosplite",	no_argument,		0,	'n'},
			{"help",		no_argument,		0,	'h'},
			{"version",		no_argument,		0,	'v'},
			{0,				0,					0,	0}
		};
		while( -1 != ( param_opt = getopt_long(argc, argv, "p:inhv", long_options, &option_index) ) ) {
			switch(param_opt) {
				case 'p':
					path = optarg;
					usepath = true;
					break;
				case 'i':
					standardinput = true;
					break;
				case 'n':
					nosplite = true;
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

		int realargc = argc -  optind;

		if ( ! standardinput && realargc == 0 )
			help();

		if ( standardinput ) {
			if ( ! ufp::readstdin(code) ) {
				std::cerr <<"표준입력이 존재하지 않습니다!"<<std::endl;
				return 1;
			}
		} else {
			code = argv[optind];
			optind++;
			realargc--;
		}

		//비교경로가 포함되어 있다면
		if ( ! usepath && code.find("/") != std::string::npos ) {
			usepath = true;
			path = ufp::dirname(code);
			code = ufp::basename(code);
			//비교경로가 포함되어 있지 않다면
		}

		if ( ! usepath )
			if ( realargc > 0 )
				for(int i = optind; i < argc; i++)
					ls.push_back(argv[i]);
			else {
				std::cerr <<"비교 대상이 존재하지 않습니다!"<<std::endl;
				return 1;
			}
	} else
		help();

	if ( code.empty() ) {
		std::cerr <<"비교명이 존재하지 않습니다!"<<std::endl;
		return 1;
	}

	if ( usepath && path.empty() ) {
		std::cerr <<"비교 경로가 존재하지 않습니다!"<<std::endl;
		return 1;
	}

	std::cout <<ufp::dupev_name(code, path, nosplite, ls)<<std::endl;
}