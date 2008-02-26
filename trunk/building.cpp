#include ".\farm.h"
#include "library.h"
#include "terrain.h"
#include "graphics.h"
#include "Camera.h"
#include "variables.h"

extern gamevars *vars;
extern Camera *camera;
extern Library *library;
extern Terrain *terrain;
extern Graphics *renderer;

Building::Building(void)
{
	family = EF_BUILDING;
	model = NULL;
	health = 1000;
	toolWindow = 0;
}

Building::~Building(void)
{
}

void Building::init() {
	toolWindow = 0;
}

extern int inputContext;
extern int currentID;

void Building::onSelected() {
	if (toolWindow) {
		currentID = id;
		toolWindow->closeButton.pressed = false;
		toolWindow->visible = true;
		inputContext = 1;
	}
}

void Building::onUnSelected() {
	if (toolWindow) {
		toolWindow->closeButton.pressed = true;
		inputContext = 0;
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
}

void Building::render() {
	if (camera->frustum.pointInFrustum(position) && alive) {
		model->setPosition(position);
		model->setRotation(rotation);
		model->setScale(scale);
		glColor3f(1,1,1);
		glBindTexture(GL_TEXTURE_2D,texture);
		model->drawObjectFrame(0,model->kDrawImmediate);
	
		renderToolTip();
	}
}