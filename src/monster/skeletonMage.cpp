
#include "skeletonMage.h"


SkeletonMage::SkeletonMage(const Texture& texture, double x, double y, unsigned int difficulty, MonsterType type)
	: Monster(texture, x, y, type)
{
	speed_ = 700.0 * (1.0 + 0.02 * difficulty);
	health_ = 25.0 * (1.0 + 0.25 * difficulty);
	max_health_ = health_;
	is_melee = false;
	projectile = ProjectileType::IceBall;
}

SkeletonMage::~SkeletonMage() {
}
