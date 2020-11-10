#include "wall.h"

Wall::Wall(const Texture& texture, int x, int y) : Renderable(texture) {
	x_ = x;
	y_ = y;
	RectPos(x_, y_);
}

Wall::~Wall() {
	
}

void Wall::GetPos(int& x, int& y) {
	x = x_;
	y = y_;
}