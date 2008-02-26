#include "entity.h"
#include "md2.h"
#include "vector.h"

class LumberTree : public Entity
{
public:
	LumberTree(void);
	~LumberTree(void);	
	
	void init();
	void process();
	void render();

	void interact(Entity *src, int type);

	Md2Object * getModel() { return model; }

	Entity *HarvestedBy;
private:
	float lastSpawnTime;
	int nChildrenSpawned;
	int nPossibleChildren;
	float birthtime;
	Md2Object *model;
	int texture;
};
