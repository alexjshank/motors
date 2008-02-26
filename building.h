#include "entity.h"
#include "md2.h"
#include "vector.h"
#include "ui.h"

#ifndef __BUILDING__H__
#define __BUILDING__H__

class Building : public Entity
{
public:
	Building(void);
	~Building(void);

	void init();
	virtual void Think() {}
	void onSelected();
	void onUnSelected();
	void SetModel(const char*mname,const char*tname);
	void process();
	void render();

	Md2Object * getModel() { return model; }

	UIWindow *toolWindow;
	Vector spawnPoint;
	std::queue<std::string> spawnQueue;
	float startBuildTime;

private:
	Md2Object *model;
	int texture;
};

#endif