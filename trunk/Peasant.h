#include "unit.h"
#include "md2.h"

#ifndef __PEASANT__H__
#define __PEASANT__H__

// gathers wood, meet,
class Peasant : public Unit {
public:
	Peasant();
	void init();
	void Think();

	void onAttacked(Entity *source);

	float startBreakTime;
	float laziness;
	float stress;

	int peasantState;
	enum peasant_states{
		Idle = 0,
		WalkingToLumber,
		ChoppingLumber,
		GatheringLumber,
		RunningAway
	};
		
	float lastChopTime;
	int chops;

	Entity *food;
	Entity *tree;
	Entity *mill;
	Entity *home;
};

// provides meet, fabrics
class Sheep : public Unit {
public:
	Sheep();
	void init();
	void Think();
	void onAttacked(Entity *source);

	float lastThinkTime;
	Entity *farm;
	float born;
};

class VillageWoman : public Unit {
public:
	VillageWoman();
	void init();
	void Think();

	bool deliveringFood;

	enum GatheringSheepCycle {
		NotInCycle=0,
		GoToSheep,
		ReturnToButchers,
		KillSheep,
		BringSheepHome
	};

	int cycleiter;
	float lastThinkTime;
	float lastEatTime;
	Entity *food;
	Entity *farm;
	float born;
};

#endif