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
		print(&f);
		f.close();
	}
}

SELogManager& SELogManager::getObj() {
	if (!logManager) logManager = new SELogManager;
	return *logManager;
}

void SELogManager::init(char bits) {
	settings = bits;
	if (bits > 7) append(LOGTYPE_WARNNING, "Invalid LogManager Initialization bits");
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
			*os << logData[logPos].msg << std::endl << std::endl;
			++logPos;
		}
	}
	char log[64];
	sprintf(log, "Current FPS: %d.  \r", static_cast<int>(1.0f / SE_Utility.getFrameTime()));
	//*os << std::endl << "Curr\rent FPS: " << static_cast<int>(1.0f / SE_Utility.getFrameTime())
	//	<< " \rblabla" << std::flush;
	*os << log;
}

void SELogManager::clearConsole() {
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	// Get the number of cells in the current buffer
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	// Fill the entire buffer with spaces
	if (!FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', cellCount, homeCoords, &count)) return;

	// Fill the entire buffer with the current colors and attributes
	if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count)) return;

	// Move the cursor home
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

#endif