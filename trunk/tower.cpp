#include "Tower.h"

Tower::Tower(void)
{
	scale = 0.14f;
	size = Vector(5,8,5);
	type=E_TOWER;
}

Tower::~Tower(void)
{
}

void Tower::init() {
	Building::init();

	SetModel("data/models/tower/tower.md2","data/models/tower/tower.JPG");
	tooltip.enabled = true;
	tooltip.tooltip = "Tower";
	toolWindow = 0;
}


void Tower::Think() {
	// if not built yet
		// building stuff
	// else
		// targeting / attacking stuff!
}

