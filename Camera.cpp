#include ".\camera.h"
#include "timer.h"
#include "graphics.h"
#include "input.h"
#include "terrain.h"
#include "variables.h"
#include <gl\glu.h>
#pragma comment(lib,"glu32.lib")

extern gamevars *vars;
extern Input *input;
extern Terrain *terrain;

#define ANG2RAD 3.14159265358979323846/180.0
#define PIover180 0.017453292f

void Frustum::setCamInternals(float angle, float ratio, float nearD, float farD) {
//	this->ratio = ratio;
	this->angle = angle;
//	this->nearD = nearD;
//	this->farD = farD;
}


void Frustum::setCamDef(Vector &pos, Vector &l, Vector &u) {
	this->p = pos;
	this->v = l;	
}

int Frustum::pointInFrustum(Vector pos) {
	Vector v2 = Normalize(pos-p);
	float a = acos(v.Dot(v2))/PIover180;
	if (a > angle) {
		return OUTSIDE;
	}
	return INSIDE;
}

int Frustum::sphereInFrustum(Vector &p, float radius) {
	return 0;
}

Camera::Camera(void)
{
	priority = TP_GAME;
	up = Vector(0,1,0);
}

Camera::~Camera(void)
{
}

void Camera::setProjectionMatrix(float zoomFactor, int width, int height) { 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluPerspective (50.0*zoomFactor, (float)width/(float)height, 0.1f,300); 
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
}

#define PI 3.14159265f

bool Camera::init() {
	setProjectionMatrix(1.0f,vars->getIntValue("screen_width"),vars->getIntValue("screen_height"));
	frustum.setCamInternals(65, 1.3f, 0.01f, 200);
	return true;
}

extern Timer *timer;
void Camera::run() {	
	Vector cameraPush,a;
	float h;

	glRotatef(rotation.x,1.0f,0,0);
	glRotatef(rotation.y,0,1.0f,0);
	glTranslatef(-position.x+view.x, -position.y+view.y, -position.z+view.z);
	bViewCalculatedThisFrame = false;

	frustum.setCamDef(GetPosition(),GetView(),GetUp());
	
	static float maxdistance = vars->getFloatValue("camera_maxdistance");

	if (followmode) {
		switch (followmode) {
		case Seek:
			LookAt(*target);
			targetPosition = *target;
			h = terrain->getInterpolatedHeight(targetPosition.x, targetPosition.z);
			if (h > targetPosition.y) targetPosition.y = h;
			break;

		case HoverAbove:
			LookAt(*target);
			if (dist2(position,*target) > maxdistance*maxdistance) {
				targetPosition = position + (Normalize(*target - position));
				targetPosition.y = terrain->getInterpolatedHeight(targetPosition.x, targetPosition.z) + 20;
			}
			break;
		}
		Vector d = (targetPosition - GetPosition());
		float l = (float)d.len2();
		if (l > 15) l = 15.0f;
		if (l > 0.01) {
			cameraPush = (targetPosition - GetPosition()) / 2;
			MoveAbsolute((cameraPush*timer->frameScalar*l));
		}
	}
}

void Camera::LookAt(Vector pos) {
	view = pos - position;
	view.normalize();

	rotation.y = 180+(float)-(atan2(view.x,view.z)/(PI/180));
	float xyDelta = (float)sqrt((view.x*view.x)+(view.z*view.z));
	float v=view.y;

	rotation.x = -asin(v)/(PI/180);
}

void Camera::RotateView(float x, float y, float z) {
	rotation += Vector(x,y,z);
/*	if (rotation.x < 0) rotation.x = 360;
	if (rotation.y < 0) rotation.y = 360;
	if (rotation.z < 0) rotation.z = 360;
	if (rotation.x > 360) rotation.x = 0;
	if (rotation.y > 360) rotation.y = 0;
	if (rotation.z > 360) rotation.z = 0;
*/
}

extern Graphics *renderer;
void Camera::CalculateView() {
	if (bViewCalculatedThisFrame) return;
	Vector v;

	v.x = (float)sin(rotation.y*(PI/180));
	v.y = (float)-sin((rotation.x)*(PI/180));
	v.z = (float)-cos(rotation.y*(PI/180));

	up = Vector(0,1,0);
	view	= Normalize(v);	
	side	= Normalize(up^view);
	up		= Normalize(view^side);	

	bViewCalculatedThisFrame = true;
}