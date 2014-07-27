#include "TextEncoding.hpp"

#include <iostream>
#include <fstream>
#include <unicode/uchar.h> //-licuuc
#include <unicode/ucsdet.h> //-licui18n
#include "FileIo.hpp"
#include <vector>

namespace ufp {
	
	//인코딩을 얻어냅니다.
	bool getEncoding(const std::string &code, std::string &encoding) {
		UErrorCode err = U_ZERO_ERROR;
		UCharsetDetector* det = ucsdet_open(&err);
		if(U_FAILURE(err))
			return false;
		ucsdet_setText(det, code.c_str(), code.length(), &err);
		if(U_FAILURE(err))
			return false;
		encoding = ucsdet_getName(ucsdet_detect(det, &err), &err);
		if(U_FAILURE(err))
			return false;
		return true;
	}
	bool fgetEncoding(const std::string &filepath, std::string &encoding) {
		std::ifstream temp(filepath);
		bool sucess = getEncoding(readall(temp), encoding);
		temp.close();
		return sucess;
	}
	
	//문자의 첫 바이트를 입력받아 utf8문자의 바이트 길이를 알아냅니다. 반환 값은 실제 길이 보다 하나 작습니다.
	int getutf8byte(const char &code) {
		if ( ( code>>3 & 0x1F ) == 0x1E ) {
			return 3;
		} else if ( ( code>>4 & 0x0F ) == 0x0E ) {
			return 2;
		} else if ( ( code>>5 & 0x07 ) == 0x06 ) {
			return 1;
		} else {
			return 0;
		}
	}
	
}