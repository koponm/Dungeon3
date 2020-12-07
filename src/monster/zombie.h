#pragma once
#ifndef ZOMBIE_H_
#define ZOMBIE_H_

#include <utility>
#include <list>
#include "monster.h"
#include "../renderable.h"

class Zombie : public Monster {
public:
	Zombie(const Texture& texture, double x, double y, unsigned int difficulty, MonsterType type);
	~Zombie();

};

#endif // ZOMBIE_H_