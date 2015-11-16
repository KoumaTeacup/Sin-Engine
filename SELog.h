#ifndef SELOG_H
#define SELOG_H

#define SE_LOG_ENABLED	0x01
#define SE_LOG_USERONLY 0x02
#define SE_LOG_LOCAL	0x04

#ifdef SE_DEBUG

#include <iostream>
#include <sstream>
#include <vector>

#include "SESystem.h"

// User Command
#define SE_LogConfig			se_debug::SELogManager::getObj().config
#define SE_Assert				se_debug::SELogManager::getObj().assert

// Internal Command
#define SE_LogManager			se_debug::SELogManager::getObj()
#define SE_LogManager_Release	se_debug::SELogManager::release()


namespace se_debug {

enum logType {
	LOGTYPE_UNKNOWN,	//0
	LOGTYPE_GENERAL,	//1
	LOGTYPE_WARNNING,	//2
	LOGTYPE_ERROR,		//3
	LOGTYPE_OPENGL,		//4
	LOGTYPE_SFML,		//5
	LOGTYPE_USER,		//6
	LOGTYPE_CONTINUE,	//7
};

struct SELog {
	logType		type;
	std::string msg;
};

class SELogManager:public se_system::SESystem<SELogManager> {
	friend SESystem<SELogManager>;

public:
	// dtor
	~SELogManager();

	// a dummy init function
	void config(char bits = SE_LOG_ENABLED);

	// User method for log manipulations.
	void assert(bool exp, const char* msg = "User assertion failed.");

	// Kernel method for log manipulations.
	void append(logType t, const char *msg);

	// Output error message(s) to console.
	void print(std::ostream *os = &std::cout);


private:
	// flags
	char settings;

	// Private constructor for singleton pattern.
	SELogManager();
	
	// log queue
	std::vector<SELog> logData;
	unsigned logPos;

	// sfml streambuff
	std::stringstream sfmlErr;
};

}

#else // SE_DEBUG

#define SE_Assert(...)	/*__VA_ARGS__*/
#define SE_LogConfig(x)	/*x*/

#endif // SE_DEBUG

#endif // SELOG_H