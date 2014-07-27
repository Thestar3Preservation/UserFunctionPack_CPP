#pragma once

#include <string>
#include <vector>

namespace ufp {
	
	bool trashput(std::string);
	bool mv(const std::string&, std::string);
	bool cp(const std::string&, const std::string&);
	bool mkdir(const std::string&);
	bool rm(const std::string&);
	std::string pwd();
	bool cd(const std::string &path);
	bool ls(const std::string&, std::vector<std::string>&);
	bool dupev_mkdir(const std::string&, std::string&);
	bool dupev_mv(const std::string&, const std::string&);
	bool dupev_cp(const std::string&, const std::string&);
	
}