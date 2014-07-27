#line 2 "SplitBycol.hpp"
#pragma once

#include <string>
#include <unicode/uchar.h> //-licuuc
#include "Option.hpp"

namespace ufp {

	std::string SplitBycol(const std::string&, const unsigned int&, option = "");
	int getCharLength(const UChar32&);
	
}