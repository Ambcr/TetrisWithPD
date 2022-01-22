#ifndef _LOG_UTILS_H_
#define _LOG_UTILS_H_

//Dependency Library
#include <iostream>
#include <string>
#include <thread>

//Platform related
#ifdef WIN32    //Windows 95 and above
#include <windows.h>

#else	//LINUX

#endif //WIN32


#define LOG_MASK 0x10	//LOG MASK
#define VOID void
#define UINT unsigned int
#define LOG_NAME_LEN 10
#define LOG(a,b) WriteLogGTD(__FILE__,__FUNCTION__,__LINE__,a,b)


typedef enum LOG_LEVEL_GTD 
{
	LOG_INFO = 0,
	LOG_DEBUG,
	LOG_WARN,
	LOG_ERROR,
	LOG_FATAL
}LOG_LEVEL;

typedef enum LOG_MASK_GTD
{
	INFO_MASK =0x01 ,
	DEBUG_MASK=0x02,
	WARN_MASK=0x04,
	ERROR_MASK=0x08,
	FATAL_MASK=0x10
};

const char LOG_LEVEL_PRINT[][LOG_NAME_LEN] = {"INFO","DEBUG","WARN","ERROR","FATAL"};


std::string GetTimeMM();
std::thread::id GetProcessId_GTD();

VOID WriteLogGTD(const char * filename,const char * functionname, int line, LOG_LEVEL loglevel, std::string logdetail);
#endif	//_LOG_UTILS_H_