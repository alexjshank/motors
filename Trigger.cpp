#include ".\trigger.h"

#include "entity.h"
#include <string.h>
#include "timer.h"
#include "console.h"
#include "variables.h"

extern Timer *timer;
extern Console *console;
extern EntityContainer *ents;

Trigger::Trigger(void) {
	type = E_TRIGGER;
	family = EF_ENVIRONMENT;
	size = Vector(0,0,0);
	lastTriggerTime = 0;
}

void Trigger::Create(Vector p, float radius, float resetTime, const char *script) {
	creationTime = timer->time;
	triggerType = HOTSPOT;
	triggerScript = script;

	triggerResetTime = resetTime;
	hotspotRadius = radius;
	position = p;
}

void Trigger::Create(float time, bool triggerTime, const char *script) {
	creationTime = timer->time;
	triggerScript = script;
	lifespan = time;
	triggerType = (triggerTime) ? TRIGGERTIME : GAMETIME;
}

void Trigger::Pull() {
	lastTriggerTime = timer->time;
	console->RunLine(triggerScript.c_str());
}

void Trigger::process() {
	switch (triggerType) {
		case HOTSPOT: {					
			if (timer->time - lastTriggerTime < triggerResetTime) break;

			Entity *closestEntity = ents->qtree.tree->getClosestEntity(position,hotspotTypeFilter,hotspotFamilyFilter,true,hotspotTeamFilter);

			if (closestEntity && dist(closestEntity->position, position) < hotspotRadius) {
				Pull();
			}

			break;
		}
		case GAMETIME: {
			if (timer->time > lifespan) {
				Pull();
				Remove();		// gametime and triggertime triggers can only be triggered once, so remove them after they're triggered
			}
			break;
		}
		case TRIGGERTIME: {
			if (timer->time - creationTime > lifespan) {
				Pull();
				Remove();
			}
			break;
		}
	}
}
