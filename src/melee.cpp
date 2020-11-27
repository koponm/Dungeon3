#pragma once
#include "melee.h"

Melee::Melee(const Texture& texture, double x, double y, double speed, double dir)
	: Projectile(texture, x, y, speed, dir)
{
	damage_ = 5.0;
}

Melee::~Melee() {
}