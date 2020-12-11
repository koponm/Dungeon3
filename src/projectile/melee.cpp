
#include "melee.h"
//Melee attack in form of a projectile that doesn't move
Melee::Melee(const Texture& texture, double x, double y, double speed, double dir, std::shared_ptr<Renderable> parent)
	: Projectile(texture, x, y, speed, dir, parent)
{
	damage_ = 6.0;
}

Melee::~Melee() {
}
