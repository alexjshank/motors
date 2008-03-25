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

	void process();
	void render();
	void interact(Entity *source, int type);
	void WalkTo(Vector t);

	void GetScriptRegisters();
	void SetScriptRegisters();
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

	struct tScripts {
		std::string onSpawn, checkBuildRequirements, onStartBuild,onFinishBuild,onThink,onDestroy,onSelected,onUnSelected,onOrderIssued,onCompletedOrder,onEnemyNearby,onAttacked,onKilled;
	} Scripts;
	struct tScriptRegisters {
		int ria,rib,ric,rid,rie,rif,rig,rih;
		float rfa,rfb,rfc,rfd,rfe,rff,rfg,rfh;
	} ScriptRegisters;
};
