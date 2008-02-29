#include ".\ship.h"
#include "variables.h"
#include "input.h"
#include "entity.h"

extern EntityContainer *ents;
extern gamevars *vars;
extern Input *input;
extern Entity *entControlling;

Ship::Ship(void) {
	type = E_SHIP;
	scale = 0.2f;
	size=Vector(3,3,3);

	selected = false;
	tooltip.enabled = true;
	tooltip.tooltip = "Pirate Ship";
	waypointHistory[0] = 0;

}

Ship::~Ship(void)
{
}


void Ship::onSelected() {
	input->inputContext = Sailing;
	entControlling = this;
	selected = true;
}

void Ship::onUnSelected() { 
	input->inputContext = NormalInput;
	entControlling = 0;
	selected = false;
}

void Ship::init() {
	SetModel("data/models/ship.md2","data/models/ship.bmp");
	runspeed = vars->getFloatValue("ship_topspeed");
	team=2;
}

void Ship::onAttacked() {
	// kaboom
	particles->Spawn(100,Vector(1,0.5,0),Vector(0,0,0), position, Vector(0,-1,0),Vector(0,5,0),Vector(5,5,5),50,20);
}

void Ship::process() {
	if (velocity.len2() > runspeed*runspeed) {
		velocity.normalize();
		velocity *= runspeed;
	}
	if (!selected) {
		// ai!	
		Think();
	} else {
		if (terrain->getInterpolatedHeight(position.x,position.z) > terrain->fAvgWaterHeight) {
			// washed ashore!
			position -= velocity * timer->frameScalar;
			velocity = Vector(0,0,0);
			onUnSelected();
		}
	}

	position += velocity * timer->frameScalar;
	position.y = terrain->fAvgWaterHeight - 0.6f;
	rotation.z = atan2f(velocity.x,velocity.z) / (3.1415f/180.0f);
	model->setRotation(rotation);
	model->setPosition(position);
}

void Ship::Think() {

	if (!waypointHistory[0]) {
		waypointHistory[0] = waypointHistory[1] = waypointHistory[2] = waypointHistory[3] = ents->qtree.tree->getClosestEntity(position,E_WAYPOINT,0,1,0);
	} else {
		if (dist(waypointHistory[0]->position.flat(),position.flat()) < 10) {
			waypointHistory[3] = waypointHistory[2];
			waypointHistory[2] = waypointHistory[1];
			waypointHistory[1] = waypointHistory[0];
            
			waypointHistory[0] = ents->qtree.tree->getWaypoint(waypointHistory[0]->position + velocity,waypointHistory[1]->id,waypointHistory[2]->id,waypointHistory[3]->id);
		}
	}
	if (waypointHistory[0]) {
		velocity += Normalize(waypointHistory[0]->position.flat() - position.flat()) / 10;
	}
}