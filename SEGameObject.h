#ifndef SEGAMEOBJECT_H
#define SEGAMEOBJECT_H

#include <vector>

#include "SEObject.h"

#include "SEComponent.h"
#include "SEComRenderer.h"
#include "SEComTransform.h"
#include "SEComCamera.h"
#include "SEComListener.h"

// New Components TO DOs
#define SE_COMP_TRANSFORM	(*static_cast<SEComTransform*>(getOwner()[COM_TRANSFORM]))
#define SE_COMP_CAMERA		(*static_cast<SEComCamera*>(getOwner()[COM_CAMERA]))
#define SE_COMP_RENDERER	(*static_cast<SEComRenderer*>(getOwner()[COM_RENDERER]))
#define SE_COMP(x)			(*static_cast<SEComRenderer*>(getOwner()[x]))

class SEGameObject : public se_system::SEObject{
public:
	SEGameObject(std::string name = std::string(), std::string tag = std::string());
	SEGameObject(const SEGameObject& rhs);

	virtual ~SEGameObject();

	virtual SEGameObject& operator=(const SEGameObject &rhs);

	SEComponent*& operator[](componentType type);
	SEComponent*& operator[](int index);

	int attach(componentType type, 
		std::string name = std::string(), 
		std::string tag = std::string());
	int attach(SEComponent *com);

	virtual const char* toString() const;

	// Setters & Getters
	int getCompNum() { return components.size(); }

	// Inherited virtual methods.
	virtual void onInit();
	virtual void onRelease();
	virtual void onUpdate();
	virtual void onDraw();
	virtual void onPostUpdate();
	virtual void onPause();
	virtual void onResume();

private:
	std::vector<SEComponent*> components;

	static int num; // used for naming
};

#endif 