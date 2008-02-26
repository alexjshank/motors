#pragma once
#include "entity.h"
#include <string.h>
#include "timer.h"
#include "console.h"
#include "variables.h"

class Trigger : public Entity {
public:
	Trigger(void);
	~Trigger(void);

	void init() {}
	void Create(Vector p, float radius, float resetTime, const char *script);
	void Create(float time, bool triggerTime, const char *script);
	void Pull();
	void process();
	void render() {
		// triggers don't need to be rendered normally... but we should render a box for debug mode
	}


	float creationTime;
	enum TriggerTypes {
		HOTSPOT = 0,
		GAMETIME,
		TRIGGERTIME
	};
	int triggerType;

	float lastTriggerTime;
	float triggerResetTime;
	float hotspotRadius;
	int	hotspotTypeFilter;
	int hotspotFamilyFilter;
	int hotspotTeamFilter;

	float lifespan;

	std::string triggerScript;
};
