#include "terrain.h"
#include "entity.h"
#include "md2.h"

#ifndef _EVI_OBJ_H
#define _EVI_OBJ_H

extern Terrain *terrain;

class EnvironmentObject : public Entity
{
public:
	EnvironmentObject(void);
	~EnvironmentObject(void);

	void render();
	void SetModel(const char*model, const char*tex);
	virtual void Think() {}

	Md2Object *model;	
	int texture;
};

class LampPost : public EnvironmentObject 
{
public:
	LampPost() {
		type = E_LAMPPOST;
		scale = 0.05f;
		size=Vector(1,1,1);
	}

	void init() {
		SetModel("data/models/lightpost.md2","data/models/VillageStuff.bmp");
		position = Vector(position.x,terrain->getInterpolatedHeight(position.x,position.z),position.z);
	}

};

class Dock : public EnvironmentObject 
{
public:
	Dock() {
		type = E_DOCK;
		scale = 0.5f;
		size=Vector(1,1,1);
	}

	void init() {
		SetModel("data/models/dock.md2","data/models/wood.bmp");
		position = Vector(position.x,terrain->getInterpolatedHeight(position.x,position.z),position.z);
	}

};



#endif