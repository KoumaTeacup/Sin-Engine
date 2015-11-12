#ifndef SEOBJECT_H
#define SEOBJECT_H

#include <string>

namespace se_system {

class SEObject {
public:
	SEObject(std::string n = std::string(), std::string t = std::string(), int num = 0);
	SEObject(const SEObject& rhs);

	virtual ~SEObject() {}

	virtual SEObject& operator=(const SEObject &rhs);

	// Setter & Getter
	std::string getName() const				{ return name; }
	bool		isTag(std::string t) const	{ return tag == t; }
	void		enTag(std::string t)		{ tag = t; }
	void		unTag()						{ tag = std::string(); }
	bool		isEnabled() const			{ return enabled; }
	void		enable()					{ enabled = true; }
	void		disable()					{ enabled = false; }

	virtual const char* toString() const = 0;

	virtual void onInit() = 0;
	virtual void onRelease() = 0;

	virtual void onUpdate() {}
	virtual void onDraw() {}
	virtual void onPostUpdate() {}
	virtual void onPause() {}
	virtual void onResume() {}


private:
	std::string name, tag;

	bool enabled;
};

}
#endif