#pragma once

#include <iostream>
#include <limits>

namespace ufp {
	
	inline void pause();
	
	//일시정지 명령. 사용자의 입력을 대기합니다.
	inline void pause(){
		std::cout <<"[Enter]를 눌러 다음으로 진행합니다...";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout <<std::endl;
	}
	
}