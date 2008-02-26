#pragma once
#include "unit.h"


class Ship : public Unit 
{
public:
	Ship();
	~Ship();

	void onSelected();
	void onUnSelected();
	void init();
	void process();
	void Think();

	Entity *waypointHistory[4];
};