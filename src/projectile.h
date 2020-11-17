#pragma once
#include <utility>
#include <list>
#include "renderable.h"


class Projectile : public Renderable {
public:
	Projectile(const Texture& texture, double x, double y, double speed,double dir);
	~Projectile();
	void GetPos(double& x, double& y);
	void SetPos(const double& x, const double& y);
	void AddPos(const double& x, const double& y);
	void AddVel(const double& x, const double& y);
	void SetVel(bool t, const double& v);
	void CalcPos(const size_t& fps_desired);
	double GetSpeed() { return speed_; }
private:
	double x_ = 0.0;
	double y_ = 0.0;
	double x_vel_ = 0.0;
	double y_vel_ = 0.0;
	double speed_ = 0.0;
};