#pragma once

#include <utility>
#include <list>
#include "projectile.h"

class IceBall : public Projectile {
public:
	IceBall(const Texture& texture, double x, double y, double speed, double dir, Renderable* parent);
	~IceBall();
};
