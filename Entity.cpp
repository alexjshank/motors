
#include "Task.h"
#include "Quadtree.h"
#include "Timer.h"
#include "graphics.h"
#include ".\entity.h"

#include "terrain.h"
#include "EnvironmentObject.h"
#include "lassoselector.h"
#include "trigger.h"
#include "waypoint.h"
//#include "storm.h"

extern Graphics *renderer;
extern LassoSelector *selector;
extern EntityContainer *ents;
extern Timer *timer; 
extern Terrain *terrain;

ENT_TASK *ENT_TASK::CreateScriptedTask(std::string action) {
	scriptedAction = action;
	type = SCRIPTEDACTION;
	return this;
}

ENT_TASK *ENT_TASK::CreateFollow(Entity *entityToFollow) {
	target = entityToFollow;
	type = FOLLOW;
	return this;
}

ENT_TASK *ENT_TASK::CreateMove(Vector moveto) {
	position = moveto;
	type = MOVE;
	return this;
}

ENT_TASK *ENT_TASK::CreateAttack(Entity *t) {
	target = t;
	type = ATTACK;
	return this;
}	

ENT_TASK *ENT_TASK::CreateRepair(Entity *t) {
	target = t;
	type = REPAIR;
	return this;
}

// t is the building that has already been created & placed but has a health of 0
ENT_TASK *ENT_TASK::CreateBuild(Entity *t) {
	target = t;
	type = BUILD;
	return this;
}

Entity::Entity(void) {
	size = Vector(0,0,0);
	leaf = NULL;
	health = 0;
	maxhealth = 0;
	completed = 0;
	tooltip.enabled = false;
	tooltip.tooltip = "";
	tooltip.subtitle = "";
	lastPathTime = 0;
	Attached = false;
}

Entity::~Entity(void)
{
}

void Entity::Attach(Entity *attach_to) {
	Attached = true;
	AttachedTo = attach_to;
}

void Entity::process() {
	position += velocity * timer->frameScalar;
}

void Entity::render() {
	renderToolTip();
}

void Entity::renderToolTip() {
	if (tooltip.enabled && dist2(position,selector->LassoPosition) < 4) {
		renderer->text.printpositional(position + Vector(0,size.y+1,0),"%s %s",tooltip.tooltip.c_str(), tooltip.subtitle.c_str());
	}
}

void Entity::interact(Entity *source, int type) {
	switch (type) {
		case EI_ATTACK:
			health -= source->damage;
			if (health <= 0)
				Kill();
			break;
	}
}

Entity * Entity::PathToNearest(unsigned int requiredType, unsigned int requiredFamily, bool requireAlive, unsigned int requiredTeam) {
	Entity *ent = ents->qtree.tree->getClosestEntity(position,requiredType,requiredFamily,requireAlive,requiredTeam);
	
	if (!PathTo(ent))
		return 0;

	return ent;
}

bool Entity::PathTo(Entity *target) {
	if (target) {
		return PathTo(target->position);
	}
	return false;
}

bool Entity::PathTo(Vector p) {
	if (p.x < 0 || p.z < 0) return false; // blah! deese'ere varmants shoube toss overboar'!
	bool pathed = false;	
	int tries = 1;
	AS_Node *end;

	do {
		end = terrain->AStarSearch(position,p + (Normalize(position - p) * (float)tries));
	} while (!end && tries++ < 6);	
	
	if (!end) return false;

	Vector startp = position;
	startp.y = 0;
	int pathlength = 0;
	int maxPathLength = 30;
	while (end && (end->position-startp).len2() > 1) {
		ENT_TASK *newtask = new ENT_TASK;
		QueueTask(newtask->CreateMove(end->position),true);
		if (end == end->parent) break;
		if (pathlength++ > maxPathLength) break;
		end = end->parent;
	}

	
	return true;
}

bool Entity::QueueTask(ENT_TASK *newTask,bool front) {
	if (newTask) { 
		if (front)
			taskQueue.push_front(newTask);
		else
			taskQueue.push_back(newTask);
		return true;
	}
	return false;
}

ENT_TASK *Entity::GetTask() {
	if (taskQueue.size() > 0) {
		ENT_TASK *task = taskQueue.front();
		taskQueue.pop_front();
		return task;
	}
	return 0;
}

void Entity::ClearTasks() {
	taskQueue.clear();
	state = Stopped;	// we aren't doing anything so set us to stopped
}

void Entity::Remove() {
	selector->Unselect(this);

	int s = (int)ents->entities.size();
	if (leaf) leaf->RemoveEntity(this);
	leaf = 0;
	for (int i=0;i<s;i++) {
		if (ents->entities[i] == this) {
			ents->entities[i] = 0;
			break;
		}
	}

	delete this;
}

void Entity::Kill() {
	alive = false;
	deathTime = timer->time;
}

int Entity::Serialize(unsigned char *byteData, int maxSize) {		// returns number of bytes written to byteData
	struct flattened {
		float x,y;
	} flat;

	flat.x = this->position.x;
	flat.y = this->position.z;

	return 0;
}

int Entity::Deserialize(const unsigned char *data) {
	return 0;
}

EntityContainer::EntityContainer() {
	entities.clear();
	priority = TP_GAME;
	last_id = 0;
	qtree.createTree(Vector(128,0,128),128,5);
}

void EntityContainer::AddEntity(Entity *ent) {
	if (!ent) return;
	ent->id = last_id++;
	entities.push_back(ent);
	if (ent->size.len2() > 0) {
		qtree.AddEntity(ent);
	}
	ent->init();
}


void EntityContainer::run() {
	float a,size;
	int s = (int)entities.size();

	for (int i=0;i<s;i++) {
		Entity *e = entities[i];
		if (e) {
			if (e->leaf) {
				size=e->leaf->getSize();
				a = (float)(e->leaf->getPosition() - e->position).len();
				if (a > size && e->leaf->getParent()) {
					// and we're out of range of that leaf,
					e->leaf->RemoveEntity(e);
					qtree.AddEntity(e);
				}
			} else {
				qtree.AddEntity(e);
			}
			e->process();
		}
	}

	for (int i=0;i<s;i++) {
		Entity *e = entities[i];
		if (e) {
			e->render();
		}
	}
}


