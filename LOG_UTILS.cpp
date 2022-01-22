#include "LOG_UTILS.h"
VOID WriteLogGTD(const char* filename, const char* functionname, int line, LOG_LEVEL loglevel,  std::string logdetail)
{
	switch (loglevel)
	{
		case LOG_INFO:
			if ((INFO_MASK & LOG_MASK) == 0) return;
			break;
		case LOG_DEBUG:
			if ((DEBUG_MASK & LOG_MASK) == 0) return;
			break;
		case LOG_WARN:
			if ((WARN_MASK & LOG_MASK) == 0) return;
			break;
		case LOG_ERROR:
			if ((ERROR_MASK & LOG_MASK) == 0) return;
			break;
		case LOG_FATAL:
			if ((FATAL_MASK & LOG_MASK) == 0) return;
			break;
		default:
			return;
	}
	std::cout << "[" << GetTimeMM() << "]";
	std::cout << "[" << GetProcessId_GTD() << "]";
	std::cout << "[" << LOG_LEVEL_PRINT[loglevel] << "]";
	std::cout << "[" << strrchr(filename,'\\')+1 ;
	std::cout << "  ";
	std::cout << functionname <<"()";
	std::cout << "::";
	std::cout << line << "]";
	std::cout << " " << logdetail <<std::endl;

}

std::string GetTimeMM()
{	
	std::string time_str="";
#ifdef WIN32	//WINDOWS
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	time_str = std::to_string(sys.wYear) + "-" + std::to_string(sys.wMonth)+"-"\
		+ std::to_string(sys.wDay) + " " + std::to_string(sys.wHour) + ":"\
		+ std::to_string(sys.wMinute) + ":" + std::to_string(sys.wSecond) + "."\
		+ std::to_string(sys.wMilliseconds) ;
#else	//LINUX

#endif
	
	return time_str;

}

std::thread::id GetProcessId_GTD()
{
	std::thread::id tid = std::this_thread::get_id();
	return tid;
}
