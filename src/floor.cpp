#include "floor.h"

Floor::Floor(const Texture& texture, int x, int y) : Renderable(texture) {
	x_ = x;
	y_ = y;
	RectPos(x_, y_);
	SetImageSpeed(0);
	if (rand() % 20 == 0) {
		SetFrame(rand() % int(GetFrames()));
	}
}

Floor::~Floor() {
	
}

void Floor::GetPos(int& x, int& y) {
	x = x_;
	y = y_;
}