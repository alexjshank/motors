#pragma once
#include "entity.h"
#include "md2.h"
#include "particlemanager.h"

class Unit : public Entity
{
public:
	Unit(void);
	~Unit(void);

	virtual void Think(); 

	void SetScript(const char *szScript);
	bool LoadScript(const char *szFilename);

// entity calls
	void process();
	void render();
	void interact(Entity *source, int type);
	void WalkTo(Vector t);

	void SetModel(const char*model, const char*tex);
	Md2Object *model;	
	int texture;
	struct tmodelAnimations {
		float fps;
		int walkStart,walkEnd;
		int runStart,runEnd;
		int attackStart,attackEnd;
		int idleStart,idleEnd;

	} modelAnimations;

	ParticleEmitter blood;
	bool footprints;
	float lastFootprintSpawn;
	int foot;
	bool ground_unit;

	Vector target;
	float runspeed;
	float walkspeed;

	float buildtime;

	float frame;

	float lastRepositionTime;
	Vector targetvelocity;

	Vector calibratedModelPosition;	// these are used to correct models that don't follow the standard positioning stuff
	Vector calibratedModelRotation;

	std::string script;
};
