#include "items.h"

Item::Item(const Texture& texture, int x, int y, ItemType type) : Renderable(texture) {
	x_ = x;
	y_ = y;
	type_ = type;
	RectPos(x_, y_);
}
void Item::GetPos(int& x, int& y) {
	x = x_;
	y = y_;
}

void Item::Pickup(Player* player) {
	spawned_ = false;
	switch (type_) {
	case ItemType::health_potion:
		player->AddHealthPotion();
		break;
	case ItemType::mana_potion:
		player->AddManaPotion();
		break;
	}
}