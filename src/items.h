#pragma once
#ifndef ITEMS_H_
#define ITEMS_H_

#include "player.h"
#include "renderable.h"

enum class ItemType {
	health_potion,
	mana_potion,
	random, // cut-off for randomised items
	staff,
	sword
};

class Item : public Renderable
{
public:
	Item(const Texture& texture, int x, int y, ItemType type);
	~Item() {}

	void GetPos(int& x, int& y);
	void Pickup(Player* player);
	bool Spawned() { return spawned_; } //despawn if false

private:
	int x_ = 0;
	int y_ = 0;
	bool spawned_ = true;
	ItemType type_;

};

#endif // !