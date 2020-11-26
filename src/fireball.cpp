#pragma once
#include "fireball.h"

Fireball::Fireball(const Texture& texture, double x, double y, double speed, double dir)
	: Projectile(texture, x, y,speed,dir)
{
	x_vel_ = speed_ * cos(dir * M_PI / 180);
	y_vel_ = -speed_* sin(dir* M_PI/180);
	damage_ = 5.0;
}

Fireball::~Fireball() {
}