#include "building.h"

class Crops :
	public Building
{
public:
	Crops(void);
	~Crops(void);

	void init();
	void Think();
};
