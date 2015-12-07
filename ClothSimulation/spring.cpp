#include "spring.h"

#include "SEComTransform.h"
#include "SESin.h"
#include "SEEvent.h"
#include "SEInput.h"

void Spring::onInit() {
	SEComTransform &comTrans = SE_TRANSFORM;
	SEVector3f position(comTrans[tx], comTrans[ty], comTrans[tz]);
	direction = -position;
	velocity = position* amplitute;
}

void Spring::handle(SEEvent & event)
{
	if (event.type == EVENT_KEYPRESS) {
		switch (event.info.key.code) {
		case SE_KEYBOARD::Num1:
			integrator = Euler;
			break;
		case SE_KEYBOARD::Num2:
			integrator = Euler_Cromer;
			break;
		case SE_KEYBOARD::Num3:
			integrator = Verlet;
			break;
		}
	}
}

void Spring::onUpdate()
{
	// Getting the frametime.
	// --In all integrator evaluations, time step should be fixed
	float ft = SIN.getFrameTime();

	if (ft > 0.05f) return;
	ft /= 10.0f;
	SEComTransform &comTrans = SE_TRANSFORM;
	SEVector3f position(comTrans[tx], comTrans[ty], comTrans[tz]);
	SEVector3f acceleration = direction * (position.lengthSqaure() - 105);

	SEVector3f oldVel;
	SEVector3f temp;
	switch (integrator) {
	case Euler:
		oldVel = velocity;
		velocity += acceleration * ft;
		position += oldVel * ft;
		break;
	case Euler_Cromer:
		velocity += acceleration * ft;
		position += velocity * ft;
		break;
	case Verlet:
		temp = oldPosition;
		oldPosition = position;
		if (temp.lengthSqaure() < 0.01f) {
			velocity += acceleration *ft;
			position += velocity *ft;
		}
		else {
			position += position - temp + acceleration*ft*ft;
		}
		break;
	}

	comTrans[tx] = position[0];
	comTrans[ty] = position[1];
	comTrans[tz] = position[2];
}
