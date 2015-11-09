#include "SEGameObject.h"

int SEGameObject::num = 0;

SEGameObject::SEGameObject(std::string name, std::string tag) :
	SEObject::SEObject(name, tag, ++num),
	components(COM_NUM, NULL) {}

SEGameObject::SEGameObject(const SEGameObject& rhs) : 
	SEObject(rhs),
	components(rhs.components){}

SEGameObject& SEGameObject::operator=(const SEGameObject &rhs) {
	SEObject::operator=(rhs);
	for (auto i : components)
		if (i) delete i;
	components = rhs.components;
	return *this;
}

SEGameObject::~SEGameObject() {
	for (auto i : components)
		if (i) delete i;
}

SEComponent*& SEGameObject::operator[](componentType type) {
	return components[type];
}

void SEGameObject::attach(componentType type, std::string name, std::string tag) {
	// New Component TO DOs
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
	case COM_TRANSFORM: pCom = static_cast<SEComponent *>(new SEComTransform(this, name, tag)); break;
	case COM_CAMERA:	pCom = static_cast<SEComponent *>(new SEComCamera(this, name, tag)); break;
	case COM_RENDERER:	pCom = static_cast<SEComponent *>(new SEComRenderer(this, name ,tag)); break;
	}
	components[type] = pCom;
}

void SEGameObject::attach(SEComponent *pCom) {
	if (!pCom) return;
	components[pCom->getType()] = pCom;
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
		if (i) i->onUpdate();
}

void SEGameObject::onDraw() {
	for (auto i : components)
		if (i) i->onDraw();
}

void SEGameObject::onPostUpdate() {
	for (auto i : components)
		if (i) i->onPostUpdate();
}

void SEGameObject::onPause() {
	for (auto i : components)
		if (i) i->onPause();
}

void SEGameObject::onResume() {
	for (auto i : components)
		if (i) i->onResume();
}

const char* SEGameObject::toString() const {
	char *output = (char*)malloc(sizeof(char) * 64);
	sprintf(output, "Game Object \"%s\"", getName().c_str());
	return output;
}