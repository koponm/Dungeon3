#pragma once
#ifndef NECROMANCER_H_
#define NECROMANCER_H_


#include <utility>
#include <list>
#include "monster.h"
#include "../renderable.h"

class Necromancer : public Monster {
public:
	Necromancer(const Texture& texture, double x, double y, unsigned int difficulty, MonsterType type);
	~Necromancer();

};

#endif // NECROMANCER_H_
