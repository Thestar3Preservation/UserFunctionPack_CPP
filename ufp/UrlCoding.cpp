#include "UrlCoding.hpp"

#include <sstream>
#include <string.h>
#include <stdexcept>
#include "TextEncoding.hpp"

namespace ufp {
	
	
	// url Encoding된 스타일로 바꾸어줌.
	std::string url_encoding(const std::string &value, option opt) {
		int n = value.length(), i = 0;
		std::ostringstream escaped;
		escaped <<std::hex;
		auto func = [&]()->bool{
			int len = getutf8byte(value[i]);
			if ( len == 0 )
				return false;
			for (int j = 0; j <= len; j++)
				escaped <<'%'<<((unsigned int)value[i+j] & 0xFF);
			i += len;
			return true;
		};
		if ( opt.check("default") ) {
			//unreserved를 제외한 모든 문자를 Percent-encoding함.
			for (; i != n; i++) {
				if (func())
					continue;
				if (isalnum(value[i]) != 0 || value[i] == '-' || value[i] == '_' || value[i] == '.' || value[i] == '~')
					escaped << value[i];
				else
					escaped <<'%'<<((int)value[i]);
			}
		} else if ( opt.check("web") ) {
			//reserved와 unreserved를 제외한 모든 문자를 Percent-encoding함.
			for (; i != n; i++) {
				if (func())
					continue;
				if (isalnum(value[i]) != 0 || value[i] == '-' || value[i] == '_' || value[i] == '.' || value[i] == '~' || value[i] == '!' || value[i] == '*' || value[i] == '\'' || value[i] == '(' || value[i] == ')' || value[i] == ';' || value[i] == ':' || value[i] == '@' || value[i] == '&' || value[i] == '=' || value[i] == '+' || value[i] == '$' || value[i] == ',' || value[i] == '/' || value[i] == '?' || value[i] == '#' || value[i] == '[' || value[i] == ']')
					escaped << value[i];
				else
					escaped <<'%'<<((int)value[i]);
			}
		} else if ( opt.check("path") ) {
			//unreserved와 '/'문자를 제외한 모든 문자를 Percent-encoding함.
			for (; i != n; i++) {
				if (func())
					continue;
				if (isalnum(value[i]) != 0 || value[i] == '-' || value[i] == '_' || value[i] == '.' || value[i] == '~' || value[i] == '/' || value[i] == ' ')
					escaped << value[i];
				else
					escaped <<'%'<<((int)value[i]);
			}
		} else
			throw std::logic_error("잘못된 옵션입니다!");
		return escaped.str();
	}

	//url를 Decoding함.
	std::string url_decoding(const std::string &code, option opt) {
		const char *value = code.c_str();
		int len = strlen(value), i = 0;
		std::ostringstream unescaped;
		auto func = [&]()->bool{
			int leng = getutf8byte(value[i]);
			if ( 0 == leng )
				return false;
			for (int j = 0; j <= leng; j++)
				unescaped <<value[i+j];
			i += leng;
			return true;
		};
		if ( opt.check("utf8") ) {
			//표준. %[a-z0-9]{2}식의 퍼센트 이스케이프된 내용을 디코딩함.
			for(; i < len; i++){
				if ( func() )
					continue;
				if ( value[i] == '%' ) {
					if ( (i+2) < len && isalnum(value[i+1]) !=0 && isalnum(value[i+2]) != 0 ) {
						unescaped <<(char)strtol(new char[3]{value[i+1], value[i+2], '\0'}, NULL, 16);
						i += 2;
					} else
						unescaped <<"%";
				} else
					unescaped <<value[i];
			}
		} else if ( opt.check("utf16") ) {
			//비표준. %u[a-z0-9]{4}식의 퍼센트 이스케이프된 내용을 디코딩함.
			for(; i < len; i++){
				if ( func() )
					continue;
				if ( value[i] == '%' ) {
					if ( i + 5 < len && value[i+1] == 'u' ) {
						if ( isalnum(value[i+2]) !=0 && isalnum(value[i+3]) != 0 && isalnum(value[i+4]) != 0 && isalnum(value[i+5]) != 0 ) {
							unescaped <<(char)strtol(new char[3]{value[i+2], value[i+3], '\0'}, NULL, 16)<<(char)strtol(new char[3]{value[i+4], value[i+5], '\0'}, NULL, 16);
							i += 5;
						} else {
							unescaped <<"%u";
							i++;
						}
					} else
						unescaped <<"%";
				} else
					unescaped <<value[i];
			}
		} else if ( opt.check("all") ) {
			//utf8과 utf16방식으로 디코딩함. 잘못 디코딩될수 있음. 혼합된 경우 수동으로 분석하는것을 추천.
			for(; i < len; i++){
				if ( func() )
					continue;
				if ( value[i] == '%' ) {
					if ( i + 5 < len && value[i+1] == 'u' ) {
						if ( isalnum(value[i+2]) !=0 && isalnum(value[i+3]) != 0 && isalnum(value[i+4]) != 0 && isalnum(value[i+5]) != 0 ) {
							unescaped <<(char)strtol(new char[3]{value[i+2], value[i+3], '\0'}, NULL, 16)<<(char)strtol(new char[3]{value[i+4], value[i+5], '\0'}, NULL, 16);
							i += 5;
						} else if ( isalnum(value[i+2]) !=0  ) {
							unescaped <<(char)strtol(new char[3]{'u', value[i+2], '\0'}, NULL, 16);
							i += 2;
						} else {
							unescaped <<"%u";
							i++;
						}
					} else if ( i + 2 < len && isalnum(value[i+1]) != 0 && isalnum(value[i+2]) != 0 ) {
						unescaped <<(char)strtol(new char[3]{value[i+1], value[i+2], '\0'}, NULL, 16);
						i += 2;
					} else
						unescaped <<"%";
				} else
					unescaped <<value[i];
			}
		} else
			throw std::logic_error("잘못된 옵션입니다!");
		return unescaped.str();
	}
	
	
}