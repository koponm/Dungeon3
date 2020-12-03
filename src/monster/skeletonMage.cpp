
#include "skeletonMage.h"


SkeletonMage::SkeletonMage(const Texture& texture, double x, double y)
	: Monster(texture, x, y)
{
	speed_ = 700;
	attack_ = 100;
	health_ = 25;
	max_health_ = health_;
	is_melee = false;
	projectile = ProjectileType::IceBall;
}

SkeletonMage::~SkeletonMage() {
}
