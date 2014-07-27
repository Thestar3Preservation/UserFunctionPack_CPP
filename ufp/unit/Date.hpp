#pragma once

#include <time.h>
#include <string>

namespace ufp {
	
	inline std::string date(const std::string&);
	
	//format에 따른 날자 형식을 반환합니다.
	inline std::string date(const std::string &format){
		time_t cur;
		struct tm *ptm;
		char buf[256] = {0,};
		cur = time(NULL);
		ptm = localtime(&cur);
		strftime(buf, sizeof(buf), format.c_str(), ptm);
		return buf;
	}
	
}