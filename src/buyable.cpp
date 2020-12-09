#include "buyable.h"

Buyable::Buyable(const Texture& texture, int x, int y) : Renderable(texture) {
	x_ = x;
	y_ = y;
	RectPos(x_, y_);
}

Buyable::~Buyable() {
	
}

void Buyable::GetPos(int& x, int& y) {
	x = x_;
	y = y_;
}