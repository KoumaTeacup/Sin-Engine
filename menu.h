#ifndef MEEENU_H
#define MEEENU_H

#include "SEComListener.h"

class Menu : public SEComListener {
public:

	Menu(unsigned p) :planeObj(p) {}
	~Menu() {};

	void handle(SEEvent &e);

	SEComponent *clone() const { return new Menu(*this); }

private:
	unsigned planeObj;
};

#endif