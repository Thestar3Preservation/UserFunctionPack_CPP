UserFunctionPack_CPP
====================

c++용 사용자 정의 함수 모음

이 저장소는 관리하고 있지 않습니다. 모든 함수는 작동이 되나, 작동을 위해 의존하는 부분(라이브러리, 라이브러리 버전등...)이 이 저장소에서 누락되었을수 있습니다. 사용상의 책임은 지지 않습니다.

compile은 컴파일시 의존 라이브러리나 컴파일 타입(c++0x)을 대신 정하여 처리해주는 부분이나, 큰 의미는 없습니다. 필요한 라이브러리를 찾아 적당한 표준으로 컴파일하면 됩니다.

모든 함수는 테스트를 마친 것들입니다. 

iconvclass에서 변환시 특수문자가 포함되어 있으면 작업이 끝나지 않는 문제가 있습니다. 그외엔 오류 없이 정상적으로 잘 작동합니다. 

trashput은 일반적 환경(ext4포멧, 데비안, xwindow...)에서 사용시 문제 없이 정상적으로 작동하나, 비정상적 환경(접근 불가, 파일 깨짐, 디스크 용량 초과, 아이노드 포화 등...)에 대해서는 처리를 해주지 않았습니다. 베타 버전이니 개인용이 아닌 범용으로 사용하려면 보다 자세한 오류 처리가 필요로 합니다. 

ufp/는 ufp 라이브러리의 소스코드를 담아둔 폴더, function_bin/은 ufp라이브러리르 사용하여 구현한 프로그램의 소스코드를 담아둔 폴더입니다.
