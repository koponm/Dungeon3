#pragma once
#ifndef ITEMS_H_
#define ITEMS_H_

#include "renderable.h"

enum class ItemType {
	health_potion,
	mana_potion,
	staff,
	sword,
	random // cut-off for randomised items
};

class Item : public Renderable
{
public:
	Item(const Texture& texture, int x, int y, ItemType type);
	~Item() {}

	void GetPos(int& x, int& y);
	void Pickup();
	bool Spawned() { return spawned_; } //despawn if false
	ItemType GetItemType() { return type_; }

private:
	int x_ = 0;
	int y_ = 0;
	bool spawned_ = true;
	ItemType type_;

};

#endif // !