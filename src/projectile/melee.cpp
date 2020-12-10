
#include "melee.h"

Melee::Melee(const Texture& texture, double x, double y, double speed, double dir, std::shared_ptr<Renderable> parent)
	: Projectile(texture, x, y, speed, dir, parent)
{
	damage_ = 6.0;
}

Melee::~Melee() {
}
