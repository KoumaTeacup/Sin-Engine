#ifndef SAMPLE_H
#define SAMPLE_H

#include "SEComponent.h"

class SEComSample : public SEComponent {
public:
	SEComSample(SEGameObject *onwer);
	SEComSample(const SEComSample &rhs);
	~SEComSample();

	SEComSample& operator =(const SEComSample &rhs);

	// Local methods.

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