#pragma once
#include "monster.h"
#include "../wall.h"

#include <vector>

void UpdateMonsters(vector<Monster*>& monsters, size_t fps, double speed, vector<Wall*>& walls, bool can_move);