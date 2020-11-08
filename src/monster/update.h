#pragma once
#include "monster.h"
#include "../wall.h"

#include <vector>

void CalculatePath(vector<Monster*> monsters, bool* path_tiles, int player_x, int player_y,
	unsigned size, unsigned int room_width, unsigned int room_height);

void UpdateMonsters(vector<Monster*>& monsters, double delta_speed, bool can_move,
	unsigned int room_width, unsigned int room_height);