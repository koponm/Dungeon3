#include "ladder.h"

Ladder::Ladder(const Texture& texture, int x, int y) : Renderable(texture) {
	x_ = x;
	y_ = y;
	RectPos(x_, y_);
}

Ladder::~Ladder() {
	
}

void Ladder::GetPos(int& x, int& y) {
	x = x_;
	y = y_;
}