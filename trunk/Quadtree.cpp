#include ".\quadtree.h"
#include "entity.h"
#include "graphics.h"
#include "console.h"

extern Console *console;
#define DEBUG

// how many entities can go into a leaf before it sub divides
#define MAX_LEAF_CONTENTS 50		
// maximum number of times a leaf can sub divide
#define MAX_TREE_LEVEL 40

Node::Node() {
	leaf = false;
}

Node::Node(int nodeLevel, Vector nodePosition, float nodeRadius, Node *nodeParent) {
	position = nodePosition;
	level = nodeLevel;
	parent = nodeParent;
	size = nodeRadius;
	leaf = true;
	contents.clear();
}

bool Node::Subdivide() {
	if (leaf && level < MAX_TREE_LEVEL) {
		children[0] = new Node(level + 1, position + Vector(size/2,0,size/2),size/2,this);
		children[1] = new Node(level + 1, position + Vector(-size/2,0,size/2),size/2,this);
		children[2] = new Node(level + 1, position + Vector(size/2,0,-size/2),size/2,this);
		children[3] = new Node(level + 1, position + Vector(-size/2,0,-size/2),size/2,this);
		leaf = false;

		for (std::list<Entity *>::iterator iter=contents.begin();iter!=contents.end();iter++) {
			AddEntity((*iter));	// this will get passed to the proper child, now that this is a branch
		}
		contents.clear();
		return true;
	}
	return false;
}

Node::~Node() {
	for (int i=0;i<4;i++) {
		delete children[i];
		children[i] = 0;
	}
}


bool Node::AddEntity(Entity *ent) {
	if (ent == 0) return false;

	if (!leaf) {
		if (ent->position.x > position.x) {
			if (ent->position.z > position.z) {
				return children[0]->AddEntity(ent);
			} else {
				return children[2]->AddEntity(ent);
			}
		} else {
			if (ent->position.z > position.z) {
				return children[1]->AddEntity(ent);
			} else {
				return children[3]->AddEntity(ent);
			}
		}
	} else {
		ent->leaf = this;
		contents.push_back(ent);
		if (contents.size() > MAX_LEAF_CONTENTS) {
			Subdivide();
		}
		return true;
	}
}

bool Node::RemoveEntity(Entity *ent) {
	if (ent == 0) return false;
//	console->Printf("Node::RemoveEntity: ent(%f,%f,%f, [type:] %d)",ent->position.x,ent->position.y, ent->position.z, ent->type);
	contents.remove(ent);
	return true;
}

Node *Node::getLeaf(Vector p) {
	if (!leaf) {
		if (p.x > position.x) {
			if (p.z > position.z) {
				return children[0]->getLeaf(p);
			} else {
				return children[2]->getLeaf(p);
			}
		} else {
			if (p.z > position.z) {
				return children[1]->getLeaf(p);
			} else {
				return children[3]->getLeaf(p);
			}
		}	
	} else {
		return this;
	}
}
Entity *Node::getClosestEntity(Vector pos, int type, int family, bool alive, int team) {
	return getClosestEntity(pos,type,family,alive,team,-1);
}

extern Graphics *renderer;
Entity *Node::getClosestEntity(Vector pos, int type, int family, bool alive, int team, int ignoreEntity) {
	Entity *closestEntity=0;
	float closestDistance=9999;				

	if (leaf) {
		for (std::list<Entity *>::iterator iter=contents.begin();iter!=contents.end();iter++) {
			Entity *ent = *iter;
			if ((type != -1 && ent->type != type) || 
				(family != 0 && ent->family != family) || 
				(alive && !ent->alive) || 
				(team != 0 && ent->team != team) ||
				(ent->position == pos) || 
				(ignoreEntity != -1 && ent->id != ignoreEntity)) {
				continue;
			}

			Vector delta = (*iter)->position - pos;
			float distance = (float)delta.len();

			if (distance < closestDistance) {	
				closestDistance = distance;
				closestEntity = (*iter);
			}
		}
		return closestEntity;
	} else {
		int start;
		if (pos.x > position.x) {
			if (pos.z > position.z) {
				start = 0;
			} else {
				start = 2;
			}
		} else {
			if (pos.z > position.z) {
				start = 1;
			} else {
				start = 3;
			}
		}	

		for (int i=0;i<4;i++) {
			int u = (start+i)%4;
			Node *child = children[u];
			closestEntity = child->getClosestEntity(pos, type, family, alive, team);
			if (closestEntity)
				return closestEntity;
		}	
		return closestEntity;
	}
}


Entity *Node::getWaypoint(Vector pos, int lwp1, int lwp2, int lwp3) {
	Entity *closestEntity=0;
	float closestDistance=9999;				

	if (leaf) {
		for (std::list<Entity *>::iterator iter=contents.begin();iter!=contents.end();iter++) {
			Entity *ent = *iter;
			if (ent->type != E_WAYPOINT || (!ent->alive) || ent->id == lwp1 || ent->id == lwp2 || ent->id == lwp3) {
				continue;
			}

			Vector delta = (*iter)->position - pos;
			float distance = (float)delta.len();

			if (distance < closestDistance) {	
				closestDistance = distance;
				closestEntity = (*iter);
			}
		}
		return closestEntity;
	} else {
		int start;
		if (pos.x > position.x) {
			if (pos.z > position.z) {
				start = 0;
			} else {
				start = 2;
			}
		} else {
			if (pos.z > position.z) {
				start = 1;
			} else {
				start = 3;
			}
		}	

		for (int i=0;i<4;i++) {
			int u = (start+i)%4;
			Node *child = children[u];
			closestEntity = child->getWaypoint(pos, lwp1, lwp2, lwp3);
			if (closestEntity)
				return closestEntity;
		}	
		return closestEntity;
	}
}


Quadtree::Quadtree(void)
{
	tree = 0;
}

Quadtree::~Quadtree(void)
{
}

bool Quadtree::createTree(Vector origin, float size, int maxDetailLevel) {
	position = origin;
	tree = new Node(0,position, size, NULL);

	if (tree) 
		return true;
	// else...
	return false;
}

bool Quadtree::AddEntity(Entity *ent) {
	return tree->AddEntity(ent);
}



std::list<Entity *> * Quadtree::getNearbyEntities(Vector pos,float maxDistance) {
	std::list<Entity *> *entities = new std::list<Entity *>;

	Node *leaf = tree->getLeaf(pos);

//	if (distance from leaf->position to pos > maxDistance + leaf->size * 1.3) {

	return entities;
}











