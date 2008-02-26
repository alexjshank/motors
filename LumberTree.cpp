#include ".\lumbertree.h"
#include "library.h"
#include "terrain.h"
#include "graphics.h"
#include "camera.h"
#include "timer.h"
#include "variables.h"

extern gamevars *vars;
extern Timer *timer;
extern Library *library;
extern Terrain *terrain;
extern Graphics *renderer;
extern Camera *camera;

LumberTree::LumberTree(void)
{
	size = Vector(2,4,2);
	type = E_TREE;
	family = EF_ENVIRONMENT;
	team = 1;
	lastSpawnTime = 0;
	nChildrenSpawned = 0;
	HarvestedBy = 0;
}

LumberTree::~LumberTree(void)
{
}


void LumberTree::init() {
	model = new Md2Object;
	model->setModel((Model_MD2*)library->Export("data/models/felwoodbush1.md2"));
	position.y = terrain->getInterpolatedHeight(position.x,position.z);
	texture = renderer->LoadTexture("data/models/FelwoodNatural.BMP");
	birthtime = timer->time;
	
	if (birthtime < 10) birthtime -= 60;

	terrain->setContents((int)position.x,(int)position.z,TC_STRUCTURE);
}

void LumberTree::interact(Entity *src, int type) {
	if (type == EI_ATTACK) {
		rotation.y = (float)(rand()%20);	// this could be animated ... but im not an animator
	}
	Entity::interact(src,type);
}

void LumberTree::process() {
	if (alive) {
		// eventually...: give that player lumber resources when the tree falls
		float height = (0.1f*((timer->time - birthtime) / 100));
		if (height > 0.06f) height = 0.06f;
		model->setScale(0.01f + height);

		rotation /= 1+(2 * timer->frameScalar);
		model->setRotation(rotation);

	} else {
		if (rand()%1000 == 123) {	
			Vector newP = position + Vector(((rand()%100)/20)-2.5f,0,((rand()%100)/20)-2.5f);
			if (terrain->getContents((int)newP.x, (int)newP.z)==0) {
				SpawnEntity("tree",newP);
			}
		}
	
		if (rotation.x < 90) rotation.x += 4;
		model->setRotation(Vector(rotation.x,0,atan2(velocity.x,velocity.z+0.01f)/(3.1415f/180)));
	}
}

void LumberTree::render() {
	if (camera->frustum.pointInFrustum(position)) {
		model->setPosition(position);
		model->setRotation(rotation);

		glPushAttrib(GL_FOG_BIT);
		glColor4f(1,1,1,1.0f);	
		glDisable(GL_FOG);
		glBindTexture(GL_TEXTURE_2D,texture);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE_MINUS_SRC_COLOR,GL_SRC_COLOR);
		glDepthMask(0);

		model->drawObjectFrame(0,model->kDrawImmediate);
		
		glDepthMask(1);		
		glDisable(GL_BLEND);
		glPopAttrib();
	}
}