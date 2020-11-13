#pragma once
#ifndef LOCATION_H_
#define LOCATION_H_

#include <limits.h>

struct LocationNode
{
	LocationNode(int _location, int _H) : location(_location), H(_H) {}

	~LocationNode() {}

	unsigned int F = INT_MAX;
	unsigned int G = INT_MAX;
	unsigned int H; // smallest distance to end, doesn't change
	int location;
	LocationNode* parent = nullptr;
};

#endif // LOCATION_H_