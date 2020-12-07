#pragma once
#ifndef BANSHEE_H_
#define BANSHEE_H_

#include <utility>
#include <list>
#include "monster.h"
#include "../renderable.h"

class Banshee : public Monster {
public:
	Banshee(const Texture& texture, double x, double y, unsigned int difficulty, MonsterType type);
	~Banshee();

};

#endif // GHOST_H_