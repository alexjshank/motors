#include ".\farm.h"
#include "library.h"
#include "terrain.h"
#include "graphics.h"
#include "Camera.h"
#include "variables.h"
#include "input.h"
#include "timer.h"

extern Timer * timer;
extern gamevars *vars;
extern Camera *camera;
extern Library *library;
extern Terrain *terrain;
extern Graphics *renderer;
extern Input *input;

Building::Building(void)
{
	family = EF_BUILDING;
	model = NULL;
	health = 1000;
	toolWindow = 0;
	completed = 0;
}

Building::~Building(void)
{
}

void Building::init() {
	toolWindow = 0;
}

extern int currentID;

void Building::onSelected() {
	if (toolWindow) {
		currentID = id;
		toolWindow->closeButton.pressed = false;
		toolWindow->visible = true;
//		input->inputContext = BuildMenu;
	}
}

void Building::onUnSelected() {
	if (toolWindow) {
		toolWindow->closeButton.pressed = true;
//		input->inputContext = NormalInput;
	}
}

void Building::SetModel(const char*mname, const char*tname) {
	// at this point, position and size should already be set
	if (model) delete model;
	model = new Md2Object;
	model->setModel((Model_MD2*)library->Export(mname));
	position.y = terrain->getInterpolatedHeight(position.x,position.z);
	texture = renderer->LoadTexture(tname);

	terrain->setContents((int)position.x,(int)position.z,(int)size.x, (int)size.z,TC_STRUCTURE);
}


void Building::process() {
	Think();
	if (health < 0) alive = false;

	if (spawnQueue.size() > 0 && timer->time - startBuildTime >= 10) { 
		startBuildTime = timer->time;
		SpawnEntity(spawnQueue.front().c_str(),spawnPoint);
		spawnQueue.pop();
	}
}

void Building::render() {
	if (camera->frustum.pointInFrustum(position) && alive) {
		if (this->completed < 100) {
			DrawCube(position,size);
		} else {
			model->setPosition(position);
			model->setRotation(rotation);
			model->setScale(scale);
			glColor3f(1,1,1);
			glBindTexture(GL_TEXTURE_2D,texture);
			model->drawObjectFrame(0,model->kDrawImmediate);
		
			renderToolTip();
		}
	}
}