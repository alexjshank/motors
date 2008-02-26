#pragma once
#include "entity.h"

class Storm : public Entity {
public:
	Storm(void);
	~Storm(void);

	void init();
	void process();
	void render();
};
