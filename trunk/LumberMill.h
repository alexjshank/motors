#include "building.h"
#include "vector.h"

class LumberMill : public Building
{
public:
	LumberMill(void);
	~LumberMill(void);

	void init();
	void Think();

	void UptakeResources(Entity *res);

	float lastSpawnTime;

	int lumber;
	int food;
};
