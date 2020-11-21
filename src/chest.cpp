#include "chest.h"

Chest::Chest(const Texture& texture, int x, int y) : Renderable(texture) {
	x_ = x;
	y_ = y;
	RectPos(x_, y_);
	SetImageSpeed(0);
}

void Chest::GetPos(int& x, int& y) {
	x = x_;
	y = y_;
}

void Chest::OpenChest() {
	if (!opened_) {
		opened_ = true;
		SetFrame(1);
	}
}