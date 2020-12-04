#pragma once
#include "renderable.h"
#include <vector>
#include <iostream>
#include <memory>
#include "wall.h"

class Door : public Renderable {
public:
	Door(const Texture& texture, int x, int y);
	~Door();
	void GetPos(int& x, int& y);
	void SetSibling(std::shared_ptr<Door> sibling) { sibling_ = sibling; }
	void SetFirst(const int& room) { begin_ = room; }
	std::shared_ptr<Door> GetSibling() const { return sibling_; }
	int GetFirst() const { return begin_; }
	void AddConnected(const int& room) { connected_.push_back(room); }

	void SetTiles(const int& tile0, const int& tile1) { tile0_ = tile0; tile1_ = tile1; }
	void GetTiles(int& tile0, int& tile1) { tile0 = tile0_; tile1 = tile1_; }

	void SetWalls(std::shared_ptr<Wall> wall0, std::shared_ptr<Wall> wall1) { wall0_ = wall0; wall1_ = wall1; }
	void GetWalls(std::shared_ptr<Wall>& wall0, std::shared_ptr<Wall>& wall1) { wall0 = wall0_; wall1 = wall1_; }

	std::vector<int> GetConnected() const { return connected_; }
private:
	int x_ = 0;
	int y_ = 0;
	std::shared_ptr<Door> sibling_ = nullptr;
	int begin_ = -1;
	int tile0_ = -1;
	int tile1_ = -1;
	std::shared_ptr<Wall> wall0_ = nullptr;
	std::shared_ptr<Wall> wall1_ = nullptr;
	std::vector<int> connected_;
};