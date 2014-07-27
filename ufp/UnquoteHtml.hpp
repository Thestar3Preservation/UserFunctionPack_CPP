//출처 : http://www.codecodex.com/wiki/Unescape_HTML_special_characters_from_a_String
#pragma once

#include <map>
#include <string>
#include <sstream>

//최상위 namespace시작.
namespace ufp {

	typedef struct {
		const char * Name;
		unsigned int Value;
	} EntityNameEntry;

	typedef std::map<std::string, unsigned int> EntityNameMap;
	typedef std::pair<std::string, unsigned int> EntityNamePair;

	std::string UnquoteHtml(const std::string in);
	void UnquoteHtml(std::stringstream & In, std::ostringstream & Out);

} //최상위 namespace끝.