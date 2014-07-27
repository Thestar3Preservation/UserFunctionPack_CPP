#이 파일은 컴파일 과정에서 자동 생성되는 파일입니다.

#########################매크로 정의 부분#########################
.PHONY		= all test release clean install update
CXX			= g++
CPPFLAGS	= -std=c++0x
Srcs		:= $(wildcard *.cpp)
Objs		:= $(Srcs:.cpp=.o)
Output		= libUserFunctionPack.a

########################명령어 정의 부분###########################
all : test

test : CPPFLAGS += -O0
test : $(Output)

release : CPPFLAGS += -O3
release : $(Output)

install : $(Output)
	linkto lib $(Output)

update :
	./update_make_file.sh

clean :
	$(RM) $(Objs)
	
#########################의존관계 부분#############################

$(Output) : $(Objs)
	ar rc $(Output) $(Objs)

FileIo.o: FileIo.cpp FileIo.hpp TextEncoding.hpp IconvClass.hpp \
 Option.hpp
	$(CXX) $(CPPFLAGS) -c FileIo.cpp

FileTable.o: FileTable.cpp FileTable.hpp FileIo.hpp UrlCoding.hpp \
 Option.hpp PathName.hpp StringManipulation.hpp unit/to_string.hpp \
 TextEncoding.hpp IconvClass.hpp unit/Date.hpp
	$(CXX) $(CPPFLAGS) -c FileTable.cpp

IconvClass.o: IconvClass.cpp IconvClass.hpp Option.hpp TextEncoding.hpp
	$(CXX) $(CPPFLAGS) -c IconvClass.cpp

Option.o: Option.cpp Option.hpp
	$(CXX) $(CPPFLAGS) -c Option.cpp

PathName.o: PathName.cpp PathName.hpp Option.hpp TextEncoding.hpp \
 IconvClass.hpp StringManipulation.hpp unit/to_string.hpp UrlCoding.hpp \
 FileTable.hpp Shell.hpp
	$(CXX) $(CPPFLAGS) -c PathName.cpp

Shell.o: Shell.cpp Shell.hpp StringManipulation.hpp FileIo.hpp
	$(CXX) $(CPPFLAGS) -c Shell.cpp

SplitByCol.o: SplitByCol.cpp SplitByCol.hpp Option.hpp TextEncoding.hpp \
 IconvClass.hpp
	$(CXX) $(CPPFLAGS) -c SplitByCol.cpp

StringManipulation.o: StringManipulation.cpp StringManipulation.hpp \
 TextEncoding.hpp IconvClass.hpp Option.hpp
	$(CXX) $(CPPFLAGS) -c StringManipulation.cpp

TextEncoding.o: TextEncoding.cpp TextEncoding.hpp IconvClass.hpp \
 Option.hpp FileIo.hpp
	$(CXX) $(CPPFLAGS) -c TextEncoding.cpp

UnquoteHtml.o: UnquoteHtml.cpp UnquoteHtml.hpp
	$(CXX) $(CPPFLAGS) -c UnquoteHtml.cpp

UrlCoding.o: UrlCoding.cpp UrlCoding.hpp Option.hpp TextEncoding.hpp \
 IconvClass.hpp
	$(CXX) $(CPPFLAGS) -c UrlCoding.cpp

base64.o: base64.cpp base64.hpp
	$(CXX) $(CPPFLAGS) -c base64.cpp
