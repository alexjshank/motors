#include ".\environmentobject.h"
#include "timer.h"
#include "library.h"
#include "terrain.h"
#include "graphics.h"
#include "entity.h"
#include "camera.h"

extern Camera *camera;
extern Graphics *renderer;
extern Terrain * terrain;
extern Library *library;
extern Timer *timer;
extern EntityContainer *ents;

EnvironmentObject::EnvironmentObject(void)
{
	model = NULL;
	texture = -1;
	scale = 0.08f;
	family = EF_ENVIRONMENT;
	// very simple 3d object that doesn't do anything but is simply for looks
}

EnvironmentObject::~EnvironmentObject(void)
{
}

void EnvironmentObject::render() {
	Think();
	if (camera->frustum.pointInFrustum(position)) {
		model->setScale(scale);
		model->setPosition(position);		
		model->setRotation(rotation);

		glBindTexture(GL_TEXTURE_2D,texture);
		model->drawObjectFrame(1,model->kDrawImmediate);
	}
}

void EnvironmentObject::SetModel(const char *modelname, const char *texturename) {
	if (model) delete model;
	model = new Md2Object;
	model->setModel((Model_MD2*)library->Export(modelname));
	texture = renderer->LoadTexture(texturename);

}


