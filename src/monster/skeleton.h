#pragma once
#ifndef SKELETON_H_
#define SKELETON_H_

#include <utility>
#include <list>
#include "monster.h"
#include "../renderable.h"

class Skeleton : public Monster {
public:
	Skeleton(const Texture& texture, double x, double y);
	~Skeleton();

};

#endif // SKELETON_H_
