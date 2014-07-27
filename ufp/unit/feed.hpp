#pragma once

#include "UserMessageDefine.hpp"

namespace ufp {
	
	// 계속해서 해당 줄에 인자로 준 내용을 갱신하여 표시합니다. 작업진행률 표시등에 사용됩니다. ex) 10% 진행됨... 50%진행됨...; 모든 작업이 완료되면, 다음 줄로 줄바꿈을 해줘야 합니다.
	UserMessageDefine feed(std::cerr, []()->std::string{ return "\r\e[K"; });
	
}