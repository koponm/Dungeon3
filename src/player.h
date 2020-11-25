#pragma once
#ifndef PLAYER_H_
#define PLAYER_H_
#include <utility>
#include "renderable.h"

using namespace std;

class Player : public Renderable {
public:
	Player(const Texture& texture, double x, double y);
	~Player();
	void GetPos(double& x, double& y);
	void SetPos(const double& x, const double& y);
	void AddPos(const double& x, const double& y);
	void AddVel(const double& x, const double& y);
	void CalcPos(const size_t& fps_desired);
	void SetVel(bool t, const double& v);
	double GetSpeed(void) const;
	double GetHealth() const;
	double GetMana() const;
private:
	double x_ = 0.0;
	double y_ = 0.0;
	double x_vel_ = 0.0;
	double y_vel_ = 0.0;
	double fric_ = 4.0;
	double speed_ = 800.0;
	double health_ = 100;
	double mana_ = 100;
};
#endif