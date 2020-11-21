#pragma once
#ifndef CHEST_H_
#define CHEST_H_

#include "renderable.h"

class Chest :
    public Renderable
{
public:
	Chest(const Texture& texture, int x, int y);
	~Chest() {}

	void GetPos(int& x, int& y);
	void OpenChest();
	bool ChestOpen() { return opened_; }

private:
	int x_ = 0;
	int y_ = 0;
	bool opened_ = false;
};

#endif