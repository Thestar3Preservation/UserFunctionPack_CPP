#!/bin/bash
source ~/.bash_profile
LOAD_USER_FUNTION

LibName=libUserFunctionPack.a	#생성될 라이브러리 명
MainHpp=main.hpp				#통합 헤더 파일 명
Makefile=Makefile				#메이크 파일 이름

WriteMakeBaseHeader(){
	cat >> "$Makefile" <<EOF
#이 파일은 컴파일 과정에서 자동 생성되는 파일입니다.

#########################매크로 정의 부분#########################
.PHONY		= all test release clean install update
CXX			= g++
CPPFLAGS	= -std=c++0x
Srcs		:= \$(wildcard *.cpp)
Objs		:= \$(Srcs:.cpp=.o)
Output		= $LibName

########################명령어 정의 부분###########################
all : test

test : CPPFLAGS += -O0
test : \$(Output)

release : CPPFLAGS += -O3
release : \$(Output)

install : \$(Output)
	linkto lib \$(Output)

update :
	./update_make_file.sh

clean :
	\$(RM) \$(Objs)
	
#########################의존관계 부분#############################
EOF
}

WriteMakeObjectDependency(){
	local i
	for i in *.cpp; do
		echo
		g++ -MM "$i"
		echo $'\t''$(CXX) $(CPPFLAGS) -c '"$i"
	done >> "$Makefile"
}

WriteMakeMakeLib(){
	cat >> "$Makefile" <<EOF

\$(Output) : \$(Objs)
	ar rc \$(Output) \$(Objs)
EOF
}

MakeAllInOneHeaderFileToThisLib(){
	{
		echo '#pragma once'
		echo
		local i
		for i in *.hpp unit/*.hpp; do
			[ "$i" = "$MainHpp" ] && continue
			echo "#include \"$i\""
		done
	} > "$MainHpp"
}

MakeMakeFile(){
	trash-put -f "$Makefile"
	WriteMakeBaseHeader
	WriteMakeMakeLib
	WriteMakeObjectDependency
}

Main(){
	MakeMakeFile
	
	MakeAllInOneHeaderFileToThisLib
	
	exit 0
}

Main

# g++ -std=c++0x -fPIC -c *.cpp || exit 1
# g++ -fPIC -shared *.o -o "$LibName" || exit 1