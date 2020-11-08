#pragma once
#ifndef PLAYER_H_
#define PLAYER_H_
#include <utility>
#include "renderable.h"

using namespace std;

class Player : public Renderable {
public:
	Player(SDL_Texture* texture, SDL_Rect rect, double x, double y);
	~Player();
	void GetPos(double& x, double& y);
	void SetPos(const double& x, const double& y);
	void AddPos(const double& x, const double& y);
	void AddVel(const double& x, const double& y);
	void CalcPos(const size_t& fps_desired);
	void SetVel(bool t, const double& v);
	double GetSpeed(void) const;
private:
	double x_ = 0.0;
	double y_ = 0.0;
	double x_vel_ = 0.0;
	double y_vel_ = 0.0;
	double fric_ = 0.1875;
	double speed_ = 2.0;
};
#endif