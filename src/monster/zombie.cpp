
#include "zombie.h"


Zombie::Zombie(const Texture& texture, double x, double y)
: Monster(texture, x, y)
{
	speed_ = 600;
	attack_ = 100;
	health_ = 100;
}

Zombie::~Zombie() {
}
