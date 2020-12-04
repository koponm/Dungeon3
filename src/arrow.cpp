#pragma once
#include "arrow.h"

Arrow::Arrow(const Texture& texture, double x, double y, double speed, double dir, std::shared_ptr<Renderable> parent)
	: Projectile(texture, x, y, speed, dir, parent)
{
	x_vel_ = speed_ * cos(dir * M_PI / 180);
	y_vel_ = -speed_ * sin(dir * M_PI / 180);
	damage_ = 4.0;
}

Arrow::~Arrow() {
}