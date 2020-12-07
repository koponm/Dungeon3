#pragma once
#ifndef PLAYER_H_
#define PLAYER_H_
#include <utility>
#include <memory>
#include "renderable.h"
#include "items.h"

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
	double GetMaxHealth() const;
	double GetMaxMana() const;
	int GetHealthPotions() const;
	int GetManaPotions() const;
	void AddHealth(double health);
	void AddMana(double mana);
	void UseHealthPotion();
	void UseManaPotion();
	std::shared_ptr<Item> AddItem(std::shared_ptr<Item> item, std::shared_ptr<TextureHandler> textures);
	ItemType GetWeapon() { return weapon_; }
	void SetHealth(double newHealth) {
		health_ = newHealth;
	}
	void SetMana(double newMana) {
		mana_ = newMana;
	}
	double GetXp() const { return xp_; }
	double GetNextXp() const { return next_xp_; }
	int GetLevel() const { return level_; }
	double GetMoney() const { return money_; }
	void AddXp(const double& xp);

private:
	double x_ = 0.0;
	double y_ = 0.0;
	double x_vel_ = 0.0;
	double y_vel_ = 0.0;
	double fric_ = 4.0;
	double speed_ = 800.0;
	double max_health_ = 100;
	double health_ = 100;
	double max_mana_ = 100;
	double mana_ = 100;
	double xp_ = 0;
	double next_xp_ = 100;
	int level_ = 1;
	double money_ = 20;
	int health_potions_ = 0;
	int mana_potions_ = 0;
	ItemType weapon_ = ItemType::sword;
};
#endif