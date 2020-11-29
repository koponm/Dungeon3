#pragma once
#include "iceball.h"

IceBall::IceBall(const Texture& texture, double x, double y, double speed, double dir, Renderable* parent)
	: Projectile(texture, x, y, speed, dir, parent)
{
	x_vel_ = speed_ * cos(dir * M_PI / 180);
	y_vel_ = -speed_ * sin(dir * M_PI / 180);
	damage_ = 5.0;
}

IceBall::~IceBall() {
}