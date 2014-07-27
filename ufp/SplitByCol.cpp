#include "SplitByCol.hpp"

#include <unicode/ucsdet.h> //-licui18n
#include "TextEncoding.hpp"
#include <wchar.h>

namespace ufp {

	/**유니코드 값이 가진 폭을 알아냅니다.
	 * 이 함수를 이해하고 싶다면, 다음 비공식 문서를 참조하십시오. http://www.unicode.org/reports/tr11/
	 * @return 해당 유니코드의 폭
	 */
	int getCharLength(
		const UChar32 &Unicode ///< 유니코드
	) {
		int Type = u_getIntPropertyValue(Unicode, UCHAR_EAST_ASIAN_WIDTH); //Character가 가진 동아시아 폭 속성을 가져옵니다.
		int Width; //문자 폭
		
		//가져온 값이 비정상적 범위에 존재한다면 종료.
		if (Type < 0 || Type >= U_EA_COUNT)
			return 0;
		
		switch ( Type ) {
			//(W) 와이드 동아시아 : 넓은 항상 다른 모든 문자 . 이러한 문자 들은 (예 : 통합 한 표의 문자 또는 제곱 가타카나 기호 등 ) 넓은 문자입니다 동아시아 타이포그래피 의 맥락 에서만 발생합니다. 이 카테고리는 명시 적 반자 대응 이 문자 가 포함되어 있습니다.
			case U_EA_WIDE:
			//동아시아 전체 너비 (F) : 암시 적으로 좁은 하지만 대면 하다 다른 곳에서 유니 코드 표준 의 문자 에 형식 <wide> 의 호환성 분해 함으로써 유니 코드 표준 [ 유니 코드 ] 의 전체 너비 로 정의 된 모든 문자 .
			case U_EA_FULLWIDTH:
				Width = 2;
				break;
				
			//동아시아 모호한 (A) : 때로는 넓고 때로는 좁은 할 수있는 모든 문자 . 모호한 문자는 더 이상 자신의 폭 을 해결하기 위해 문자 코드 에 포함 되지 않은 추가 정보가 필요합니다.
			case U_EA_AMBIGUOUS:
			//동아시아 반폭 (H) : 명시 적으로 암시 적으로 대형, 취소됨 , 플러스 U +20 A9 ₩ 원 이십니까 다른 유니 코드 표준 의 문자 에 형식 <narrow> 의 호환성 분해 함으로써 유니 코드 표준 의 반폭 으로 정의 된 모든 문자 .
			case U_EA_HALFWIDTH:
			//( 나 ) 좁은 동아시아 : 항상 좁고 명시 적 전각 또는 넓은 대응 이 다른 모든 문자 . 그들은 명시 적 전각 또는 넓은 대응 을 가지고 있기 때문에 이러한 문자는 동아시아 타이포그래피 와 기존 문자 세트 에서 암시 적으로 좁은 . ASCII 의 모든 동아시아 좁은 문자 의 예입니다.
			case U_EA_NARROW:
				Width = 1;
				break;
				
			//중립 ( 안 동아시아 ) : 다른 모든 문자 . 중립 문자는 기존의 동아시아 문자 집합 에서 발생하지 않습니다. 더 나아가 , 그들은 또한 동아시아 타이포그래피 발생하지 않습니다. 예를 들어 , 데바 나가리 를 조판 에는 일본의 전통적인 방법이 없습니다. 좁은 중립 문자 의 정식 상응하는 자체가 각각 좁은 중립 하지 않을 수 있습니다. 예를 들어 , U +00 C5 라틴 대문자WITH 링 위에 중립 , 그러나 그것의 분해는좁은 문자로 시작합니다.
			case U_EA_NEUTRAL:
				Width = wcwidth(Unicode); //문자 폭을 알아냄.
				//출력 불가능한 문자라면 문자 길이는 0으로 설정
				if ( Width == -1 ) {
					Width = 0; 
				}
				break;
		}
		
		return Width;
	}
	
	//칸수를 기준으로 문자를 끊습니다. widecharter의 길이를 찾아내는데 실패하여, 임의의 값을 지정해두었습니다(계산 결과가 잘못될수 있습니다).
	std::string SplitBycol(const std::string &src, const unsigned int &col, option opt) {
		bool SkipBashControlCharacters = opt.check("SkipBashControlCharacters"); //이 옵션이 켜지면, bash 쉘의 제어문자(색상 표시 문자열 등)는 길이 계산에서 제외함.
		std::wstring srcWstrTemp, resWstrTemp;
		std::string srcTemp;
		iconv("UTF-8", "UCS-4BE").SetSrc(src).GetStr(srcTemp);
		int srcLeng = srcTemp.length();
		for(int i=0; i<srcLeng; i+=4)
			srcWstrTemp += (srcTemp[i]&0xFF) << 24 | (srcTemp[i+1]&0xFF) << 16 | (srcTemp[i+2]&0xFF) << 8 | srcTemp[i+3]&0xFF;
		int leng = 0;
		bool skip = false;
		for ( wchar_t i : srcWstrTemp ) {
			if ( SkipBashControlCharacters ) {
				if ( skip ) {
					if ( i <= 0x7F && isalpha((char)i) )
						skip = false;
				} else {
					if ( i == '\e' )
						skip = true;
				}
			}
			if ( skip ) {
				resWstrTemp += i;
				continue;
			}
			int cleng = getCharLength((UChar32)i);
			leng += cleng;
			if ( col > leng )
				resWstrTemp += i;
			else if ( col < leng ) {
				resWstrTemp += '\n';
				resWstrTemp += i;
				leng = cleng;
			} else {
				resWstrTemp += i;
				resWstrTemp += '\n';
				leng = 0;
			}
		}
		std::string resStrTemp, res;
		for(wchar_t i : resWstrTemp) {
			resStrTemp += i >> 24 & 0xFF;
			resStrTemp += i >> 16 & 0xFF;
			resStrTemp += i >> 8 & 0xFF;
			resStrTemp += i & 0xFF;
		}
		iconv("UCS-4BE", "UTF-8").SetSrc(resStrTemp).GetStr(res);
		return res;
	}
	
}