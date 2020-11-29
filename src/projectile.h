#pragma once
#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include <utility>
#include <list>
#include "renderable.h"

enum class ProjectileType {
	Fireball,
	Melee,
	IceBall
};
class Projectile : public Renderable {
public:
	Projectile(const Texture& texture, double x, double y, double speed,double dir, Renderable* parent);
	~Projectile();
	void GetPos(double& x, double& y);
	void SetPos(const double& x, const double& y);
	void AddPos(const double& x, const double& y);
	void AddVel(const double& x, const double& y);
	void SetVel(bool t, const double& v);
	void CalcPos(const size_t& fps_desired);
	double GetSpeed() { return speed_; };
	Renderable* GetParent() { return parent_; };
	void SetActive(const bool& a) { active_ = a; };
	bool GetActive() const { return active_; };
	double GetDamage() const { return damage_; };

protected:
	double x_ = 0.0;
	double y_ = 0.0;
	double x_vel_ = 0.0;
	double y_vel_ = 0.0;
	double speed_ = 0.0;
	double damage_ = 0.0;
	double timer_ = 0.0;
	bool active_ = true;
	Renderable* parent_ = nullptr;
};

#endif
