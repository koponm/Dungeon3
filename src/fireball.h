#pragma once
#include <utility>
#include <list>
#include "Projectile.h"

class Fireball : public Projectile {
public:
	Fireball(const Texture& texture, double x, double y, double speed, double dir, Renderable* parent);
	~Fireball();
};