#ifndef LISTENER_H
#define LISTENER_H

#include "SEComponent.h"

class SEEvent;

class SEComListener : public SEComponent {
public:
	SEComListener(SEGameObject *onwer,
		std::string name = std::string(),
		std::string tag = std::string());
	SEComListener(const SEComListener &rhs);
	~SEComListener();

	SEComListener& operator =(const SEComListener &rhs);

	// Local methods.
	void handle(SEEvent &event);

protected:
	// Inherited pure virtuals.
	void onInit() {}
	void onRelease() {}

	// Inherited virtuals.
	void onUpdate();
	void onDraw();
	void onPostUpdate();
	void onPause();
	void onResume();

private:

};

#endif