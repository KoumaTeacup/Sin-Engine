#include "SEGameObject.h"

#include "SEComTemp.h"
#include "SEUtility.h"
#include "SELog.h"

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
		SE_Utility.typeToString(typeStr, type);
		sprintf(log, "%s can't have more than one %s component", name, typeStr);
		free((void*)name);
		SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
#endif
		delete components[type];
	}
	components[type] = SE_Utility.typeToPointer(type);

	return components.size() - 1;
}

int SEGameObject::attach(SEComponent *pCom) {
	if (!pCom) return -1;
	if (pCom->getType() < COM_NUM) {
		if (components[pCom->getType()]) {
#ifdef SE_DEBUG
			char log[256], typeStr[32];
			const char *name = toString();
			SE_Utility.typeToString(typeStr, pCom->getType());
			sprintf(log, "%s can't have more than one %s component", name, typeStr);
			free((void*)name);
			SE_LogManager.append(se_debug::LOGTYPE_WARNNING, log);
#endif
			delete components[pCom->getType()];
		}

		components[pCom->getType()] = pCom->clone();
	}
	else components.push_back(pCom->clone());
	return components.size() - 1;
}

void SEGameObject::detach(componentType type) {
	delete components[type]; components[type] = NULL; 
}

void SEGameObject::resetOwner() {
	for (auto i : components) {
		if (i) i->setOwner(this);
	}
}

void SEGameObject::onInit() {
	for (auto i : components)
		if(i) i->onInit();
}

void SEGameObject::onRelease() {
	for (auto i : components)
		if (i) i->onRelease();
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