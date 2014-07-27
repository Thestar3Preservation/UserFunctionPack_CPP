#pragma once

#include <string>
#include <vector>

namespace ufp {

	class option {
	private:
		std::vector<std::string> list;
		
		void init_single(const std::string&);
		void init_multi(const std::vector<std::string>&);
		
	public:
		option(const std::vector<std::string>&);
		option(const std::string&);
		option(const char*);
		option(){};
		
		void set(const char*);
		void set(const std::string&);
		void set(const std::vector<std::string>&);
		
		void unset(const char*);
		void unset(const std::string&);
		
		bool check(const std::string&);
		
		void operator = (const std::vector<std::string>&);
		void operator = (const std::string&);
		void operator = (const char*);
		void operator |= (const std::string&);
		void operator |= (const char*);
	};

}