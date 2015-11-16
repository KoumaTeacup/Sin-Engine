#include "SEObject.h"

using namespace se_system;

unsigned SEObject::numOfObjects = 0;

SEObject::SEObject(std::string n, std::string t, int num) :
	name(n),
	tag(t),
	enabled(true),
	id(numOfObjects++) {
#ifdef SE_DEBUG
	if (name == ""){
		if (num != 0) {
			char str[8];
			sprintf(str, "%03d", num);
			name = str;
		}
	}
#endif
}

SEObject::SEObject(const SEObject& rhs) :
	name(rhs.name + "_copy"),
	tag(rhs.tag),
	enabled(rhs.enabled),
	id(numOfObjects++) {
#ifdef SE_DEBUG
	std::string nameStr = rhs.name;
	if (nameStr.length() > 8) {
		for (auto i = nameStr.begin(), j = i + 5; j != nameStr.end(); ++i,++j)
			if (std::string(i, j) == std::string("_copy")) {
				std::string numStr(j, nameStr.end());
				int num = std::stoi(numStr, NULL);
				char cstr[8];
				sprintf(cstr, "%03d", ++num);
				name = std::string(nameStr.begin(), j) + cstr;
				return;
			}
	}
	name = nameStr + "_copy000";
#endif
}

SEObject& SEObject::operator=(const SEObject &rhs) {
	tag = rhs.tag;
	enabled = rhs.enabled;
	return *this;
}