#include "task.h"
#include "vector.h"

#ifndef _CAMERA_
#define _CAMERA_

enum {
	TOP = 0, BOTTOM, LEFT,
	RIGHT, NEARP, FARP
};
enum {OUTSIDE, INTERSECT, INSIDE};

class Plane {
public:
	void set3Points(Vector a, Vector b, Vector c) {
		_Position = a;
		_Normal = Normalize((c - a) ^ (b - a));
		d = -(_Normal.x * a.x + _Normal.y * a.y + _Normal.z * a.z);
	}
	inline float distance(Vector p) {
		return _Normal.x * p.x + _Normal.y * p.y + _Normal.z + p.z + d;
	}
	Vector _Position;	// a point on the plane
	Vector _Normal;
	float d;
};
    

class Frustum {
public:
	Frustum::Frustum() {}
	Frustum::~Frustum() {}

	void setCamInternals(float angle, float ratio, float nearD, float farD);
	void setCamDef(Vector &p, Vector &l, Vector &u);
	int pointInFrustum(Vector p);
	int sphereInFrustum(Vector &p, float raio);
//	int boxInFrustum(AABox &b);

	Vector p, v;
	float angle;
};

class Camera : public Task {
public:
	Camera();
	~Camera();

	bool init();
	void run();

	// viewing stuff:

	void LookAt(Vector pos);
	void RotateView(float x, float y, float z);
	Vector GetRotation() { return rotation; }
	Vector GetView() { CalculateView(); return view; }
	Vector GetSide() { CalculateView(); return side; }
	Vector GetUp() { CalculateView(); return up; }
	
	void StopFollowing() {
		followmode = NotFollowing;
		target = NULL;
	}
	void Follow(Vector *ft, int fm) { 
		followmode = fm;
		target = ft;
		targetPosition = position;
	}

	// moving stuff:

	void MoveRelative(Vector relativeMovement);
	void MoveAbsolute(Vector absoluteMovement);

	void SetPosition(Vector p) { position = p; }
	Vector GetPosition() { return position; }
	Vector GetActualPosition() { return position - (view*zoom); }

	void SetZoom(float z) { zoom = z; }

	enum FollowModes { NotFollowing = 0, Seek, HoverAbove };

	Frustum frustum;
private:
	void CalculateView();
	void setProjectionMatrix(float zoomFactor, int width, int height);

	Vector *target;
	Vector targetPosition;
	int followmode;	
	
	Vector position,  up, view, side;
	Vector rotation;
	float zoom;
	float i;
	int priority;
	bool bViewCalculatedThisFrame;
};

#endif