#include ".\peasant.h"
#include "timer.h"
#include "library.h"
#include "terrain.h"
#include "graphics.h"
#include "entity.h"
#include "lumbermill.h"
#include "console.h"
#include "variables.h"
#include "lumbertree.h"

extern gamevars *vars;
extern Console *console;
extern Graphics *renderer;
extern Terrain * terrain;
extern Library *library;
extern Timer *timer;
extern EntityContainer *ents;


Peasant::Peasant(void) {
	type=E_PEASANT;
	family=EF_UNIT;
	size=Vector(1,1,1);
	runspeed = 3.2f;
	walkspeed = 2.5f;
	scale = 0.05f;
	food = NULL;
	home = NULL;
	mill = NULL;
	footprints = true;
	tree = 0;
	chops = 0;
	team = 1;
	peasantState = 0;
	laziness = (float)(5+rand()%10);

	tooltip.enabled = true;
	tooltip.tooltip = "Peasant";
}

void Peasant::init() {
	SetModel("data/models/peasant.md2","data/models/Peasant.BMP");
	runspeed = vars->getFloatValue("peasant_runspeed");
	walkspeed = vars->getFloatValue("peasant_walkspeed");
	damage = vars->getIntValue("peasant_damage");
}

void Peasant::onAttacked(Entity *source) {
	peasantState = RunningAway;
	PathTo(position + (position - source->position));
}

void Peasant::Think() {
	const float chopInterval = 2;

	switch (peasantState) {
	case Idle:
		// figure out what we should do!
		peasantState = WalkingToLumber;
		if (stress >= laziness) {
			if (timer->time - startBreakTime < stress*3) {
				peasantState = Idle;
			}
		}
		break;
	case WalkingToLumber:
		if (state == Stopped && (timer->time - lastPathTime < 1) && (!tree || (tree && dist2(tree->position.flat(), position.flat()) > 4))) {
			lastPathTime = timer->time;
			tree = PathToNearest(E_TREE,0,true,0);
		} 
		if (tree) {
			if (dist2(tree->position.flat(), position.flat()) < 4) {
				peasantState = ChoppingLumber;
			}
		}
		break;
	case ChoppingLumber:
		if (!tree) {
			peasantState = Idle;
			break;
		}	
		if (dist2(tree->position.flat(), position.flat()) > 4) {
			peasantState = WalkingToLumber;
			break;
		}
		if (timer->time - lastChopTime >= chopInterval) {
			chops++;
			lastChopTime = timer->time;
			
			if (!tree->alive && ((LumberTree*)tree)->HarvestedBy && ((LumberTree*)tree)->HarvestedBy->alive) {	// someone else killed it... they get to return it.
				peasantState = Idle;
				break;
			}

			tree->interact(this,EI_ATTACK);   // do chop  (heeyaw!)

			if (!tree->alive) {		// timber! this means we killed it.
				peasantState = GatheringLumber;
				((LumberTree*)tree)->HarvestedBy = this;
			}
		}
		break;
	case GatheringLumber: {
			if (state == Stopped && timer->time - tree->deathTime > 3) {
				if (timer->time - lastPathTime > 1) {
					lastPathTime = timer->time;
					mill = PathToNearest(E_LUMBERMILL,0,true,team);
				}
			}
			
			if (mill && dist(mill->position,position) <= vars->getFloatValue("mill_mincollectiondistance")) {
				((LumberMill*)mill)->UptakeResources(tree);
				mill = 0;
				tree = 0;
				peasantState = Idle;
				stress++;
			} else {
				Vector op = tree->position;
				tree->position = position - Normalize(position - op);
				Vector dir = Normalize(op - tree->position);

				tree->rotation.y = (float)atan2(dir.x,dir.z)/(3.1415926f/180.0f);			
			}
		}
		break;
	case RunningAway:
		if (state == Stopped) {
			peasantState = Idle;
		}
		break;
	default:
		peasantState = 0;
	}



}

Sheep::Sheep() {
	type=E_SHEEP;
	family=EF_UNIT;
	size=Vector(1,1,1);
	runspeed = 2.0f;
	walkspeed = 1.0f;
	farm = NULL;
	scale = 0.05f;
	frame = 0;
}

void Sheep::init() {
	SetModel("data/models/sheep.md2","data/models/Sheep.BMP");
	born = timer->time;
}

void Sheep::onAttacked(Entity *source) {
	state = RunToTarget;
	target = position + (position - source->position);
}

void Sheep::Think() {
	scale = 0.02f + ((timer->time - born)/1000);
	if (scale > 0.04f) scale = 0.04f;

	if (timer->time - lastThinkTime > 5) {
		lastThinkTime = timer->time;

		if (timer->time - born > vars->getIntValue("sheep_lifespan")) {
			Kill();
			return;
		}
		Vector newP = position + Vector((rand()%10)-5.0f,0.0f,(rand()%10)-5.0f);

		target = newP;
		state = WalkToTarget;
	}
}

VillageWoman::VillageWoman() {
	type = E_VILLAGEWOMAN;
	family=EF_UNIT;
	size = Vector(1,2,1);
	runspeed = 3.0f;
	walkspeed = 2.0f;
	scale = 0.05f;
	footprints = true;
	food=0;
	farm=0;
	lastEatTime = -25;
	cycleiter=0;
	team = 1;
}

void VillageWoman::init() {
	SetModel("data/models/villagewoman.md2","data/models/VillagerWoman.BMP");
	born = timer->time;
}

void VillageWoman::Think() {
	if (!farm) {
		farm = ents->qtree.tree->getClosestEntity(position, E_LUMBERMILL, 0, 1, team);
	}

	if (cycleiter == ReturnToButchers && food) {
		food->position = position + Normalize(food->position - position);
	}

	if (state == Stopped) {
	
		switch (cycleiter) {
			case 0:
				if (timer->time - lastEatTime > 30) {										// if we're hungry
					cycleiter = GoToSheep;	
				}
				break;
			
			case GoToSheep:
				if (!food) {												
					food = ents->qtree.tree->getClosestEntity(position,E_SHEEP,0,1,team);			// find something to eat
				}

				if (food) {
					if (DistanceBetween(food->position, position) < 3) {							// if we're close enough to the food to pick it up
						((Unit*)food)->Kill();
						cycleiter = ReturnToButchers;
					} else {
						PathTo(food->position);						
					}		
				}
				break;

			case ReturnToButchers:
				if (farm) {
					if (DistanceBetween(position.flat(),farm->position.flat()) < 5) {
						lastEatTime = timer->time;												// dig in!	
						((LumberMill*)farm)->UptakeResources(food);
						food = 0;
						cycleiter=0;
					} else {
						PathTo(farm->position);
					}
				}
				break;

		}
	}		
}
