#ifndef ENEMY_H
#define ENEMY_H

#define MAX_UFO 4
#define MAX_BOT 4

#define UFO_LIFE 5
#define BOT_LIFE 5

#include <vector>

#include "SEComListener.h"


class EnemyFactory : public SEComListener {
public:
	EnemyFactory(unsigned u, unsigned b):ufoId(u), botId(b), ufo(0), bot(0) {};
	~EnemyFactory() {};

	void handle(SEEvent &e);

	SEComponent *clone() const { return new EnemyFactory(*this); }

protected:
	void onUpdate();

private:
	unsigned ufo, bot, ufoId, botId;
	std::vector<unsigned> ufos, bots;
};

#endif