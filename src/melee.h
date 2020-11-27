#pragma once
#include <utility>
#include <list>
#include "Projectile.h"

class Melee : public Projectile {
public:
	Melee(const Texture& texture, double x, double y, double speed, double dir);
	~Melee();
};
