#include ".\lumbermill.h"

#include "library.h"
#include "terrain.h"
#include "graphics.h"
#include "Camera.h"
#include "particlemanager.h"
#include "timer.h"

extern ParticleManager *particles;
extern Timer *timer;
extern Camera *camera;
extern Library *library;
extern Terrain *terrain;
extern Graphics *renderer;
extern UI *ui;

LumberMill::LumberMill(void)
{
	size = Vector(3,2,3);
	type=E_LUMBERMILL;
	family = EF_BUILDING;
	scale = 0.06f;
	lumber = 0;
	food = 0;

	tooltip.enabled = true;
	tooltip.tooltip = "Mill";
}

LumberMill::~LumberMill(void)
{
}

void LumberMill::init() {
	Building::init();
	size = Vector(3,2,3);

	SetModel("data/models/mill.md2","data/models/LumberMill.BMP");
	toolWindow = (UIWindow *)ui->CreateFromFile("data/UI/buildmenu.ui");
}


void LumberMill::Think() {
	if (timer->time - lastSpawnTime > 0.2f) {
		particles->Spawn(1,Vector(1,1,1),Vector(0,0,0),position+Vector(0.5f,2,-0.7f),Vector(0.5f,0,0),Vector(0,2,0),Vector(0.2f,1,0.2f),50,25);
		lastSpawnTime = timer->time;
	}

	char buf[50];
	sprintf(buf," - Wood: %d - Food: %d",lumber,food);
	tooltip.subtitle = buf;

}

void LumberMill::UptakeResources(Entity *res) {
	if (!res) return;
	switch (res->type) {
	case E_TREE:
		lumber += 20;
		res->Remove();
		
		break;
	case E_SHEEP:
		food += 10;
		res->Remove();

		break;
	}
}
