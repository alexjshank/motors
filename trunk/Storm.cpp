#include ".\storm.h"
#include "ParticleManager.h"

extern ParticleManager *particles;

Storm::Storm(void)
{
	position = Vector(130,20,140);
}

Storm::~Storm(void)
{
}

void Storm::init() {


}

void Storm::process() {
	for (int i=0;i<4;i++) {
		float iter = (timer->time / 10) + i;
		particles->Spawn(1,Vector(1,1,1), Vector(0,0,0),Vector(125,25,125), Vector(0,0,0), Vector(sin(iter),0,cos(iter)), Vector(1,1,1),400,0);
	}
}

void Storm::render() {


}