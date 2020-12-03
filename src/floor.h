#pragma once
#include "renderable.h"
#include <stdlib.h>
#include <time.h>

class Floor : public Renderable {
public:
	Floor(const Texture& texture, int x, int y);
	~Floor();
	void GetPos(int& x, int& y);
private:
	int x_ = 0;
	int y_ = 0;
};