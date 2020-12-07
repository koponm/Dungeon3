#include "monster.h"

Monster::Monster(const Texture& texture, double x, double y, MonsterType type) : Renderable(texture) {
	x_ = x;
	y_ = y;
	type_ = type;
	RectPos((int)floor(x_ + .5), (int)floor(y_ + .5));
}

void Monster::GetPos(double& x, double& y) {
	x = x_;
	y = y_;
}
 
void Monster::SetPos(const double& x, const double& y) {
	x_ = x;
	y_ = y;
}

void Monster::AddPos(const double& x, const double& y) {
	x_ += x;
	y_ += y;
}

void Monster::SetVel(bool t, const double& v) {
	if (t) {
		y_vel_ = v;
	}
	else {
		x_vel_ = v;
	}
}

void Monster::AddVel(const double& x, const double& y) {
	x_vel_ += x;
	y_vel_ += y;
}

void Monster::CalcPos(const size_t& fps_desired) {
	x_vel_ -= x_vel_ * fric_ / fps_desired;
	y_vel_ -= y_vel_ * fric_ / fps_desired;

	x_ += x_vel_ / fps_desired;
	y_ += y_vel_ / fps_desired;

	RectPos((int)floor(x_ + .5), (int)floor(y_ + .5));
}

int Monster::GetNextTile() {
	if (!next_moves_.empty()) {
		return next_moves_.front();
	}
	else {
		return -1;
	}
}

void Monster::PopNextMove() {
	if (!next_moves_.empty()) {
		next_moves_.pop_front();
	}
}

void Monster::Kill() {
{
	is_dead = true;
}

}
