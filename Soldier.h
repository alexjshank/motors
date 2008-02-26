#pragma once
#include "unit.h"

class Soldier : public Unit
{
public:
	Soldier(void);
	~Soldier(void);

	void init();
	void Think();
	void Attack(Entity *t);
	void onAttacked(Entity *src);

	int soldierState;
	enum SoldierStates {
		HoldingPosition = 0,
		Idle,
		AttackLocation,
		AdvanceTowardEnemy,
		AttackEnemy
	};

	float range;

	float lastLookAroundTime;
	float lastMoveTime;
	float lastAttackTime;
	float lastAttackAnimationTime;
	Entity *enemyTarget;

	Vector OriginalAttackedLocation;

	bool headToMeetingPoint;
	Vector meetingPoint;
};
