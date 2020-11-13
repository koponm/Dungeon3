#pragma once
#ifndef UPDATE_H_
#define UPDATE_H_

#include "monster.h"
#include "LocationNode.h"
#include "../wall.h"

#include <vector>
#include <algorithm>

namespace update {

	unsigned int calculateHCost(int current, int target, unsigned int w);

	bool nodeComp(const LocationNode* lhs, const LocationNode* rhs);

	void CalculatePath(vector<Monster*> monsters, bool* path_tiles, int player_x, int player_y,
		unsigned size, unsigned int room_width, unsigned int room_height);

	void UpdateMonsters(vector<Monster*>& monsters, double delta_speed, bool can_move,
		unsigned int room_width, unsigned int room_height);

}; // udpate

#endif // UPDATE_H_