#pragma once
#ifndef UPDATE_H_
#define UPDATE_H_

#include "monster.h"
#include "LocationNode.h"
#include "../wall.h"

#include <vector>
#include <algorithm>
#include <cmath>


namespace update {

	unsigned int calculateHCost(int current, int target, unsigned int w);

	bool nodeComp(const LocationNode* lhs, const LocationNode* rhs);

	list<LocationNode*> getNeighbours(LocationNode* current_node, bool* path_tiles, 
		unsigned int w, unsigned int size);

	void A_star_algorithm(Monster* monster, bool* path_tiles, int player_x, int player_y,
		unsigned int size, unsigned int room_width, unsigned int room_height);

	void CalculatePath(std::vector<Monster*> monsters, bool* path_tiles, int player_x, int player_y,
		unsigned size, unsigned int room_width, unsigned int room_height);
	
	void UpdateMonsters(vector<Monster*>& monsters, double delta_speed, bool can_move,
		unsigned int room_width, unsigned int room_height);

}; // udpate

#endif // UPDATE_H_