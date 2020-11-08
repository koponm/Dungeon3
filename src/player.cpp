#include "player.h"

Player::Player(SDL_Texture* texture, SDL_Rect rect, double x, double y) : Renderable(texture, rect) {
	x_ = x;
	y_ = y;

	RectPos((int)floor(x_ + .5), (int)floor(y_ + .5));
}

Player::~Player() {

}

void Player::GetPos(double& x, double& y) {
	x = x_;
	y = y_;
}

void Player::SetPos(const double& x, const double& y) {
	x_ = x;
	y_ = y;
}

void Player::AddPos(const double& x, const double& y) {
	x_ += x;
	y_ += y;
}

void Player::SetVel(bool t, const double& v) {
	if (t) {
		y_vel_ = v;
	} else {
		x_vel_ = v;
	}
}

void Player::AddVel(const double& x, const double& y) {
	x_vel_ += x;
	y_vel_ += y;
}

double Player::GetSpeed(void) const {
	return speed_;
}

void Player::CalcPos(const size_t& fps_desired) {
	x_vel_ -= x_vel_ * fric_ / fps_desired;
	y_vel_ -= y_vel_ * fric_ / fps_desired;

	x_ += x_vel_ / fps_desired;
	y_ += y_vel_ / fps_desired;

	RectPos((int) floor(x_ + .5), (int) floor(y_ + .5));
}