#pragma once
#ifndef MONTER_H_
#define MONSTER_H_
#include <utility>
#include <list>
#include "../renderable.h"
#include "../projectile/projectile.h"

using namespace std;

class Monster : public Renderable {
public:
	Monster(const Texture& texture, double x, double y);
	~Monster() {}
	void GetPos(double& x, double& y);
	void SetPos(const double& x, const double& y);
	void AddPos(const double& x, const double& y);
	void AddVel(const double& x, const double& y);
	void CalcPos(const size_t& fps_desired);
	void SetVel(bool t, const double& v);
	void PlayerMoved() { can_move_ = true; }
	bool CanMove() { return can_move_; }
	int GetNextTile();
	void ChangeNextMoves(list<int> moves) { next_moves_ = moves; }
	void PopNextMove();
	double GetSpeed() { return speed_; }
	double GetAttack() { return attack_; }
	double GetHealth() { return health_; }
	bool IgnoreWalls() { return ignore_walls_; }
	void SetLignOfSight(bool input) { has_lign_of_sight = input; }
	bool HasLignOfSight() { return has_lign_of_sight; }
	bool IsMelee() { return is_melee; }
	bool IsDead() { return health_ <= 0; }
	bool Dead() { return is_dead; }
	void Kill();
	double GetTimer() { return shoot_timer; }
	void SetTimer(double timer) { shoot_timer = timer; }
	void SetHealth(double newHealth) { health_ = newHealth; }
	double GetMaxHealth() { return max_health_; }
	ProjectileType GetProjectile() { return projectile; }
protected:
	double x_ = 0.0;
	double y_ = 0.0;
	double x_vel_ = 0.0;
	double y_vel_ = 0.0;
	double fric_ = 5.0;
	bool can_move_ = false;
	list<int> next_moves_;
	double speed_ = 512.0;
	double attack_ = 1;
	double health_ = 100;
	double max_health_ = health_;
	bool ignore_walls_ = false;
	bool has_lign_of_sight = false;
	bool is_melee = true;
	bool is_dead = false;
	double shoot_timer = 0.0;
	ProjectileType projectile = ProjectileType::Melee;
};
#endif // MONSTER_H_
