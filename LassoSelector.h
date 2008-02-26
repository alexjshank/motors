#pragma once
#include "task.h"
#include "vector.h"
#include "entity.h"
#include <vector>

struct selectionPoint { Vector v; bool a; };

class LassoSelector :
	public Task
{
public:
	LassoSelector(void);
	~LassoSelector(void);

	bool init();
	void run();

	void Select(Entity *e);
	void ClearSelection();

	bool enabled;

	std::vector<Entity*> SelectedEntities;
	std::vector<selectionPoint> selectionPoints;
	bool selectionBroke;
	float selectTime;
	float selectFadeTime;
	Vector LassoPosition, gridAlignedLassoPosition;
	int texSelectedEntity;
};
