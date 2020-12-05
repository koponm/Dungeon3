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
	Item(const Texture& texture, int x, int y, ItemType type);
	~Item() {}

	void GetPos(int& x, int& y);
	ItemType GetItemType() { return type_; }

private:
	int x_ = 0;
	int y_ = 0;
	ItemType type_;

};

namespace item {

std::shared_ptr<Item> GetItem(const int x, const int y, TextureHandler* textures, ItemType weapon, ItemType type = ItemType::random);

} // namespace item

#endif // !