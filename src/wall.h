#pragma once
#include "renderable.h"

class Wall : public Renderable {
public:
	Wall(const Texture& texture, int x, int y);
	~Wall();
	void GetPos(int& x, int& y);
private:
	int x_ = 0;
	int y_ = 0;
};