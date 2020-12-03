
#include "ghost.h"


Ghost::Ghost(const Texture& texture, double x, double y)
	: Monster(texture, x, y)
{
	speed_ = 800;
	attack_ = 100;
	health_ = 20;
	max_health_ = health_;
	ignore_walls_ = true;
}

Ghost::~Ghost() {
}
