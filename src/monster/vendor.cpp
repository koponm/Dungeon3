
#include "vendor.h"


Vendor::Vendor(const Texture& texture, double x, double y, unsigned int difficulty, MonsterType type)
: Monster(texture, x, y, type)
{
	speed_ = 0;
	health_ = 9007199254740990;
	max_health_ = health_;
}

Vendor::~Vendor() {
}
