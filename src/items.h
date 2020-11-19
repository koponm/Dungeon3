#pragma once
#ifndef ITEMS_H_
#define ITEMS_H_

#include "renderable.h"

enum class ItemTypes {
	health_potion,
	mana_potion
};

class Item : public Renderable
{
public:
	Item(const Texture& texture, int x, int y, ItemTypes type);
	~Item() {}

	void GetPos(int& x, int& y);
	void Pickup();
	bool Spawned() { return spawned_; } //despawn if false

private:
	int x_ = 0;
	int y_ = 0;
	bool spawned_ = true;
	ItemTypes type_;

};

#endif // !