#pragma once
#include <utility>
#include <list>
#include <cmath>
#include "projectile.h"

class Fireball : public Projectile {
public:
	Fireball(const Texture& texture, double x, double y, double speed, double dir, std::shared_ptr<Renderable> parent);
	~Fireball();
};
