#include "enemy.h"

#include "SEEvent.h"
#include "SESin.h"
#include "SEScene.h"

void EnemyFactory::handle(SEEvent & e)
{
	if (strcmp(e.infoString, "UFO_destroy") == 0) {
		SIN.getActiveScene()->destroy(e.numOfObjects);
		--ufo;
	}
	if (strcmp(e.infoString, "BOT_destroy") == 0) {
		SIN.getActiveScene()->destroy(e.numOfObjects);
		--bot;
	}
}

void EnemyFactory::onUpdate()
{
	if (ufo < MAX_UFO) {
		SIN.getActiveScene()->instantiate(ufoId);
		++ufo;
	}

	if (bot < MAX_BOT) {
		SIN.getActiveScene()->instantiate(botId);
		++bot;
	}

}
