#include "SEObject.h"

SEObject::SEObject(std::string n, std::string t, int num) :
	name(n),
	tag(t),
	enabled(true) {
	if (name == ""){
		if (num != 0) {
			char str[8];
			sprintf(str, "%03d", num);
			name = str;
		}
	}
}

SEObject::SEObject(const SEObject& rhs) :
	name(rhs.name + "_copy"),
	tag(rhs.tag),
	enabled(rhs.enabled) {}

SEObject& SEObject::operator=(const SEObject &rhs) {
	name = rhs.name + "_copy";
	tag = rhs.tag;
	enabled = rhs.enabled;
	return *this;
}