
#include "ghost.h"


Ghost::Ghost(const Texture& texture, double x, double y, unsigned int difficulty, MonsterType type)
	: Monster(texture, x, y, type)
{
	speed_ = 750.0 * (1.0 + 0.02 * difficulty);
	health_ = 20.0 * (1.0 + 0.25 * difficulty);
	max_health_ = health_;
	ignore_walls_ = true;
}

Ghost::~Ghost() {
}
