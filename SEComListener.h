#ifndef LISTENER_H
#define LISTENER_H

#include "SEComponent.h"

struct SEEvent;

class SEComListener : public SEComponent {
public:
	SEComListener(std::string name = std::string(),
		std::string tag = std::string(),
		SEGameObject *owner = NULL);
	SEComListener(const SEComListener &rhs);
	~SEComListener() {}

	virtual SEComListener& operator =(const SEComListener &rhs);

	// Local methods.
	virtual void handle(SEEvent &event) = 0;

protected:
	// Inherited pure virtuals.
	virtual void onInit() {}
	virtual void onRelease() {}

	// Inherited virtuals.
	virtual void onUpdate() {}
	virtual void onDraw() {}
	virtual void onPostUpdate() {}
	virtual void onPause() {}
	virtual void onResume() {}

private:

};

#endif