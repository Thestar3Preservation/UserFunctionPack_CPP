#pragma once

#include <string>
#include <functional>
#include <vector>
#include "Option.hpp"

namespace ufp {
	
// 	class Path {
// 		std::string PathName;
// 		inline Path(std::string PathName){
// 			this.PathName = PathName;
// 		}
// 		inline std::string GetString(){
// 			return PathName;
// 		}
// 		inline &Path FullPath(){
// 			PathName = fullpath(PathName);
// 			return *Path;
// 		}
// 		inline &Path AbsolutePath(){
// 			PathName =  realpath(PathName);
// 			return *Path;
// 		}
// 		inline &Path SubstituteSpecialChar(){
// 			PathName =  wl_replace(PathName);
// 			return *Path;
// 		}
// 		inline &Path Name(){
// 			PathName =  ex_name(PathName);
// 			return *Path;
// 		}
// 		inline &Path Extension(){
// 			PathName =  ex_ext(PathName);
// 			return *Path;
// 		}
// 		inline &Path Basename(){
// 			PathName =  basename(PathName);
// 			return *Path;
// 		}
// 		inline &Path Dirname(){
// 			PathName =  dirname(PathName);
// 			return *Path;
// 		}
// 	};
	
	bool dupevLCodef(std::function<bool(std::string, std::string)>, const std::string&, std::string, bool = false);
	bool dupevLCode(const std::string&, const std::string&, const std::string&, bool = false);
	std::string dupev_name(std::string, const std::string& = "", bool = false, std::vector<std::string> = std::vector<std::string>());
	std::string trim_name(std::string);
	std::string trim_webname(std::string, option = "");
	std::string dirname(std::string);
	std::string basename(std::string);
	bool ex_ext(std::string, std::string&, bool = false);
	std::string ex_name(std::string);
	std::string realpath(const std::string&);
	std::string fullpath(const std::string&);
	std::string wl_replace(std::string);
	
}