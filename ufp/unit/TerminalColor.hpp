#pragma once

#include <string>

namespace ufp {
	
	//색상 값을 선언
	namespace color {
		std::string reset = "\e[39;49;00m";			//: 색상값을 초기화
		
		//배경 색
		namespace bak {
			std::string red = "\e[41m";				//: 빨강색
			std::string green = "\e[42m";			//: 초록색
			std::string yellow = "\e[43m";			//: 노랑색
			std::string blue = "\e[44m";				//: 파랑색
			std::string magenta = "\e[45m";			//: 자홍색
			std::string cyan = "\e[46m";				//: 시안색
			std::string darkgray = "\e[100m";		//: 밝은 회색
			std::string lightgreen = "\e[102m";		//: 밝은 초록색
		}
		
		//글자 색
		std::string red = "\e[31;01m";				//: 빨강색
		std::string green = "\e[32;01m";				//: 초록색
		std::string yellow = "\e[33;01m";			//: 노랑색
		std::string blue = "\e[34;01m";				//: 파랑색
		std::string magenta = "\e[35;01m";			//: 자홍색, 마젠타색
		std::string cyan = "\e[36;01m";				//: 밝은 파랑색, 시안색
	};
	
}