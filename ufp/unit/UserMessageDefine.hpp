#pragma once

#include <iostream>
#include <functional>
#include <string>

namespace ufp {
	
	class UserMessageDefine {
	private:
		std::function<std::string()> fp;
		std::ostream *obj;
	public:
		inline UserMessageDefine(std::ostream &obja, std::function<std::string()> fpa);
		template < typename T > inline std::ostream& operator << (T&);
	};
	
		
	//원하는 메시지를 붙여 원하는 위치로 출력합니다; 첫째 인자는 출력 스트림을 지정, 두번쨰 인자는 객체를 호출할때 먼저 출력할 메세지를 만들어내는 함수입니다. 두번쨰 인자는 아무런 인자를 가지지 않으며 std::string객체를 되돌려주는 형태입니다.
	UserMessageDefine::UserMessageDefine(std::ostream &obja, std::function<std::string()> fpa) {
		fp = fpa;
		obj = &obja;
	}
	
	template < typename T > std::ostream& UserMessageDefine::operator << (T &data) {
		return (*obj)<<fp()<<data;
	}
	
}