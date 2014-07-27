#pragma once

#include <string>
#include "Option.hpp"
#include <iconv.h>
#include <functional>

namespace ufp {
	
	// 문자열의 인코딩을 변환하고 저장합니다.
	//변환 불가능한 문자가 발견될 경우 비슷한 것으로 대체하여 변환합니다. 그래도 불가능할 경우, 무시하고 넘어갑니다. 즉, 이 함수는 어떻게든 문자열을 변환시킵니다. 만약 옵션을 NoForce로 지정한다면 강제 변환하지 않습니다. 그러나, 사용자가 수동으로 강제 변환 옵션(//TRANSLIT//IGNORE)을 준다면 적용되게 됩니다.
	//SetInTerm, SetOutTerm은 입력 wstring과 출력 wstirng의 각 문자를 char형태로 분할합니다. 고정길이인 경우 숫자를 그대로 입력하면 되고, 가변 길이인 경우 사용자가 수동으로 그 값을 확인하뒤 올바른 바이트 길이를 반환하는 함수를 전달해야 합니다. UTF-16과 같은 가변길이에 바이트 순서가 다른(빅엔디안, 리틀엔디안) 경우 사용자는 자신이 전달하는 원본 문자열의 엔디안에 대해 이해고, 이에 대한 처리를 해주는 함수에 엔디안을 고려하여 처리해야 합니다. 예컨데, UTF16의 경우 원본 문자열의 BOM을 읽거나, 입력 인코딩과 출력 인코딩에 대한 사용자 정의(UTF-16BE/LE)를 미리 알고 SetIn/OutTerm에 전달된 함수에 str[Loc+1]과 같은 식으로 바이트 순서를 미리 고려하여 순서를 알아내야 합니다.
	class iconv {
	private:
		//인코딩
		std::string FromEncoding;
		std::string ToEncoding;
		
		option Opt; //옵션
		iconv_t IconvSetting; //변환 초기화
		std::string Source; //입력
		bool NeedIconvSettingUpdate; //변환 초기화가 수행되어야 하는가? ture = 그렇다. false = 아니다.
		bool IsSetSource; //변환 문자열이 설정되었는가? true = 그렇다. false = 아니다.
		std::string DestStr; //출력 변수
		
		//인코딩에 대한 기본 설정을 위한 상수 및 임시변수
		std::string MatchingTarget; //체크를 위한 임시 변수
		const static char *CO_EncUcs4;
		const static char *CO_EncUcs2;
		const static char *CO_EncUtf32;
		const static char *CO_EncUtf8;
		
		//출력 wstr 문자열의 각 문자를 분할하기 위한 변수
		typedef std::function < unsigned int(const std::string&, const int&) > WstrOutSpliteType;
		WstrOutSpliteType GetOutSize; //wstr을 분할하기 위한 함수
		bool IsSetOutTerm; //출력 간격이 설정되었는가? true = 그렇다. false = 아니다.
		
		//입력 wstr 문자열의 각 문자를 분할하기 위한 변수
		typedef std::function < unsigned int(const std::wstring&, const int&) > WstrInSpliteType;
		WstrInSpliteType GetInSize; //wstr을 분할하기 위한 함수
		bool IsSetInTerm; //입력 간격이 설정되었는가? true = 그렇다. false = 아니다.
		
		//입력 wstirng
		std::wstring SourceWstr;
		bool IsInputWstring; //입력값이 wstring인가? true = 그렇다. false = 아니다.
		
	private:
		void UpdateIconvSetting(); //설정된 인코딩에 대한 iconv설정을 업데이트
		void UnsetIconvSetting(); //iconv사전 설정을 close시킴.
		void Convert(); //문자열 인코딩을 변환.
		void Init(const std::string &FromEncoding, const std::string &ToEncoding); //초기화
		void WorkGetCommon(); //GetStr/Wstr에 대한 공통 처리
		void ConfigToWstr(); //Wstr에 대한 사전 처리에 대한 모든것
		void MargeWstr(std::wstring &DestWstr); //wstr output에 대한 처리. char들을 묶어 wchar_t에 저장하여 wstr를 만듬.
		void SpliteWstr(const std::wstring &Wstr, std::string &Str); //wstr output에 대한 처리. wchar_t를 분할하여 str로 만듬.
		
		//인코딩에 대한 기본 설정을 위한 함수
		void SetDefaultOutEncodingTerm();
		void SetDefaultInEncodingTerm();
		bool CheckMatchEncoding(const std::string &Encoding);
		
	public:
		//생성자
		iconv();
		iconv(const std::string &FromEncoding, const std::string &ToEncoding);
		
		~iconv(); //소멸자
		iconv& SetOption(option Opt); //옵션 설정
		
		//입력 텍스트의 인코딩을 설정
		iconv& SetFrom(const std::string &FromEncoding);
		iconv& SetFrom(const std::string &FromEncoding, const unsigned int &Term);
		iconv& SetFrom(const std::string &FromEncoding, WstrInSpliteType);
		
		//출력 텍스트의 인코딩을 설정
		iconv& SetTo(const std::string &ToEncoding); 
		iconv& SetTo(const std::string &ToEncoding, const unsigned int &Term); 
		iconv& SetTo(const std::string &ToEncoding, WstrOutSpliteType); 
		
		//입력 텍스트를 설정
		iconv& SetSrc(const std::string &Source);
		iconv& SetSrc(const std::wstring &Source);
		iconv& SetSrc(const wchar_t &Source);
		
		//wstring 형태의 입력 텍스트의 각 문자의 바이트 수를 설정
		iconv& SetInTerm(const unsigned int &Term);
		iconv& SetInTerm(WstrInSpliteType);
		
		//wstring 형태의 출력 텍스트의 각 문자의 바이트 수를 설정
		iconv& SetOutTerm(const unsigned int &Term);
		iconv& SetOutTerm(WstrOutSpliteType);
		
		//인코딩을 변환하여 wstring 형태로 반환
		std::wstring GetWstr();
		void GetWstr(std::wstring &Save);
		
		//인코딩을 변환하여 string 형태로 반환
		std::string GetStr();
		void GetStr(std::string &Save);
		
	};
	
}