#pragma once
#ifndef INITIALIZE_MONSTER_
#define INITIALIZE_MONSTER_

#include "../renderable.h"
#include "../textureHandler.h"
#include "../constants.h"
#include "monster.h"
#include "zombie.h"
#include "ghost.h"
#include "skeleton.h"
#include "skeletonMage.h"
#include "necromancer.h"
#include <stdlib.h>
#include <time.h>

#include <vector>
#include <memory>

namespace monster {
	
	shared_ptr<Monster> AddMonster(std::vector<std::shared_ptr<Monster>>& monsters, TextureHandler* texture,
		double x, double y, unsigned int difficulty, MonsterType type = MonsterType::MONSTER_COUNT);


} // namespace monster

#endif // INITIALIZE_MONSTER_