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

#include <vector>
#include <memory>

enum class MonsterType {
	zombie,
	ghost,
	skeleton_archer,
	skeleton_mage,
	MONSTER_COUNT
};

namespace monster {
	
	void AddMonster(std::vector<std::shared_ptr<Monster>>& monsters, TextureHandler* texture,
		double x, double y, unsigned int difficulty, MonsterType type = MonsterType::MONSTER_COUNT);


} // namespace monster

#endif // INITIALIZE_MONSTER_