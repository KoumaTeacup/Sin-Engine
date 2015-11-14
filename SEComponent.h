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
	COM_USER,
	COM_LISTENER,
	COM_CONTROLLER,

	// This is the temporary type
	// Any operation performed on it will not affect the game logic
	COM_UNDEFINED
};

class SEComponent : public se_system::SEObject {
public:

	// Internal Constructor
	SEComponent(componentType t,
	std::string name = std::string(),
		std::string tag = std::string(),
		SEGameObject* o = NULL);

	SEComponent(const SEComponent& rhs);

	virtual ~SEComponent() {};

	virtual SEComponent& operator=(const SEComponent& rhs);

	virtual void attach(const char* filename) {}

	// Setters & Getters
	componentType	getType() const				{ return type; }
	SEGameObject&	getOwner() const			{ return *owner; }
	void			setOwner(SEGameObject *o)	{ owner = o; }

	const virtual char* toString() const;

	// Derived class must implement this clone function to return a copy of itself.
	virtual SEComponent *clone() const = 0;

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
	componentType type;
	SEGameObject *owner;

	static int num; // used for naming
};

#endif