#include ".\farm.h"
#include "peasant.h"
#include "entity.h"
#include "timer.h"
#include "entity.h"
#include "variables.h"
#include "ui.h"

extern UI *ui;
extern gamevars *vars;
extern Timer *timer;
extern EntityContainer *ents;

Farm::Farm(void)
{
	scale = 0.1f;
	size = Vector(4,2,4);
	type = E_FARM;
	spawnRate = 25;
}

Farm::~Farm(void)
{
}

void Farm::init() {	
	Building::init();

	family = EF_BUILDING;
	type = E_FARM;

	SetModel("data/models/farm.md2","data/models/house.BMP");
	spawnPoint = position + Vector(1,0,1);
	lastSpawnTime = 0;

	spawnRate = vars->getFloatValue("farm_spawnrate");

	toolWindow = (UIWindow *)ui->CreateFromFile("data/UI/farm.ui");

	tooltip.enabled = true;
	tooltip.tooltip = "Farm";
}

void Farm::Think() {		
	if (position.x < 0 || position.z < 0 || position.x > 256 || position.z > 256) {
		return;
	}

	if (timer->time - pst > 0.5f) {
//		particles->Spawn(1,Vector(1,1,1),Vector(0,0,0),position+Vector(-0.4f,4,-1.6f),Vector(0.5f,0,0),Vector(0,2,0),Vector(0.2f,1,0.2f),10,5);
		pst = timer->time;
	}
	Sheep *s=0;
	if (timer->time - lastSpawnTime > spawnRate) {
		lastSpawnTime = timer->time;
		Sheep *ps = (Sheep*)ents->qtree.tree->getClosestEntity(position,E_SHEEP,0,1,team);
		if (ps) {
			s = (Sheep*)SpawnEntity("sheep",ps->position);		// born!
		} else {
			s = (Sheep*)SpawnEntity("sheep",spawnPoint);		// comes out of the farm (imported from sheepland?)
		}
		if (s) {
			s->farm = this;
		}
	}
}