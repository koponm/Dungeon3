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


namespace item {

std::shared_ptr<Item> GetItem(const int x, const int y, TextureHandler* textures, ItemType type) {
	if (type == ItemType::random) { //random if ItemType not given
		type = static_cast<ItemType>(rand() % (int)(ItemType::random));
	}
	TextureType t;
	switch (type) { //get index for texture
	case ItemType::health_potion:
		t = TextureType::healthPotion;
		break;
	case ItemType::mana_potion:
		t = TextureType::manaPotion;
		break;
	case ItemType::staff:
		t = TextureType::staff;
		break;
	case ItemType::sword:
		t = TextureType::sword;
		break;
	default:
		return nullptr;
	}
	std::shared_ptr<Item> temp(new Item(textures->Get(t), x, y, type));
	return temp;
}
} // namespace item