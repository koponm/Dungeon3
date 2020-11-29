#pragma once
#include "melee.h"

Melee::Melee(const Texture& texture, double x, double y, double speed, double dir, Renderable* parent)
	: Projectile(texture, x, y, speed, dir, parent)
{
	damage_ = 5.0;
}

Melee::~Melee() {
}