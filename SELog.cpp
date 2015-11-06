#ifdef SE_DEBUG

#include <fstream>
#include <Windows.h>
#include <SFML\System.hpp>

#include "SELOG.h"
#include "SEUtility.h"

using namespace se_debug;

SELogManager* SELogManager::logManager = NULL;

SELogManager::SELogManager() :settings(0), logPos(0) {
	sf::err().rdbuf(sfmlErr.rdbuf());
}

SELogManager::~SELogManager() {
	if (settings & SE_LOG_LOCAL) {
		std::ofstream f;
		f.open("SinEngine Log.txt");
		logPos = 0;
		print(&f);
		f.close();
	}
}

SELogManager& SELogManager::getObj() {
	if (!logManager) logManager = new SELogManager;
	return *logManager;
}

void SELogManager::release() {
	if (logManager)
		delete logManager;
}

void SELogManager::config(char bits) {
	settings = bits;
	if (bits > 7) append(LOGTYPE_WARNNING, "Invalid LogManager configuration bits");
	else append(LOGTYPE_GENERAL, "Log Manager initialized.");
	if (settings && !(settings&SE_LOG_ENABLED)) 
		settings |= SE_LOG_ENABLED;
}

void SELogManager::assert(bool exp, const char* msg) {
	if (exp)  append(LOGTYPE_USER, msg);
}

void SELogManager::append(logType t, const char *msg) {
	if (settings & SE_LOG_ENABLED) {
		SELog log = { t, msg };
		logData.push_back(log);
	}
}

void SELogManager::print(std::ostream *os) {
	//check sfml errors.
	if (sfmlErr.rdbuf()->in_avail()) {
		append(LOGTYPE_SFML, sfmlErr.str().c_str());
		sfmlErr.str(std::string());
	}
	if (settings & SE_LOG_ENABLED) {
		while (logPos < logData.size()) {
			if (settings & SE_LOG_USERONLY && logData[logPos].type != LOGTYPE_USER) continue;
			*os << "- Sin Engine -";
			switch (logData[logPos].type) {
			case LOGTYPE_UNKNOWN:
				*os << "[Unknown] ";
				break;
			case LOGTYPE_GENERAL:
				*os << "[General] ";
				break;
			case LOGTYPE_WARNNING:
				*os << "[Warnning] ";
				break;
			case LOGTYPE_ERROR:
				*os << "[Error] ";
				break;
			case LOGTYPE_OPENGL:
				*os << "[OpenGL] ";
				break;
			case LOGTYPE_SFML:
				*os << "[SFML] ";
				break;
			case LOGTYPE_USER:
				*os << "[USER] ";
				break;
			default: break;
			}
			*os << logData[logPos].msg << std::endl;
			++logPos;
		}
	}
	*os << "Current FPS: " << static_cast<int>(1.0f / SE_Utility.getFrameTime()) << "  \r";
}

#endif