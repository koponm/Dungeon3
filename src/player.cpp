#include "player.h"

Player::Player(const Texture& texture, double x, double y) : Renderable(texture) {
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

double Player::GetHealth() const {
	return health_;
}

double Player::GetMana() const {
	return mana_;
}

double Player::GetMaxHealth() const {
	return max_health_;
}

double Player::GetMaxMana() const {
	return max_mana_;
}
int Player::GetHealthPotions() const {
	return health_potions_;
}

int Player::GetManaPotions() const {
	return mana_potions_;
}
void Player::AddHealth(double health) {
	health_ += health;
	if (health_ > max_health_) health_ = max_health_;
}
void Player::AddMana(double mana) {
	mana_ += mana;
	if (mana_ > max_mana_) mana_ = max_mana_;
}
void Player::UseHealthPotion(){
	if (health_potions_ > 0 && health_ < max_health_) {
		AddHealth(5);
		health_potions_--;
	}
}
void Player::UseManaPotion(){
	if (mana_potions_ > 0 && mana_ < max_mana_) {
		AddMana(5);
		mana_potions_--;
	}
}

void Player::AddItem(Item* item) {
	ItemType type = item->GetItemType();
	switch (type) {
	case ItemType::health_potion:
		health_potions_++;
		break;
	case ItemType::mana_potion:
		mana_potions_++;
		break;
	case ItemType::sword:
		weapon_ = type;
		break;
	case ItemType::staff:
		weapon_ = type;
		break;
	}
}

void Player::CalcPos(const size_t& fps_desired) {
	x_vel_ -= x_vel_ * fric_ / fps_desired;
	y_vel_ -= y_vel_ * fric_ / fps_desired;

	x_ += x_vel_ / fps_desired;
	y_ += y_vel_ / fps_desired;

	RectPos((int) floor(x_ + .5), (int) floor(y_ + .5));
}