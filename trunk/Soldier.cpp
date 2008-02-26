#include ".\soldier.h"
#include "variables.h"
#include "entity.h"
#include "console.h"

extern Console *console;
extern gamevars *vars;
extern EntityContainer *ents;

Soldier::Soldier(void) {
	soldierState = Idle;
	lastLookAroundTime = 0;
	lastAttackTime = 0;
	lastMoveTime = 0;
	enemyTarget = 0;
	
	type=E_SOLDIER;
	family=EF_UNIT;
	size=Vector(0.2,0.75f,0.2);
	runspeed = 4.0f;
	walkspeed = 3.5f;
	scale = 0.02f;

	calibratedModelPosition.y = 1;
	calibratedModelRotation.z = -90;

	modelAnimations.walkStart = 40;
	modelAnimations.walkEnd = 46;
	modelAnimations.runStart = 40;
	modelAnimations.runEnd = 46;
	modelAnimations.fps = 8;

	footprints = true;

	meetingPoint = Vector(0,0,0);
	headToMeetingPoint = false;

	tooltip.enabled = true;
	tooltip.tooltip = "Soldier";
}

Soldier::~Soldier(void)
{
}

void Soldier::init() {
	target = position;
	damage = vars->getIntValue("soldier_damage");
	range = vars->getFloatValue("soldier_attackrange");
	
	SetModel("data/models/soldier.md2","data/models/soldier.bmp");
	runspeed = vars->getFloatValue("soldier_runspeed");
	walkspeed = vars->getFloatValue("soldier_walkspeed");

	console->LoadScript("data/scripts/soldier.py");
}


void Soldier::onAttacked(Entity *src) {
	if (!enemyTarget || dist2(position, src->position) <= dist2(position,enemyTarget->position)) {
		enemyTarget = src;
		soldierState = AttackEnemy;
	}
}

void Soldier::Think() {
	static float lookAroundInterval = vars->getFloatValue("entity_searchinterval");
	switch(soldierState) {
		case HoldingPosition:
			state = Stopped;

			if (timer->time - lastLookAroundTime >= lookAroundInterval) {
				lastLookAroundTime = timer->time;
				Entity *enemy = ents->qtree.tree->getClosestEntity(position,-1,EF_UNIT,true,(this->team == 1)?2:1);
				if (enemy && dist(enemy->position,position) < vars->getFloatValue("soldier_visiblerange")) {
					enemyTarget = enemy;
				}
			}

			if (enemyTarget && enemyTarget->alive) {
				if (timer->time - lastAttackTime >= vars->getFloatValue("soldier_attackinterval")) {
					lastAttackTime = timer->time;
					Attack(enemyTarget);
				}
				static float weaponAttackTime = vars->getFloatValue("soldier_weaponattacktime");
				if (timer->time - lastAttackAnimationTime > weaponAttackTime) {
					lastAttackAnimationTime = timer->time;
					glPushAttrib(GL_ALL_ATTRIB_BITS);
					glDisable(GL_TEXTURE_2D);
					glLineWidth(2);
					glBegin(GL_LINES);
						glColor3f(0.7f,0.7f,0);
						glVertex3f(position.x,position.y+size.y,position.z);
						glColor3f(1,1,0);
						glVertex3f(enemyTarget->position.x+((rand()%40)/10),enemyTarget->position.y+((rand()%20)/10),enemyTarget->position.z+((rand()%40)/10));
					glEnd();
					glPopAttrib();
				}
				break;
			}
		case Idle:
			if (timer->time - lastLookAroundTime >= lookAroundInterval) {
				lastLookAroundTime = timer->time;
				Entity *enemy = ents->qtree.tree->getClosestEntity(position,-1,EF_UNIT,true,(this->team == 1)?2:1);
				if (enemy && dist(enemy->position,position) < vars->getFloatValue("soldier_visiblerange")) {
					taskQueue.clear();
					enemyTarget = enemy;
					soldierState = AdvanceTowardEnemy;
					OriginalAttackedLocation = position;
					break;
				} else {
					if (OriginalAttackedLocation.x != 0 && OriginalAttackedLocation.z != 0) {
						if (dist2(OriginalAttackedLocation,position) <= 3 ||	// if we're there
							!PathTo(OriginalAttackedLocation)) {				// or we cant get there
							OriginalAttackedLocation = Vector(0,0,0);			// stop trying to go there
						}
					}
				}
			}
			if (headToMeetingPoint) {
				soldierState = AttackLocation; 
			}
			break;

		case AttackLocation: {		
			if (timer->time - lastLookAroundTime >= vars->getFloatValue("entity_searchinterval")) {
				lastLookAroundTime = timer->time;
				Entity *enemy = ents->qtree.tree->getClosestEntity(position,-1,EF_UNIT,true,(this->team == 1)?2:1);
				if (enemy && dist(enemy->position,position) < vars->getFloatValue("soldier_visiblerange")) {
					enemyTarget = enemy;
					taskQueue.clear();
					state = Stopped;
					soldierState = AdvanceTowardEnemy;
					break;
				}
			}

			if (state == Stopped) {
				if (dist2(position,meetingPoint) > 5) {			
					PathTo(meetingPoint);
				} else {
					meetingPoint = Vector(0,0,0);
					headToMeetingPoint = false;
					soldierState = Idle;
					lastLookAroundTime = -10;
				}
			}
			break;
		}

		case AdvanceTowardEnemy:	
			if (!enemyTarget || !enemyTarget->alive) {
				if (meetingPoint.len2() != 0) {
					soldierState = AttackLocation;
				} else {
					soldierState = Idle;
				}
				lastLookAroundTime = -10;
				break;
			}

			if (state == Stopped && timer->time - lastLookAroundTime >= vars->getFloatValue("entity_searchinterval")) {
				lastLookAroundTime = timer->time;
				PathTo(enemyTarget->position);
				if (state == WalkToTarget) state = RunToTarget;
			}

			if (dist2(enemyTarget->position,position) < range*range) {
				soldierState = AttackEnemy;
				console->Print("*Radio* - engaging enemy units");
			}

			break;

		case AttackEnemy: {
			state = Stopped;
			if (!enemyTarget || !enemyTarget->alive) {
				if (meetingPoint.x != 0 && meetingPoint.z != 0) {
					soldierState = AttackLocation;
				} else {
					soldierState = Idle;
				}
				lastLookAroundTime = -10;
				break;
			}
			if (dist2(enemyTarget->position,position) > range*range) {
				soldierState = AdvanceTowardEnemy;
				break;
			}
			if (timer->time - lastAttackTime >= vars->getFloatValue("soldier_attackinterval")) {
				lastAttackTime = timer->time;
				Attack(enemyTarget);
			}
			static float weaponAttackTime = vars->getFloatValue("soldier_weaponattacktime");
			if (timer->time - lastAttackAnimationTime > weaponAttackTime) {
				lastAttackAnimationTime = timer->time;
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				glDisable(GL_TEXTURE_2D);
				glLineWidth(2);
				glBegin(GL_LINES);
					glColor3f(0.0f,0.0f,0);
					glVertex3f(position.x,position.y+size.y,position.z);
					glColor3f(1,1,0);
					glVertex3f(enemyTarget->position.x+((rand()%20)/10),enemyTarget->position.y+((rand()%10)/10),enemyTarget->position.z+((rand()%20)/10));
				glEnd();
				glPopAttrib();
			}
			break;
		}
	}
}

void Soldier::Attack(Entity *t) {
	float r = vars->getFloatValue("soldier_attackrange");
	if (t && t->alive && dist2(t->position,position) <= r*r) {
		// spawn missle?
		// set model to animate the attack
		// sound
		t->interact(this,EI_ATTACK);
	}
}