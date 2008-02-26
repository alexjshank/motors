#pragma once
#include "unit.h"
#include "peasant.h"

class Vulture :
	public Unit
{
public:
	Vulture();
	~Vulture();

	void init();
	void Think();

	float lastEatTime;
	Sheep *food;
};
