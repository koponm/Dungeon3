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

void Item::Pickup() {
	spawned_ = false;
}