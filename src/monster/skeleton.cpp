
#include "skeleton.h"


Skeleton::Skeleton(const Texture& texture, double x, double y)
	: Monster(texture, x, y)
{
	speed_ = 700;
	attack_ = 100;
	health_ = 100;
	is_melee = false;
}

Skeleton::~Skeleton() {
}
 