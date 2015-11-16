#ifndef SEGAMEOBJECT_H
#define SEGAMEOBJECT_H

#include <vector>

#include "SEObject.h"

class SEComponent;
enum componentType;
class SEScene;

class SEGameObject : public se_system::SEObject{
	friend SEScene;
public:
	SEGameObject(std::string name = std::string(), std::string tag = std::string());
	SEGameObject(const SEGameObject& rhs);

	virtual ~SEGameObject();

	virtual SEGameObject& operator=(const SEGameObject &rhs);

	SEComponent*& operator[](componentType type);
	SEComponent*& operator[](unsigned index);

	int attach(componentType type, 
		std::string name = std::string(), 
		std::string tag = std::string());
	int attach(SEComponent *com);
	void detach(componentType type);

	virtual const char* toString() const;

	// Setters & Getters
	int getCompNum() { return components.size(); }
	void resetOwner();

protected:
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