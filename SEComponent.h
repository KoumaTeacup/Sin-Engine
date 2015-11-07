#ifndef SECOMPONENT_H
#define SECOMPONENT_H

class SEGameObject;

enum componentType {
	COM_TRANSFORM = 0,
	COM_CONTROLLER,
	COM_COLLIDER,
	COM_RENDERER,
	COM_RIGID,
	COM_ANIMATOR,
	COM_NUM
};

class SEComponent {
public:
	SEComponent(componentType t, SEGameObject* o);
	SEComponent(const SEComponent& rhs);

	virtual const SEComponent& operator=(const SEComponent& rhs) { return *this; }

	virtual ~SEComponent() {};
	virtual void attach(const char* filename) {}

	virtual void onInit() {}
	virtual void onUpdate() {}
	virtual void onDraw() {}
	virtual void onRelease() {}

	componentType getType() { return type; }
private:
	componentType type;
	SEGameObject *owner;
};

#endif