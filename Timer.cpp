#include ".\timer.h"
#include "SDL.h"
 
Timer::Timer(void)
{
	run();
	priority = TP_ENGINE;
}

Timer::~Timer(void)
{
}

void Timer::run() {
	lastFrame = time;
	time = (float)SDL_GetTicks()/1000;
	frameDifference = time - lastFrame;
	fps = 1000*frameDifference;
	frameScalar = frameDifference;
	if (frameScalar > 1.0f) frameScalar = 1.0f;
	if (frameScalar <= 0) frameScalar = 0.0001f;
}