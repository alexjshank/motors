#pragma once
#include "building.h"

class Barracks : public Building
{
public:
	Barracks(void);
	~Barracks(void);

	void init();
	void Think();
};
