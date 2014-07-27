#include "Option.hpp"

namespace ufp {
	//옵션 목록에 찾는 값과 일치하는 값이 존재하는지 확인합니다. 존재시, true. 없으면 false를 리턴합니다.
	bool option::check(const std::string &d){
		if ( list.empty() )
			return false;
		for ( std::string i : list )
			if ( d == i )
				return true;
			return false;
	}

	//옵션 리스트를 array형태로 받아들입니다. ex) testa({"asd", "123"});
	option::option(const std::vector<std::string> &d){
		init_multi(d);
	}

	//옵션을 string형태로 받아들입니다. 이 경우, 오직 하나의 옵션만 설정됩니다.
	option::option(const std::string &d){
		if ( d.empty() )
			return;
		init_single(d);
	}

	//옵션을 const char*형태로 받아들입니다. 이 경우, 오직 하나의 옵션만 설정됩니다. 만약 ""를 준다면 아무 값도 설정하지 않습니다. ex) void testa(option a = ""); testa("asd");
	option::option(const char* d){
		if ( std::string(d).empty() )
			return;
		init_single(d);
	}

	//저장된 값을 무시하고 새로운 값을 설정합니다.
	void option::init_single(const std::string &d){
		list = {d};
	}
	void option::init_multi(const std::vector<std::string> &d){
		list = d;
	}

	void option::operator = (const std::vector<std::string> &d) {
		init_multi(d);
	}
	void option::operator = (const std::string &d) {
		init_single(d);
	}
	void option::operator = (const char *d){
		init_single(d);
	}
	void option::operator |= (const std::string &d) {
		set(d);
	}
	void option::operator |= (const char *d) {
		set(d);
	}

	//새로운 값을 뒤에 덧붙입니다.
	void option::set(const char *d){
		set((const std::string)d);
	}
	void option::set(const std::string &d){
		list.push_back(d);
	}

	//새로운 값을 뒤에 덧붙입니다. 이때, {"ㅁㄴㅇ", "ㅁㄴㅇㅁㄴㅇ"} 형태로 받아들입니다.
	void option::set(const std::vector<std::string> &d){
		list.insert(list.end(), d.begin(), d.end());
	}

	//값을 삭제합니다.
	void option::unset(const char *d){
		unset((const std::string)d);
	}
	void option::unset(const std::string &d){
		for (std::vector<std::string>::iterator it = list.begin(); it != list.end();)
			if ( it->compare(d) == 0 )
				list.erase(it);
			else
				++it;
	}
}
