#pragma once
#include "renderable.h"

class Buyable : public Renderable {
public:
	Buyable(const Texture& texture, int x, int y);
	~Buyable();
	void GetPos(int& x, int& y);
	void SetType(const int& type) { type_ = type; }
	int GetType() const { return type_; }
private:
	int x_ = 0;
	int y_ = 0;
	int type_ = -1;
};