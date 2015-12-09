#include "enemy.h"

#include "SEEvent.h"
#include "SESin.h"
#include "SEScene.h"
#include "ufo.h"
#include "bot.h"

void EnemyFactory::handle(SEEvent & e)
{

}

void EnemyFactory::onUpdate()
{
	if (ufo < MAX_UFO) {
		unsigned id = SIN.getActiveScene()->instantiate(ufoId);
		ufos.push_back(id);
		++ufo;
	}

	if (bot < MAX_BOT) {
		unsigned id = SIN.getActiveScene()->instantiate(botId);
		bots.push_back(id);
		++bot;
	}

	SEGameObject *pObj;
	for (auto i = ufos.begin(); i != ufos.end(); ++i) {
		pObj = SIN.getActiveScene()->getInst(*i);
		UFO *uptr = static_cast<UFO*>((*pObj)[7]);
		if (uptr->hitCount >UFO_LIFE) {
			SIN.getActiveScene()->destroy(*i);
			ufos.erase(i);
			break;
		}
	}

	for (auto i = bots.begin(); i != bots.end(); ++i) {
		pObj = SIN.getActiveScene()->getInst(*i);
		Bot *bptr = static_cast<Bot*>((*pObj)[7]);
		if (bptr->hitCount > BOT_LIFE) {
			SIN.getActiveScene()->destroy(*i);
			bots.erase(i);
			break;
		}
	}

	if (ufos.size() == 0 && bots.size() == 0)
		SIN.broadcast(SEEvent("win"));
}
