#pragma once
#ifndef GHOST_H_
#define GHOST_H_

#include <utility>
#include <list>
#include "monster.h"
#include "../renderable.h"

class Ghost : public Monster {
public:
	Ghost(const Texture& texture, double x, double y, unsigned int difficulty);
	~Ghost();

};

#endif // GHOST_H_