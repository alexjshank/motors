#pragma once
#include "building.h"
#include "vector.h"

class Tower : public Building
{
public:
	Tower(void);
	~Tower(void);

	void init();
	void Think();
};
