#ifndef SEGAMEOBJECT_H
#define SEGAMEOBJECT_H

#include <vector>

#include "SEObject.h"

#include "SEComponent.h"
#include "SEComRenderer.h"
#include "SEComTransform.h"
#include "SEComCamera.h"

#define SE_COMP_TRANSFORM	(*static_cast<SEComTransform*>(getOwner()[COM_TRANSFORM]))
#define SE_COMP_CAMERA		(*static_cast<SEComCamera*>(getOwner()[COM_CAMERA]))
#define SE_COMP_RENDERER	(*static_cast<SEComRenderer*>(getOwner()[COM_RENDERER]))

class SEGameObject : public SEObject{
public:
	SEGameObject(std::string name = std::string(), std::string tag = std::string());
	SEGameObject(const SEGameObject& rhs);

	virtual ~SEGameObject();

	virtual SEGameObject& operator=(const SEGameObject &rhs);

	SEComponent*& operator[](componentType type);

	void attach(componentType type, 
		std::string name = std::string(), 
		std::string tag = std::string());
	void attach(SEComponent *com);

	virtual const char* toString() const;

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