#pragma once
#ifndef MONTER_H_
#define MONSTER_H_
#include <utility>
#include <list>
#include "../renderable.h"

using namespace std;

class Monster : public Renderable {
public:
	Monster(const Texture& texture, double x, double y);
	~Monster();
	void GetPos(double& x, double& y);
	void SetPos(const double& x, const double& y);
	void AddPos(const double& x, const double& y);
	void AddVel(const double& x, const double& y);
	void CalcPos(const size_t& fps_desired);
	void SetVel(bool t, const double& v);
	void PlayerMoved() { can_move_ = true; }
	bool CanMove() { return can_move_; }
	int GetNextTile();
	void PopNextTile();
	double GetSpeed() { return speed_;  }
private:
	double x_ = 0.0;
	double y_ = 0.0;
	double x_vel_ = 0.0;
	double y_vel_ = 0.0;
	double fric_ = 0.1875;
	bool can_move_ = false;
	list<int> next_moves_;
	double speed_ = 1.5;
};
#endif // MONSTER_H_
