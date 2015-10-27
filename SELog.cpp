#include <fstream>
#include <Windows.h>
#include <SFML\System.hpp>

#include "SELOG.h"

using namespace se_debug;

SELogManager* SELogManager::logManager = NULL;

SELogManager::SELogManager() :settings(SE_LOG_ENABLED) {
	sf::err().rdbuf(sfmlErr.rdbuf());
	SELog log = { LOGTYPE_GENERAL, "Log Manager initialized." };
	logData.push_back(log);
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
	if (settings && !(settings&SE_LOG_ENABLED)) 
		settings |= SE_LOG_ENABLED;
}

void SELogManager::alert(bool exp, const char* msg) {
	if (exp) {
		SELog log = { LOGTYPE_USER, msg };
		logData.push_back(log);
	}
}

void SELogManager::append(logType t, const char *msg) {
	SELog log = { t, msg };
	logData.push_back(log);
}

void SELogManager::print(std::ostream *os) {
	//check sfml errors.
	if (sfmlErr.rdbuf()->in_avail()) {
		append(LOGTYPE_SFML, sfmlErr.str().c_str());
		sfmlErr.str(std::string());
	}
	if (settings & SE_LOG_ENABLED) {
		for (auto i : logData) {
			clearConsole();
			if (settings & SE_LOG_USERONLY && i.type != LOGTYPE_USER) continue;
			*os << "- Sin Engine -";
			switch (i.type) {
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
			*os << i.msg << std::endl;
		}
	}
}

void SELogManager::clearConsole() {
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', cellCount, homeCoords, &count)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}