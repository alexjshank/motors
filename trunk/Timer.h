#include "Task.h"

#ifndef __TIMER__H__
#define __TIMER__H__

class Timer : public Task
{
public:
	Timer(void);
	~Timer(void);

	void run();

	float fps;
	float frameScalar;		// scale vectors by this value to acheive frame-rate independant movement.
	float lastFrame;
	float frameDifference;
	float time;

private:
	int priority;
};

#endif