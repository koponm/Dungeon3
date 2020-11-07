#pragma once
#ifndef MONTER_H_
#define MONSTER_H_
#include <utility>
#include "../renderable.h"

using namespace std;

class Monster : public Renderable {
public:
	Monster(SDL_Texture* texture, SDL_Rect rect, double x, double y);
	~Monster();
	void GetPos(double& x, double& y);
	void SetPos(const double& x, const double& y);
	void AddPos(const double& x, const double& y);
	void AddVel(const double& x, const double& y);
	void CalcPos(const size_t& fps_desired);
	void SetVel(bool t, const double& v);
	void PlayerMoved() { can_move_ = true; }
	bool CanMove() { return can_move_; }
private:
	double x_ = 0.0;
	double y_ = 0.0;
	double x_vel_ = 0.0;
	double y_vel_ = 0.0;
	double fric_ = 0.1875;
	bool can_move_ = false;
};
#endif // MONSTER_H_
