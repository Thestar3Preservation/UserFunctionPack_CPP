#include "IconvClass.hpp"

#include <stdexcept>
#include <pcrecpp.h>
#include <iostream>
#include "TextEncoding.hpp"

namespace ufp {
	
	void iconv::GetStr(std::string &Save){
		Save = GetStr();
	}
	
	std::string iconv::GetStr(){
		WorkGetCommon();
		std::string Str = DestStr;
		DestStr.clear();
		return Str;
	}
	
	void iconv::GetWstr(std::wstring &Save){
		Save = GetWstr();
	}
	
	std::wstring iconv::GetWstr(){
		ConfigToWstr();
		
		//변환된 값을 wstring에 담음.
		std::wstring DestWstr;
		MargeWstr(DestWstr);
		DestStr.clear();
		
		return DestWstr;
	}
	
	iconv& iconv::SetOutTerm(const unsigned int &Term){
		this->GetOutSize = 
		[Term] (const std::string &Src, const int &Loc) -> unsigned int {
			return Term;
		};
		IsSetOutTerm = true;
		return *this;
	}
	
	iconv& iconv::SetOutTerm(WstrOutSpliteType GetOutSize){
		this->GetOutSize = GetOutSize;
		IsSetOutTerm = true;
		return *this;
	}
	
	iconv& iconv::SetInTerm(const unsigned int &Term){
		this->GetInSize = 
		[Term] (const std::wstring &Wsrc, const int &Loc) -> unsigned int {
			return Term;
		};
		IsSetInTerm = true;
		return *this;
	}
	
	iconv& iconv::SetInTerm(WstrInSpliteType GetInSize){
		this->GetInSize = GetInSize;
		IsSetInTerm = true;
		return *this;
	}
	
	iconv& iconv::SetFrom(const std::string &FromEncoding, const unsigned int &Term){
		SetInTerm(Term);
		return SetFrom(FromEncoding);
	}
	
	iconv& iconv::SetFrom(const std::string &FromEncoding, WstrInSpliteType GetInSize){
		SetInTerm(GetInSize);
		return SetFrom(FromEncoding);
	}
	
	iconv& iconv::SetTo(const std::string &ToEncoding, const unsigned int &Term){
		SetOutTerm(Term);
		return SetTo(ToEncoding);
	}
	
	iconv& iconv::SetTo(const std::string &ToEncoding, WstrOutSpliteType GetOutSize){
		SetOutTerm(GetOutSize);
		return SetTo(ToEncoding);
	}
	
	void iconv::MargeWstr(std::wstring &DestWstr){
		int Max = DestStr.size();
		for(int i = 0; i < Max; ) {
			int Size = GetOutSize(DestStr, i);
			wchar_t a = (wchar_t)NULL;
			for (int j = Size - 1; j >= 0; j--) {
				a |= ( DestStr[ i + j ] & 0xFF ) << ( Size - j - 1 ) * 8;
			}
			DestWstr += a;
			i += Size;
		}
	}
	
	void iconv::SpliteWstr(const std::wstring &Wstr, std::string &Str){
		int Max = Wstr.size();
		for (int i = 0; i < Max; i++) {
			int Size = GetInSize(Wstr, i);
			for (int j = Size - 1; j >= 0; j--) {
				Str += Wstr[i] >> (8*j) & 0xFF;
			}
		}
	}
	
	iconv& iconv::SetSrc(const wchar_t &Source){
		wchar_t WcharTemp[2] = {Source, L'\0'};
		return SetSrc(WcharTemp);
	}
	
	iconv& iconv::SetSrc(const std::wstring &Source){
		this->SourceWstr = Source;
		IsInputWstring = true;
		IsSetSource = true;
		return *this;
	}
	
	iconv& iconv::SetSrc(const std::string &Source){
		this->Source = Source;
		IsInputWstring = false;
		IsSetSource = true;
		return *this;
	}
	
	iconv& iconv::SetTo(const std::string &ToEncoding){
		this->ToEncoding = ToEncoding;
		NeedIconvSettingUpdate = true;
		return *this;
	}
	
	iconv& iconv::SetFrom(const std::string &FromEncoding){
		this->FromEncoding = FromEncoding;
		NeedIconvSettingUpdate = true;
		return *this;
	}
	
	iconv& iconv::SetOption(option Opt){
		this->Opt = Opt;
		NeedIconvSettingUpdate = true;
		return *this;
	}
	
	iconv::~iconv(){
		UnsetIconvSetting();
	}
	
	iconv::iconv(const std::string &FromEncoding, const std::string &ToEncoding){
		Init(FromEncoding, ToEncoding);
	}
	
	iconv::iconv(){
		Init("UTF-8", "UTF-8");
	}
	
	void iconv::Init(const std::string &FromEncoding, const std::string &ToEncoding){
		this->FromEncoding = FromEncoding;
		this->ToEncoding = ToEncoding;
		NeedIconvSettingUpdate = true;
		IsSetSource = false;
		IsSetOutTerm = false;
		IsSetInTerm = false;
		IconvSetting = (iconv_t)(-1);
	}
	
	void iconv::WorkGetCommon(){
		//입력
		if ( IsInputWstring ) {
			//문자 길이 설정
			SetDefaultInEncodingTerm();
			if ( ! IsSetInTerm ) {
				throw std::runtime_error("iconv : 입력 wstring의 각 문자 길이를 설정해야 합니다!");
			}
			
			//문자를 분할.
			std::string Str;
			SpliteWstr(SourceWstr, Str);
			this->Source = Str;
		}
		
		if ( ! IsSetSource ) {
			throw std::runtime_error("iconv : 변환될 문자열을 설정하지 않았습니다!");
		}
		UpdateIconvSetting();
		Convert();
	}
	
	void iconv::Convert(){
		//입력 설정
		std::vector<char> SrcBuf(Source.begin(), Source.end());
		char *SrcPtr = &SrcBuf[0];
		size_t SrcSize = Source.size();
		
		//버퍼 설정
		std::vector<char> DestBuf(1024);
		
		//변환
		while (0 < SrcSize) {
			char *DestPtr = &DestBuf[0];
			size_t DestSize = DestBuf.size();
			::iconv(IconvSetting, &SrcPtr, &SrcSize, &DestPtr, &DestSize);
			DestStr.append(&DestBuf[0], DestBuf.size() - DestSize);
		}
	}
	
	void iconv::UnsetIconvSetting(){
		if ( IconvSetting != (iconv_t)(-1) ) {
			iconv_close(IconvSetting);
		}
	}
	
	void iconv::SetDefaultInEncodingTerm(){
		if ( IsSetInTerm ) {
			return;
		}
		
		MatchingTarget = FromEncoding;
		
		IsSetInTerm = true;
		
		//UCS-4
		if ( CheckMatchEncoding(CO_EncUcs4) ) {
			SetInTerm(4);
		}
		//UCS-2
		else if ( CheckMatchEncoding(CO_EncUcs2) ) {
			SetInTerm(2);
		}
		//UTF-32
		else if ( CheckMatchEncoding(CO_EncUtf32) ) {
			SetInTerm(4);
		}
		//UTF-8
		else if ( CheckMatchEncoding(CO_EncUtf8) ) {
			SetInTerm([](const std::wstring &Wsrc, const int &Loc) -> unsigned int {
				if ( 0xF0000000 < Wsrc[Loc] ) {
					return 4;
				} else if ( 0xE00000 < Wsrc[Loc] ) {
					return 3;
				} else if ( 0xC000 < Wsrc[Loc] ) {
					return 2;
				} else {
					return 1;
				}
			});
		}
		//조건에 없는 경우
		else {
			IsSetInTerm = false;
		}
	}
	
	bool iconv::CheckMatchEncoding(const std::string &Encoding) {
		return pcrecpp::RE("(^|//)" + Encoding + "($|//)", PCRE_CASELESS | PCRE_UTF8).PartialMatch(MatchingTarget);
	}
	
	const char *iconv::CO_EncUcs4 = "UCS-?4(BE|LE)?";
	const char *iconv::CO_EncUcs2 = "UCS-?2(BE|LE)?";
	const char *iconv::CO_EncUtf32 = "UTF-?32(BE|LE)?";
	const char *iconv::CO_EncUtf8 = "UTF-?8";
	
	void iconv::SetDefaultOutEncodingTerm(){
		if ( IsSetOutTerm ) {
			return;
		}
		
		MatchingTarget = ToEncoding;
		
		IsSetOutTerm = true;
		
		//UCS-4
		if ( CheckMatchEncoding(CO_EncUcs4) ) {
			SetOutTerm(4);
		}
		//UCS-2
		else if ( CheckMatchEncoding(CO_EncUcs2) ) {
			SetOutTerm(2);
		}
		//UTF-32
		else if ( CheckMatchEncoding(CO_EncUtf32) ) {
			SetOutTerm(4);
		}
		//UTF-8
		else if ( CheckMatchEncoding(CO_EncUtf8) ) {
			SetOutTerm([](const std::string &Src, const int &Loc) -> unsigned int {
				return getutf8byte(Src[Loc]) + 1;
			});
		}
		//조건에 없는 경우
		else {
			IsSetOutTerm = false;
		}
	}
	
	void iconv::UpdateIconvSetting(){
		if ( ! NeedIconvSettingUpdate ) {
			return;
		}
		UnsetIconvSetting();
		std::string ToEncoding = this->ToEncoding;
		std::string FromEncoding = this->FromEncoding;
		if ( ! Opt.check("NoForce") ) {
			ToEncoding += "//TRANSLIT//IGNORE";
			FromEncoding += "//TRANSLIT//IGNORE";
		}
		IconvSetting = iconv_open(ToEncoding.c_str(), FromEncoding.c_str());
		if ( IconvSetting == (iconv_t)(-1) ) {
			throw std::runtime_error("iconv 초기화 과정에 문제가 생겼습니다!");
		}
		NeedIconvSettingUpdate = false;
	}
	
	void iconv::ConfigToWstr(){
		//출력
		SetDefaultOutEncodingTerm();
		if ( ! IsSetOutTerm ) {
			throw std::runtime_error("iconv : 출력 wstring의 각 문자 길이를 설정해야 합니다!");
		}
		
		//공통
		WorkGetCommon();
	}
	
}