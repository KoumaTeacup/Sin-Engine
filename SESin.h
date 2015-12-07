#ifndef SESIN_H
#define SESIN_H

#include <SFML\Window.hpp>

#include "SESystem.h"
#include "SEMatrix.h"

#define FLOAT_OFFSET 0.01f
#define MIPMAP_LEVEL 5

namespace se_data { 
	class SEFilePointer; 
	template <unsigned DIM, typename T> class vector; 
}
typedef se_data::SEFilePointer			SE_File;
typedef se_data::vector<2U, unsigned>	SEVector2ui;
typedef sf::Keyboard::Key	SE_KEY;
typedef sf::Joystick::Axis	SE_Axis;

struct SEEvent;
class SEComponent;
class SEComCamera;
class SEComRenderer;
class SEComTransform;
class SEComRigidBody;
class SEComCollider;
class SEComLight;
class SEComConstraint;
class SEScene;

#define SIN			SESin::getObj()
#define SIN_Release	SESin::getObj().cleanUp();SESin::release()

// New Components TO DOs
#define SE_CAMERA		SIN.getCamera(this)
#define SE_TRANSFORM	SIN.getTransform(this)
#define SE_RENDERER		SIN.getRenderer(this)
#define SE_RIGIDBODY	SIN.getRigidBody(this)
#define SE_COLLIDER		SIN.getCollider(this)
#define SE_LIGHT		SIN.getLight(this)
#define SE_CONSTRAINT	SIN.getConstraint(this)
#define SE_COMPONENT(x)	SIN.getComponent(this, x)

#define PI 3.1415926536f

class SESin : public se_system::SESystem<SESin>{
	friend SESystem<SESin>;

public:

	bool init();
	void cleanUp();

	void beginScene(SEScene &scene);
	void pauseScene();
	void resumeScene();
	void endScene();

	// Log methods
	void logConfig(char bits) const;
	void assert(bool exp, const char* msg) const;

	// Utility methods
	void	setFPSLimit(int limit) const;
	float	getFrameTime() const;
	void	backFaceCulling() const;

	// Resource methods
	// SE_File load(const char* filename) const;

	// Event methods
	void broadcast(SEEvent e) const;
	SEVector2ui getMousePosition() const;

	// Input methods
	bool isKeyPressed(SE_KEY key);
	bool isKeyPressed(int index, int button);
	float getJoystickPos(int index, SE_Axis axis);

	// New Components TO DOs
	// Component Access
	// Debug mode does one more step of check
	SEComCamera		&getCamera(const SEComponent *comp) const;
	SEComRenderer	&getRenderer(const SEComponent *comp) const;
	SEComTransform	&getTransform(const SEComponent *comp) const;
	SEComRigidBody	&getRigidBody(const SEComponent *comp) const;
	SEComCollider	&getCollider(const SEComponent *comp) const;
	SEComLight		&getLight(const SEComponent *comp) const;
	SEComConstraint	&getConstraint(const SEComponent *comp) const;
	SEComponent		&getComponent(const SEComponent *comp, int id) const;

	// Setters & Getters
	void			setActiveCamera(SEComponent* pCam);
	SEVector2ui		getWindowSize() const;
	SEComCamera*	getActiveCamera() const { return activeCamera; }
	SEScene*		getActiveScene() const { return activeScene; }
	unsigned		getActiveLightsNum() const;

	const std::vector<SEMatrix3f> &getActiveLightsInfo() const;

	// Functional Methods
	float toRad(float degree) const { return degree / 180.0f*PI; }
	float toDegree(float rad) const { return rad * 180.0f / PI; }

private:
	SESin();

	SEVector2i windowSize;

	bool activeSceneState;

	sf::ContextSettings glSettings;
	sf::Window window;
	SEScene *activeScene;
	SEComCamera *activeCamera;
};
#endif