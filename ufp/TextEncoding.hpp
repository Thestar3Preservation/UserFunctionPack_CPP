#pragma once

#include <string>

//일괄 삽입
#include "IconvClass.hpp"

namespace ufp {
	
	//UTF-8 BOM을 정의
	const std::string CO_Bom_Utf8 = "\xEF\xBB\xBF";
	
	bool getEncoding(const std::string&, std::string&);
	bool fgetEncoding(const std::string&, std::string&);
	
	int getutf8byte(const char&);

}