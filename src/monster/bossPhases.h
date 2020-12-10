#pragma once

#include "monster.h"
#include "initializeMonster.h"
#include "../renderable.h"
#include "../constants.h"
#include "../textureHandler.h"

#include <memory>
#include <vector>
#include <list>
#include <algorithm>
#include <cmath>

class BossHandler {
public:
	BossHandler(
		std::shared_ptr<Monster> boss,
		std::vector<std::shared_ptr<Monster>>& monsters,
		std::list<std::shared_ptr<Renderable>>& entities,
		std::shared_ptr<TextureHandler> textures,
		bool* path_tiles,
		bool* tiles,
		unsigned int size,
		unsigned int w,
		unsigned int difficulty
	) :
		boss_(boss),
		monsters_(monsters),
		entities_(entities),
		textures_(textures),
		path_tiles_(path_tiles),
		tiles_(tiles),
		size_(size),
		w_(w),
		difficulty_(difficulty)
		{}
	~BossHandler() {}

	std::shared_ptr<Monster> GetBoss() { return boss_; }

	void BossPhase();

	void SpawnMobs(unsigned int amount, MonsterType monstertype, MonsterType monstertype2);

	void RessurectMobs();

private:
	bool stage1 = false;
	bool stage2 = false;
	bool stage3 = false;

	std::shared_ptr<Monster> boss_;
	std::vector<std::shared_ptr<Monster>>& monsters_;
	std::list<std::shared_ptr<Renderable>>& entities_;
	std::shared_ptr<TextureHandler> textures_;

	bool* path_tiles_ = nullptr;
	bool* tiles_ = nullptr;
	unsigned int w_;
	unsigned int size_;
	unsigned int difficulty_;
};



