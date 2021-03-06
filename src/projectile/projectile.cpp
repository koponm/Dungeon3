#include "projectile.h"
#include <iostream>
//Projectile mother class
Projectile::Projectile(const Texture& texture, double x, double y, double speed,double dir, std::shared_ptr<Renderable> parent) : Renderable(texture) {
	x_ = x;
	y_ = y;
	speed_ = speed;
	RectPos((int)floor(x_ + .5), (int)floor(y_ + .5));
	parent_ = parent;
}

Projectile::~Projectile() {
	
}
//Copies current position
void Projectile::GetPos(double& x, double& y) {
	x = x_;
	y = y_;
}
//Sets a new position
void Projectile::SetPos(const double& x, const double& y) {
	x_ = x;
	y_ = y;
}

void Projectile::AddPos(const double& x, const double& y) {
	x_ += x;
	y_ += y;
}
//Sets velocity
void Projectile::SetVel(bool t, const double& v) {
	if (t) {
		y_vel_ = v;
	}
	else {
		x_vel_ = v;
	}
}

void Projectile::AddVel(const double& x, const double& y) {
	x_vel_ += x;
	y_vel_ += y;
}
//Calculates new position based on velocity and position
void Projectile::CalcPos(const size_t& fps_desired) {

	x_ += x_vel_ / fps_desired;
	y_ += y_vel_ / fps_desired;

	RectPos((int)floor(x_ + .5), (int)floor(y_ + .5));
}