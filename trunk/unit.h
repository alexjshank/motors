#pragma once
#include "entity.h"
#include "md2.h"
#include "particlemanager.h"
#include "ui.h"

class Unit : public Entity
{
public:
	void Construct();
	void Initialize(const char *classname, const char *modelname = 0, const char *skinname = 0);

	Unit(void);
	~Unit(void);

	Unit(const char *classname);
	Unit(const char *classname, const char *modelname, const char *skinname);

	virtual void Think(); 

	void init();
	void process();
	void render();
	void interact(Entity *source, int type);
	void WalkTo(Vector t);

	void onAttacked(Entity *source);
	void onDeath();
	void onSelected();
	void onUnSelected();

	void SetModel(const char*model, const char*tex);
	struct tmodelAnimations {
		float fps;
		int walkStart,walkEnd;
		int runStart,runEnd;
		int attackStart,attackEnd;
		int idleStart,idleEnd;
	} modelAnimations;

	string classname;
	Md2Object *model;	
	int texture;
	bool footprints;
	bool ground_unit;
	float buildtime;
	float updateInterval;	// how often onThink is called
	float lastThinkTime;
	float runspeed;
	float walkspeed;

	int foot;
	float lastRepositionTime;
	float lastFootprintSpawn;
	float frame;

	Vector target;
	Vector targetvelocity;

	Vector calibratedModelPosition;	// these are used to correct models that don't follow the standard positioning stuff
	Vector calibratedModelRotation;
	
	ParticleEmitter blood;

	UIWindow *menu;	
};
