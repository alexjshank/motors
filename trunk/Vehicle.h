#pragma once
#include "entity.h"
#include "md2.h"
#include "vector.h"

class Vehicle :	public Entity {
public:
	Vehicle(void);
	~Vehicle(void);	
	
	void init();
	void process();
	void render();

	Md2Object * getModel() { return model; }

private:
	Md2Object *model;
	int texture;
};
