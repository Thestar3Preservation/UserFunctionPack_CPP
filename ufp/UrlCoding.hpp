#pragma once

#include <string>
#include "Option.hpp"

namespace ufp {
	
	std::string url_encoding(const std::string&, option = "default");
	std::string url_decoding(const std::string&, option = "utf8");
	
}