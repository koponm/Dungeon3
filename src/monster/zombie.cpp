
#include "zombie.h"


Zombie::Zombie(const Texture& texture, double x, double y, unsigned int difficulty, MonsterType type)
: Monster(texture, x, y, type)
{
	speed_ = 600 * (1.0 + 0.02 * difficulty);
	health_ = 30.0 * (1.0 + 0.25 * difficulty);
	max_health_ = health_;
}

Zombie::~Zombie() {
}
