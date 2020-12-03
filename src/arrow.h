#pragma once
#include <utility>
#include <list>
#include "projectile.h"

class Arrow : public Projectile {
public:
	Arrow(const Texture& texture, double x, double y, double speed, double dir, Renderable* parent);
	~Arrow();
};

