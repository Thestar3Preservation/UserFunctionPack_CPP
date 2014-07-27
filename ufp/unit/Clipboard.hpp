#pragma once

#include "../Shell.cpp"

namespace ufp {
	
	inline std::string ClipOut();
	inline void ClipOut(std::string&);
	
	std::string ClipOut(){
		std::string temp;
		system("xclip -o -selection clip", temp, "out");
		return temp;
	}
	
	void ClipOut(std::string &Str){
		Str = ClipOut();
	}
	
}