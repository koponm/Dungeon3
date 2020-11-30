#pragma once
#include <utility>
#include <list>
#include "projectile.h"

class Melee : public Projectile {
public:
	Melee(const Texture& texture, double x, double y, double speed, double dir, Renderable* parent);
	~Melee();
};
