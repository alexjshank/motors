#include "building.h"
#include "vector.h"

class Farm : public Building {
public:
	Farm(void);
	~Farm(void);

	void init();
	void Think();

	float lastSpawnTime;
	float spawnRate;

	float pst;
};
