#include ".\ship.h"
#include "variables.h"

extern gamevars *vars;
extern int inputContext;
extern Entity *entControlling;

Ship::Ship(void) {
	type = E_SHIP;
	scale = 0.25f;
	size=Vector(3,3,3);

	tooltip.enabled = true;
	tooltip.tooltip = "Pirate Ship";
}

Ship::~Ship(void)
{
}


void Ship::onSelected() {
	inputContext = 4;
	entControlling = this;
}

void Ship::onUnSelected() { 
	inputContext = 0;
	entControlling = 0;
}

void Ship::init() {
	SetModel("data/models/ship.md2","data/models/ship.bmp");
	runspeed = vars->getFloatValue("ship_topspeed");
	team=2;
}

void Ship::process() {
	Think();
}

void Ship::Think() {
	if (velocity.len2() > runspeed*runspeed) {
		velocity.normalize();
		velocity *= runspeed;
	}
	if (!selected) velocity *= 0.9f;
	position += velocity * timer->frameScalar;
	position.y = terrain->fAvgWaterHeight - 0.6f;
	rotation.z = atan2f(velocity.x,velocity.z) / (3.1415f/180.0f);
	model->setRotation(rotation);
	model->setPosition(position);

	if (terrain->getInterpolatedHeight(position.x,position.z) > terrain->fAvgWaterHeight) {
		// washed ashore!
		position -= velocity * timer->frameScalar;
		velocity = Vector(0,0,0);
		onUnSelected();
	}
}