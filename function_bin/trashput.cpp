#include "function/UserFunctionPack.hpp"

int main(int argc, char *argv[]) {
	std::vector<std::string> list;

	if ( argc == 1 ) {
		std::string temp;
		if ( ufp::readstdin(temp) )
			list.push_back(temp);
		else {
			std::cerr <<"대상이 지정되지 않았습니다!"<<std::endl;
			return 1;
		}
	} else for ( int i = 1; i < argc; i++ )
		list.push_back(argv[i]);

	int rec = 0;

	for ( std::string i : list )
		if ( ! ufp::trashput(i) ) {
			std::cerr <<"대상 \"" + i + "\" 삭제 실패!"<<std::endl;
			rec = 1;
		}

	return rec;
}