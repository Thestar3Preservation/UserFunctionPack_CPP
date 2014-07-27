#pragma once

#include <string>
#include <openssl/md5.h> //-lssl
#include <sstream>
#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace ufp {
	
	inline std::string fmd5sum(const std::string&);
	
	//대상 파일의 md5sum값을 반환합니다.
	inline std::string fmd5sum(const std::string &filepath) {
		unsigned char result[MD5_DIGEST_LENGTH];
		int file_descript = open(filepath.c_str(), O_RDONLY);
		struct stat statbuf;
		unsigned long file_size = statbuf.st_size;
		char *file_buffer = (char*)mmap(0, file_size, PROT_READ, MAP_SHARED, file_descript, 0);
		MD5((unsigned char*)file_buffer, file_size, result);
		close(file_descript);
		std::ostringstream rec;
		for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
			rec <<std::hex<<(int)result[i]<<":";
		return rec.str();
	}
	
}