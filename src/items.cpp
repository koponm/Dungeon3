#include "items.h"

Item::Item(const Texture& texture, int x, int y, ItemType type, float d) : Renderable(texture) {
	x_ = x;
	y_ = y;
	type_ = type;
	damage_m_ = 0.5 * d + sqrt(sqrt(d) * (rand() % 150 + 50.0) / 100.0);
	speed_m_ = 0.8 + (0.4 * (rand() % 100)) / 100.0;
	damage_m_ = floor(damage_m_ * 100.0) / 100.0;
	speed_m_ = floor(speed_m_ * 100.0) / 100.0;

	RectPos(x_, y_);
}
void Item::GetPos(int& x, int& y) {
	x = x_;
	y = y_;
}


namespace item {

std::shared_ptr<Item> GetItem(const int x, const int y, std::shared_ptr<TextureHandler> textures, ItemType type, float d) {
	if (type == ItemType::ITEM_COUNT) { // Randomize if ItemType not given
		type = static_cast<ItemType>(rand() % (int)(ItemType::ITEM_COUNT));
	}
	TextureType t;
	switch (type) {
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
	case ItemType::bow:
		t = TextureType::bow;
		break;
	default:
		return nullptr;
	}
	std::shared_ptr<Item> temp(new Item(textures->Get(t), x, y, type, d));
	return temp;
}
} // namespace item