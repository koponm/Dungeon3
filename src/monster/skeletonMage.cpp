
#include "skeletonMage.h"


SkeletonMage::SkeletonMage(const Texture& texture, double x, double y)
	: Monster(texture, x, y)
{
	speed_ = 700;
	attack_ = 100;
	health_ = 100;
	is_melee = false;
	projectile = ProjectileType::IceBall;
}

SkeletonMage::~SkeletonMage() {
}
