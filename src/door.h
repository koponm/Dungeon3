#pragma once
#include "renderable.h"
#include <vector>
#include <iostream>
#include "wall.h"

class Door : public Renderable {
public:
	Door(const Texture& texture, int x, int y);
	~Door();
	void GetPos(int& x, int& y);
	void SetSibling(Door* sibling) { sibling_ = sibling; }
	void SetFirst(const int& room) { begin_ = room; }
	Door* GetSibling() const { return sibling_; }
	int GetFirst() const { return begin_; }
	void AddConnected(const int& room) { connected_.push_back(room); }

	void SetTiles(const int& tile0, const int& tile1) { tile0_ = tile0; tile1_ = tile1; }
	void GetTiles(int& tile0, int& tile1) { tile0 = tile0_; tile1 = tile1_; }

	void SetWalls(Wall* wall0, Wall* wall1) { wall0_ = wall0; wall1_ = wall1; }
	void GetWalls(Wall*& wall0, Wall*& wall1) { wall0 = wall0_; wall1 = wall1_; }

	std::vector<int> GetConnected() const { return connected_; }
private:
	int x_ = 0;
	int y_ = 0;
	Door* sibling_ = nullptr;
	int begin_ = -1;
	int tile0_ = -1;
	int tile1_ = -1;
	Wall* wall0_ = nullptr;
	Wall* wall1_ = nullptr;
	std::vector<int> connected_;
};