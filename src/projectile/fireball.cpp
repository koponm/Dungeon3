
#include "fireball.h"
//Fireball shot by the player to the direction of the mouse
Fireball::Fireball(const Texture& texture, double x, double y, double speed, double dir, std::shared_ptr<Renderable> parent)
	: Projectile(texture, x, y,speed,dir, parent)
{
	x_vel_ = speed_ * cos(dir * M_PI / 180);
	y_vel_ = -speed_* sin(dir* M_PI/180);
	damage_ = 5.0;
}

Fireball::~Fireball() {
}
