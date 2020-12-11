#pragma once
#ifndef UPDATE_H_
#define UPDATE_H_

#include "monster.h"
#include "LocationNode.h"
#include "../wall.h"
#include "../renderable.h"

#include <memory>
#include <unordered_map>
#include <list>
#include <vector>
#include <algorithm>
#include <cmath>


namespace monster {

unsigned int calculateHCost(int current, int target, unsigned int w);

bool nodeComp(const LocationNode* lhs, const LocationNode* rhs);

std::list<int> getNeighbours(int current_node, bool* path_tiles, 
	unsigned int w, unsigned int size);

bool getLineOfSight(bool* path_tiles, int current, int target,
	unsigned int w, unsigned int size);

void A_star_algorithm(std::shared_ptr<Monster> monster, bool* path_tiles, double player_x, double player_y,
	unsigned int size, unsigned int room_width);

void CalculatePath(std::vector<std::shared_ptr<Monster>>& monsters, bool* path_tiles, double player_x, double player_y,
	unsigned size, unsigned int room_width);
	
void UpdateMonsters(std::vector<std::shared_ptr<Monster>>& monsters, double delta_speed, const size_t fps, bool can_move,
	unsigned int room_width, std::vector<std::shared_ptr<Wall>> walls, const Texture& tombstone,
	std::list<std::shared_ptr<Renderable>>& to_render);

}; // namespace monster

#endif // UPDATE_H_
