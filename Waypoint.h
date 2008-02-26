#pragma once
#include "entity.h"

class Waypoint : public Entity
{
public:
	Waypoint(void);
	~Waypoint(void);

	void process() { }
	void render();
};
