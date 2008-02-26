#include ".\vehicle.h"
#include "library.h"
#include "terrain.h"
#include "graphics.h"

extern Library *library;
extern Terrain *terrain;
extern Graphics *renderer;

Vehicle::Vehicle(void)
{
	size = Vector(1,1,1);
}

Vehicle::~Vehicle(void)
{
}


void Vehicle::init() {
	model = new Md2Object;
	model->setModel((Model_MD2*)library->Export("data/models/tower/tower.md2"));
	position.y = terrain->getInterpolatedHeight(position.x,position.z);
	texture = renderer->LoadTexture("data/models/tower/tower.bmp");
}


void Vehicle::process() {

}

void Vehicle::render() {
	model->setPosition(position);
	model->setScale(0.08f);
	glBindTexture(GL_TEXTURE_2D,texture);
	glColor3f(1,1,1);
	model->drawObjectFrame(0,model->kDrawImmediate);
}