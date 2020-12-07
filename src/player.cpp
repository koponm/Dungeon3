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
	AddHealth(20);
	health_potions_--;
}
void Player::UseManaPotion(){
	AddMana(20);
	mana_potions_--;
}

void Player::AddXp(const double& xp) {
	xp_ += xp;
	while (xp_ >= next_xp_) {
		level_++;
		next_xp_ += next_xp_ * 0.5;
		next_xp_ = floor(next_xp_);
		health_ += 20.0;
		mana_ += 10.0;
		max_health_ += 20.0;
		max_mana_ += 10.0;
	}
}

// Returns nullptr when no rendering required, else the item
std::shared_ptr<Item> Player::AddItem(std::shared_ptr<Item> item, TextureHandler* textures) {
	std::shared_ptr<Item> result = nullptr;
	ItemType type = item->GetItemType();
	switch (type) {
	case ItemType::health_potion:
		health_potions_++;
		break;
	case ItemType::mana_potion:
		mana_potions_++;
		break;
	case ItemType::sword:
	case ItemType::staff:
	case ItemType::bow:
		result = item::GetItem((int)x_, (int)y_, textures, weapon_, weapon_);
		weapon_ = type;
	break;
	}
	return result;
}

void Player::CalcPos(const size_t& fps_desired) {
	x_vel_ -= x_vel_ * fric_ / fps_desired;
	y_vel_ -= y_vel_ * fric_ / fps_desired;

	x_ += x_vel_ / fps_desired;
	y_ += y_vel_ / fps_desired;

	RectPos((int) floor(x_ + .5), (int) floor(y_ + .5));
}