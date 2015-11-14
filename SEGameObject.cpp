#include "SEGameObject.h"
#include "SEComTemp.h"

int SEGameObject::num = 0;

SEGameObject::SEGameObject(std::string name, std::string tag) :
	SEObject::SEObject(name, tag, ++num),
	components(COM_NUM, NULL) {}

SEGameObject::SEGameObject(const SEGameObject& rhs) : 
	SEObject(rhs),
	components(rhs.components) {
	for (auto i = components.begin(); i != components.end(); ++i) {
		if (*i) *i = (*i)->clone();
	}
	resetOwner();
}

SEGameObject& SEGameObject::operator=(const SEGameObject &rhs) {
	SEObject::operator=(rhs);
	for (auto i : components)
		if (i) delete i;
	components = rhs.components;
	for (auto i = components.begin(); i != components.end(); ++i) {
		if (*i) *i = (*i)->clone();
	}
	resetOwner();
	return *this;
}

SEGameObject::~SEGameObject() {
	for (auto i : components)
		if (i) delete i;
}

/*****************************************************************************************************/
/*!
	\brief Access components attached to this game object.
	\param type component type
	\return the reference to the component pointer

	Notes: 
	-This access will not check the existence of any components, which is good for the user that
	actually want to check it outside.
	-Reference makes sure the point can be modified. Thus user should use extreme caution when trying
	to assign new value to the returned pointer, in that these pointers are not reference counted.
	-User should always prefer access methods provided by SESIN object which makes sure the pointer
	is constant.
	-The engine assume the user defined components will always exist.
	-Those who doesn't fall into above assumption, that is, If the user tries to access components
	through the componet ID and that ID is actually not valid, a temporary place holder component
	with COM_UNDEFINED type will be	returned. 
*/
/*****************************************************************************************************/
SEComponent*& SEGameObject::operator[](componentType type) {
	return components[type];
}

SEComponent*& SEGameObject::operator[](unsigned index) {
	if (index > components.size() - 1) {
		SEComponent *undefined = &SEComTemp();
		return undefined;
	}
	else return components[index];
}

int SEGameObject::attach(componentType type, std::string name, std::string tag) {
	// New Component TO DOs
	if (type >= COM_NUM) {
#ifdef SE_DEBUG
		char log[256];
		const char *name = toString();
		sprintf(log, "%s> Attaching default Listener/User component (ignored).", name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
		free((void*)name);
#endif
		return -1;
	}
	if (components[type]) {
#ifdef SE_DEBUG
		char log[256], typeStr[32];
		const char *name = toString();
		switch(type) {
		case COM_TRANSFORM: sprintf(typeStr, "Transform"); break;
		case COM_CAMERA:	sprintf(typeStr, "Camera"); break;
		case COM_RENDERER:	sprintf(typeStr, "Renderer"); break;
		}
		sprintf(log, "%s can't have more than one %s component", name, typeStr);
		free((void*)name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
#endif
		delete components[type];
	}
	SEComponent *pCom = NULL;
	switch (type) {
	case COM_TRANSFORM: pCom = new SEComTransform(name, tag); break;
	case COM_CAMERA:	pCom = new SEComCamera(name, tag); break;
	case COM_RENDERER:	pCom = new SEComRenderer(name ,tag); break;
	}
	components[type] = pCom;

	return components.size() - 1;
}

int SEGameObject::attach(SEComponent *pCom) {
	// New Components TO DOs
	SEComponent *newComp;
	if (!pCom) return -1;
	if (pCom->getType() < COM_NUM) {
#ifdef SE_DEBUG
		char log[256], typeStr[32];
		const char *name = toString();
#endif
		switch (pCom->getType()) {
		case COM_TRANSFORM:
#ifdef SE_DEBUG
			sprintf(typeStr, "Transform");
#endif
			newComp = new SEComTransform(*static_cast<SEComTransform*>(pCom));
			break;
		case COM_CAMERA:
#ifdef SE_DEBUG
			sprintf(typeStr, "Camera");
#endif
			newComp = new SEComCamera(*static_cast<SEComCamera*>(pCom));
			break;
		case COM_RENDERER:
#ifdef SE_DEBUG
			sprintf(typeStr, "Renderer");
#endif
			newComp = new SEComRenderer(*static_cast<SEComRenderer*>(pCom));
			break;
		}
		if (components[pCom->getType()]) {
#ifdef SE_DEBUG
			sprintf(log, "%s can't have more than one %s component", name, typeStr);
			free((void*)name);
			SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
#endif
			delete components[pCom->getType()];
		}

		components[pCom->getType()] = newComp;
	}
	else components.push_back(pCom);
	return components.size() - 1;
}

void SEGameObject::resetOwner() {
	for (auto i : components) {
		if (i) i->setOwner(this);
	}
}

void SEGameObject::onInit() {
	for (auto i : components)
		if(i && i->isEnabled()) i->onInit();
}

void SEGameObject::onRelease() {
	for (auto i : components)
		if (i && i->isEnabled()) i->onRelease();
}

void SEGameObject::onUpdate() {
	for (auto i : components)
		if (i && i->isEnabled()) i->onUpdate();
}

void SEGameObject::onDraw() {
	for (auto i : components)
		if (i && i->isEnabled()) i->onDraw();
}

void SEGameObject::onPostUpdate() {
	for (auto i : components)
		if (i && i->isEnabled()) i->onPostUpdate();
}

void SEGameObject::onPause() {
	for (auto i : components)
		if (i && i->isEnabled()) i->onPause();
}

void SEGameObject::onResume() {
	for (auto i : components)
		if (i && i->isEnabled()) i->onResume();
}

const char* SEGameObject::toString() const {
	char *output = (char*)malloc(sizeof(char) * 64);
	sprintf(output, "Game Object \"%s\"", getName().c_str());
	return output;
}