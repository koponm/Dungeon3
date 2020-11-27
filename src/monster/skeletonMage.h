#pragma once
#ifndef SKELETON_MAGE_H_
#define SKELETON_MAGE_H_


#include <utility>
#include <list>
#include "monster.h"
#include "../renderable.h"

class SkeletonMage : public Monster {
public:
	SkeletonMage(const Texture& texture, double x, double y);
	~SkeletonMage();

};

#endif // SKELETON_MAGE_H_
