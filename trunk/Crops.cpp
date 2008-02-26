#include ".\crops.h"
#include "timer.h"

extern Timer *timer;

Crops::Crops(void)
{
}

Crops::~Crops(void)
{
}

void Crops::init() {
	SetModel("data/models/crops.md2","data/models/crops.bmp");
}

void Crops::Think() {
//	float seasonProgresion = (timer->time % 120)/120;
}