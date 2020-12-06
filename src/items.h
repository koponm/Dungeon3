#pragma once
#ifndef ITEMS_H_
#define ITEMS_H_

#include <memory>

#include "renderable.h"
#include "constants.h"
#include "textureHandler.h"

enum class ItemType {
	health_potion,
	mana_potion,
	staff,
	sword,
	bow,
	random // for randomised items
};

class Item : public Renderable
{
public:
	Item(const Texture& texture, int x, int y, ItemType type, float d);
	~Item() {}

	void GetPos(int& x, int& y);
	ItemType GetItemType() { return type_; }
	void SetStats(const double& m1, const double& m2) { damage_m_ = m1; speed_m_ = m2; }
	void GetStats(double& m1, double& m2) { m1 = damage_m_; m2 = speed_m_; }

private:
	int x_ = 0;
	int y_ = 0;
	ItemType type_;
	double damage_m_ = 1;
	double speed_m_ = 1;
};

namespace item {

std::shared_ptr<Item> GetItem(const int x, const int y, TextureHandler* textures, ItemType weapon, ItemType type = ItemType::random, float d = 1);

} // namespace item

#endif // !