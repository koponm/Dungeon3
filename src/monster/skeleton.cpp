
#include "skeleton.h"


Skeleton::Skeleton(const Texture& texture, double x, double y)
	: Monster(texture, x, y)
{
	speed_ = 700;
	attack_ = 100;
	health_ = 20;
	max_health_ = health_;
	is_melee = false;
	projectile = ProjectileType::Arrow;
}

Skeleton::~Skeleton() {
}
 