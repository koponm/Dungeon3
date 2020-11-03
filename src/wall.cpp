#include "wall.h"

Wall::Wall(SDL_Texture* texture, SDL_Rect rect, int x, int y) : Renderable(texture, rect) {
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