#include <GL\glew.h>
#include <string>

#include "SEUtility.h"

#include "SELog.h"
#include "SEComCamera.h"
#include "SEComTransform.h"
#include "SEComRenderer.h"
#include "SEComRigidBody.h"
#include "SEComCollider.h"
#include "SEComLight.h"
#include "SEComConstraint.h"

using namespace se_system;

SEUtility::SEUtility() :fpsLimit(0), frameTime(1.0f) {
#ifdef SE_DEBUG
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, "utilityManager Initialized.");
#endif
}

void SEUtility::update() {
#ifdef SE_DEBUG
	SE_LogManager.print();
#endif
	frameTime = clock.restart().asSeconds();
	if (fpsLimit > 0) {
		float spf = 1.0f / fpsLimit;
		while (frameTime < spf)
			frameTime += clock.restart().asSeconds();
	}

	// Clear framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SEUtility::typeToString(char *str, componentType type) const {
	// NEW COMPONENT TO DOs
	switch (type) {
	case COM_TRANSFORM: sprintf(str, "Transform"); break;
	case COM_CAMERA:	sprintf(str, "Camera"); break;
	case COM_RENDERER:	sprintf(str, "Renderer"); break;
	case COM_RIGIDBODY:	sprintf(str, "RigidBody"); break;
	case COM_COLLIDER:	sprintf(str, "Collider"); break;
	case COM_LIGHT:		sprintf(str, "Light"); break;
	case COM_CONSTRAINT:sprintf(str, "Constraint"); break;
	}
}

SEComponent *SEUtility::typeToPointer(componentType type, std::string name, std::string tag) const {
	switch (type) {
	case COM_TRANSFORM: return new SEComTransform(name, tag);
	case COM_CAMERA:	return new SEComCamera(name, tag);
	case COM_RENDERER:	return new SEComRenderer(name, tag);
	case COM_RIGIDBODY:	return new SEComRigidBody(name, tag);
	case COM_COLLIDER:	return new SEComCollider(name, tag);
	case COM_LIGHT:		return new SEComLight(name, tag);
	case COM_CONSTRAINT:return new SEComConstraint(name, tag);
	}
	return NULL;
}

void se_system::SEUtility::backFaceCulling() const
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void SEUtility::setFPSLimit(int limit) {
#ifdef SE_DEBUG
	char log[32];
	sprintf(log, "FPS limit set to %d", limit);
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, log);
#endif
	fpsLimit = limit; 
}
