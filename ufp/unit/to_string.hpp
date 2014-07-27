#pragma once

#include <string>
#include <sstream>

namespace ufp {
	
	template < typename T > inline std::string to_string(const T&);
	
	//다른 종류(int, char 등...)의 변수의 값을 string 타입으로 변환함.
	template < typename T > inline std::string to_string(const T& data) {
		std::ostringstream stm;
		stm <<data;
		return stm.str();
	}
	
	
}