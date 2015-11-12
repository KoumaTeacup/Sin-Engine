#ifndef SECOMPONENT_H
#define SECOMPONENT_H

#include "SEObject.h"

#include "SEResource.h"

class SEGameObject;

// New Component TO DOs
// ATTENTION: component order matters!
// Transform < Renderer : worldspace matrix construction.
enum componentType {
	COM_TRANSFORM = 0,
	COM_CAMERA,
	//COM_CONTROLLER,
	//COM_COLLIDER,
	COM_RENDERER,
	//COM_RIGID,
	//COM_ANIMATOR,
	COM_NUM,
	
	// User derived components
	COM_LISTENER
};

class SEComponent : public se_system::SEObject {
public:
	SEComponent(componentType t, 
		SEGameObject* o, 
		std::string name = std::string(), 
		std::string tag = std::string());

	SEComponent(const SEComponent& rhs);

	virtual ~SEComponent() {};

	virtual SEComponent& operator=(const SEComponent& rhs);

	virtual void attach(const char* filename) {}

	componentType getType() const	{ return type; }
	SEGameObject& getOwner() const	{ return *owner; }

	const virtual char* toString() const;
private:
	componentType type;
	SEGameObject *owner;

	static int num; // used for naming
};

#endif