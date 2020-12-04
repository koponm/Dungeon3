#pragma once
#include "renderable.h"

class Ladder : public Renderable {
public:
	Ladder(const Texture& texture, int x, int y);
	~Ladder();
	void GetPos(int& x, int& y);
private:
	int x_ = 0;
	int y_ = 0;
};