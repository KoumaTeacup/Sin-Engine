#ifndef SECOMPONENT_H
#define SECOMPONENT_H

class SEGameObject;

enum componentType {
	COM_TRANSFORM,
	COM_CONTROLLER,
	COM_COLLIDER,
	COM_MESH,
	COM_SHADER,
	COM_RIGID,
	COM_ANIMATOR
};

class SEComponent {
public:
	SEComponent(componentType t);

	virtual ~SEComponent() {};
private:
	componentType type;
	SEGameObject *owner;
};

#endif