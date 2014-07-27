#pragma once

#include <string>
#include <iostream>
#include <fstream>

namespace ufp {
	
	//종결문자 EOF를 정의
	const char CO_Eof = 0x1A;
	
	std::string freadall(const std::string&);
	bool readstdin(std::string&);
	std::string readall(std::ifstream&);
	std::string readall(std::istream&);
	
}