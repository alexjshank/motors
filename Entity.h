#include "Task.h"
#include "Vector.h"
#include <vector>
#include <list>
#include <queue>


#ifndef __ENTITY__H__
#define __ENTITY__H__

enum ENT_FAMILY {
	EF_UNIT = 1,
	EF_BUILDING,
	EF_ENVIRONMENT,
	EF_VEHICLE			// can carry units
};

enum ENT_TYPE {
	E_TREE = 0,
	E_TOWER,
	E_FARM,
	E_PEASANT,
	E_LUMBERMILL,
	E_SHEEP,
	E_LAMPPOST,
	E_REMOVED1,
	E_VILLAGEWOMAN,
	E_SOLDIER,
	E_TRIGGER,
	E_DOCK,
	E_SHIP,
	E_BARRACKS,
	E_WAYPOINT,
	E_NUMENTS
};

extern const char *entNames[E_NUMENTS];

class Entity;

Entity *SpawnEntity(ENT_TYPE type, Vector p);
Entity *SpawnEntity(const char *entityType, Vector p);


enum EI_TYPES {
	EI_USE=0,
	EI_SPEAK,
	EI_ATTACK
};

enum ENT_STATES {
	// all entities:
	Unbuilt = 0,
	Constructing,
	Stopped,		// idle, built

	// Units
	RunToTarget,	
	WalkToTarget,
	FlankTargetLeft,
	FlankTargetRight
};

class ENT_TASK;
class Node;
/*

class RigidBody {
public:
      RigidBody() {
     
            position = Vector(0,0,-10);
            momentum = Vector(0,0,0);
            orientation.identity();
            angularMomentum = Vector(0,0,0);
           
      }
 
      void SetSize(float s) {
            size = s;
      }
 
      void SetMass(float m) {
            mass = m;
            inverseMass = 1/m;
 
            inertiaTensor = mass * size;
            inverseInertiaTensor = 1/inertiaTensor;
      }
 
      void ApplyForce(Vector Force) {
            momentum += Force;
      }
           
      void ApplyTorque(Vector Torque) {
            angularMomentum += Torque;
      }
 
      void ApplyPositionalForce(Vector p, Vector f) {
            Vector t = p - position;
            ApplyTorque(t.cross(f));
            float tl = t.length();
            if (tl > 1)
                  ApplyForce(f/tl);
            else
                  ApplyForce(f);
      }

      void Process(float dt) {
            velocity = momentum * inverseMass;
            angularVelocity = angularMomentum * inverseInertiaTensor;
    
			orientation.normalize();
	        spin = (Quaternion(0, angularVelocity.x, angularVelocity.y, angularVelocity.z) * 0.5) * orientation;
          
            position += velocity * dt;
            orientation += spin * dt;
      }

//private:
      Vector position, velocity, momentum, angularVelocity, angularMomentum;
      Quaternion spin, orientation;
      float size;
      float mass;                   float inverseMass;
      float inertiaTensor;    float inverseInertiaTensor;
};*/




class Entity
{
	friend class EntityContainer;
public:
	Entity(void);
	~Entity(void);

	virtual void init() { }
	virtual void process();
	virtual void render();
	virtual void interact(Entity *source, int type);
	virtual void onDeath() {}
	virtual void onAttacked(Entity *source) {}
	virtual void onSelected() { selected = true;}
	virtual void onUnSelected() { selected = false;}

	virtual int Serialize(unsigned char *byteData, int maxSize);		// returns number of bytes written to byteData
	virtual void Deserialize(const unsigned char *data);

	virtual void renderToolTip();

	Entity * PathToNearest(unsigned int requiredType, unsigned int requiredFamily, bool requireAlive, unsigned int requiredTeam);
	bool PathTo(Entity *target);
	bool PathTo(Vector p);

	virtual void Remove();	// remove from the game.

	bool QueueTask(ENT_TASK * newTask, bool front = false);
	ENT_TASK *GetTask();	// gets the next task in the task queue
	void ClearTasks();

	void Kill();

// Variables -

	struct tagtooltip {
		bool enabled;
		std::string tooltip;	// main title
		std::string subtitle;		// subtitle
	} tooltip;

	int id;
	ENT_STATES state;
	ENT_TYPE type;
	ENT_FAMILY family;			
	bool alive;
	bool selected;
	float scale;
	Vector size;
	Node *leaf;	// the leaf we're in
	std::list<ENT_TASK*> taskQueue;

	int maxhealth;
	int damage;		// how much health we take away from an attacked unit

	float deathTime;
	float lastPathTime;	// to keep units from hammering the path finding algorithms and killing the frame rate

	int team;
	int health;
	float completed;	// 0 -> 100 (%)
	
	Vector position;
	Vector velocity;
	Vector rotation;
};

class ENT_TASK {
public:
	enum TASKTYPES {
		MOVE = 0,
		FOLLOW,
		ATTACK,
		BUILD,
		REPAIR
	};

	ENT_TASK *CreateFollow(Entity *entityToFollow);
	ENT_TASK *CreateMove(Vector moveto);
	ENT_TASK *CreateAttack(Entity *t);
	ENT_TASK *CreateRepair(Entity *t);
	
	// t is the building that has already been created & placed but has a health of 0
	ENT_TASK *CreateBuild(Entity *t);

	TASKTYPES type;			
	Vector position;	// used in move task
	Entity *target;		// used in follow, attack, build, repair
	int modifier;		// attack type?


};

class Node {
	friend class Quadtree;
public:
	Node();
	Node(int nodeLevel, Vector nodePosition, float nodeRadius, Node *nodeParent);
	~Node();

	bool Subdivide();

	bool AddEntity(Entity *ent);
	bool RemoveEntity(Entity *ent);

	Node *getLeaf(Vector p);

	void MoveEntity(Entity *ent); 

	Entity *getClosestEntity(Vector pos, int type, int family, bool alive, int team);
	Entity *getClosestEntity(Vector pos, int type, int family, bool alive, int team, int ignoreEntity);
	
	Entity *getWaypoint(Vector pos, int lwp1, int lwp2, int lwp3);
	
	Vector getPosition() { return position; }
	float getSize() { return size; }

private:
	int level;
	Vector position;
	float size;
	bool leaf;

	Node *parent;
	Node *children[4];

	std::list<Entity *> contents;
};

class Quadtree
{
public:
	Quadtree(void);
	~Quadtree(void);

	bool createTree(Vector origin, float size, int maxDetailLevel);

	bool AddEntity(Entity *ent);

	Entity *getClosestEntity(Vector pos);
	std::list<Entity *> *getNearbyEntities(Vector pos,float maxDistance);

	Vector position;
	Node *tree;
};

class EntityContainer : public Task { friend class Entity;
public:
	EntityContainer();

	void AddEntity(Entity *ent);
	
	void run();

	// quad tree stuff
	void updateEntityNode(Entity *ent);
	Entity *findClosestEntity(Vector target, float max_distance);
	Quadtree qtree;

	// there should be a quad tree index of entities for fast position based lookup
	std::vector<Entity *> entities;
private:
	int last_id;
	int priority;
};

#endif