#ifndef SELOG_H
#define SELOG_H

#ifdef SE_DEBUG

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define SE_LogManager			se_debug::SELogManager::getObj()
//#define SE_Assert(...)			se_debug::SELogManager::getObj().assert(__VA_ARGS__)
#define SE_LogManager_Release	se_debug::SELogManager::release()

#define SE_LOG_ENABLED	0x01
#define SE_LOG_USERONLY 0x02
#define SE_LOG_LOCAL	0x04

namespace se_debug {

enum logType {
	LOGTYPE_UNKNOWN,	//0
	LOGTYPE_GENERAL,	//1
	LOGTYPE_WARNNING,	//2
	LOGTYPE_ERROR,		//3
	LOGTYPE_OPENGL,		//4
	LOGTYPE_SFML,		//5
	LOGTYPE_USER		//6
};

struct SELog {
	logType		type;
	std::string msg;
};

class SELogManager {
public:
	// flags
	char settings;

	// dtor
	~SELogManager();

	// init
	static SELogManager& getObj();
	static void release();

	// a dummy init function
	void config(char bits = SE_LOG_ENABLED);

	// User method for log manipulations.
	void assert(bool exp, const char* msg = "User assertion failed.");

	// Kernel method for log manipulations.
	void append(logType t, const char *msg);

	// Output error message(s) to console.
	void print(std::ostream *os = &std::cout);


private:
	// Private constructor for singleton pattern.
	SELogManager();
	
	// log queue
	std::vector<SELog> logData;
	unsigned logPos;

	// SEError Object
	static SELogManager* logManager;

	// sfml streambuff
	std::stringstream sfmlErr;
};

}

#else // SE_DEBUG

#define SE_Assert(...)	(__VA_ARGS__)

#endif // SE_DEBUG

#endif // SELOG_H