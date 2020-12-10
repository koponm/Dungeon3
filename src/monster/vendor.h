#pragma once
#ifndef VENDOR_H_
#define VENDOR_H_

#include <utility>
#include <list>
#include "monster.h"
#include "../renderable.h"

class Vendor : public Monster {
public:
	Vendor(const Texture& texture, double x, double y, unsigned int difficulty, MonsterType type);
	~Vendor();

};

#endif // VENDOR_H_