#include "StringManipulation.hpp"

#include <sstream>
#include <stdexcept>
#include "TextEncoding.hpp"

namespace ufp {
	
	
	//패턴의 모든 문자열을 치환합니다.
	std::string replaceAll(std::string result, const std::string &pattern, const std::string &replace) {
		std::string::size_type pos = 0;
		std::string::size_type offset = 0;
		while( ( pos = result.find(pattern, offset) ) != std::string::npos ) {
			int pSize =  pattern.size(), rSize = replace.size();
			auto begin = result.begin();
			result.replace(begin + pos, begin + pos + pSize, replace);
			offset = pos + rSize;
		}
		return result;
	}
	
	
	//입력된 내용에서 영문자를 대소문자를 무시하는 정규표현식으로 변환시킵니다.
	std::string replace_i(std::string value){
		std::ostringstream temp;
		int max = value.length(), len;
		for(int i=0; i<max; i++) {
			len = getutf8byte(value[i]);
			if ( len > 0 ) {
				for (int j = 0; j <= len; j++)
					temp <<value[i+j];
				i += len;
				continue;
			}
			if ( isupper(value[i]) )
				temp <<"["<<value[i]<<(char)tolower(value[i])<<"]";
			else if ( islower(value[i]) )
				temp <<"["<<(char)toupper(value[i])<<value[i]<<"]";
			else
				temp <<value[i];
		}
		return temp.str();
	}

	
	//현재 grep의 정규표현식 G,E에서 정상 작동함. 다른 정규표현식에도 그대로 적용될수 있을듯...
	std::string g_quote(const std::string &code){
		std::ostringstream temp;
		int max = code.length(), len;
		for(int i=0; i<max; i++) {
			len = getutf8byte(code[i]);
			if ( len > 0 ) {
				for (int j = 0; j <= len; j++)
					temp <<code[i+j];
				i += len;
				continue;
			}
			if ( code[i] == '\n' )
				temp <<"\\n";
			else if ( iscntrl(code[i]) != 0 )
				temp <<"["<<code[i]<<"]";
			else if ( code[i] == '^' || code[i] == '/' || code[i] == '#' )
				temp <<"\\"<<code[i];
			else if ( isalnum((int)code[i]) != 0 )
				temp <<code[i];
			else
				temp <<"["<<code[i]<<"]";
		}
		return(temp.str());
	}

	
	//치환될 내용에 들어갈 내용이 정규표현식으로 해석되지 않도록 합니다.
	std::string r_quote(const std::string &code){
		return replaceAll(replaceAll(replaceAll(code, "\\", "\\\\"), "/", "\\/"), "#", "\\#");
	}
	
	
}