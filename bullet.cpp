#include "bullet.h"

#include "SESin.h"
#include "SEComTransform.h"
#include "SEEvent.h"
#include "SEGameObject.h"

Bullet::Bullet()
{
}

void Bullet::handle(SEEvent & e)
{
}

void Bullet::onInit()
{
}

void Bullet::onUpdate()
{
	SE_TRANSFORM.translation() += direction * BULLET_SPEED * SIN.getFrameTime();
}
