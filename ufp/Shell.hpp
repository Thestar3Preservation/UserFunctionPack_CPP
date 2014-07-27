#pragma once

#include <string>

namespace ufp {
	
	int system(const std::string &cmd);
	int system(const std::string &cmd, std::string &ret, std::string = "out");
	std::string s_quote(const std::string&);
	bool GetEnv(const std::string &ValueName, std::string &ReturnValue);
	
}