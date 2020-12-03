#include "door.h"

Door::Door(const Texture& texture, int x, int y) : Renderable(texture) {
	x_ = x;
	y_ = y;
	RectPos(x_, y_);
}

Door::~Door() {
	
}

void Door::GetPos(int& x, int& y) {
	x = x_;
	y = y_;
}