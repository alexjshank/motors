#include "vector.h"
/*
class Matrix {
public:
	void operator =(Matrix b) {
		// make sure compiler set to unroll loops here
		for (int i=0; i<16; i++) {
			ar[i] = b.ar[i];
		}
	}
	
	Matrix operator *(Matrix b) {
		for (int i=0; i<16; i++) {
			ar[i] *= b.ar[i];
		}
	}

	union {
		float _m[4][4];
		float ar[16];
		struct {
			float _m00, _m01, _m02, _m03;
			float _m10, _m11, _m12, _m13;
			float _m20, _m21, _m22, _m23;
			float _m30, _m31, _m32, _m33;
		};
	};
};

class Quaternion {
public:
	Quaternion() {
		identity();
	}

	void identity() {
		w = 1;
		x = 0;
		y = 0;
		z = 0;
	}

	Quaternion( float _w, float _x, float _y, float _z) {
		w = _w;
		x = _x;
		y = _y;
		z = _z;
	}

	void operator =(Quaternion q) {
		w = q.w;
		x = q.x;
		y = q.y;
		z = q.z;
	}

	Quaternion operator *(Quaternion q) {
		Quaternion result;
		result.w = (w*q.w) - (x*q.x) - (y*q.y) - (z*q.z);
		result.x = (w*q.x) + (x*q.w) + (y*q.z) - (z*q.y);
		result.y = (w*q.y) - (x*q.z) + (y*q.w) + (z*q.x);
		result.z = (w*q.z) + (x*q.y) - (y*q.x) + (z*q.w);

		return result;
	}

	Quaternion operator *(float s) {
		return Quaternion(w*s,x*s,y*s,z*s);
	}

	Quaternion operator +(Quaternion b) {
		return Quaternion(w*+s,x*s,y *s,z*s);
	}

	void operator *=(Quaternion q) {
		w *= q.w;
		x *= q.x;
		y *= q.y;
		z *= q.z;
	}

	void operator +=(Quaternion q) {
		w += q.w;
		x += q.x;
		y += q.y;
		z += q.z;
	}

	inline float MagnitudeSquared() {
		return (w*w)+(x*x)+(y*y)+(z*z);
	}	

	inline float Magnitude() {
		return sqrtf(MagnitudeSquared());
	}

	void AxisAngle(Vector &vAxisOut, float &fAngleOut) {
		
	}

	void FromAxisAngle(Vector vAxis, float fAngle) {
		float a = fAngle/2;
		float b = sinf( a );
		w = cosf( a );
		x = vAxis.x * b;
		y = vAxis.y * b;
		z = vAxis.z * b;
	}

	void FromMatrix(Matrix &m) {

	}
	
	Matrix GetMatrix() { 
		Matrix m;
		m._m[0][0] = w*w+x*x-y*y-z*z;	m._m[0][1] = 2*x*y - 2*w*z;		m._m[0][2] = 2*x*z+2*w*y;		m._m[0][3] = 0;
		m._m[1][0] = 2*x*y+2*w*z;		m._m[1][1] = w*w-x*x+y*y-z*z;	m._m[1][2] = 2*y*z-2*w*x;		m._m[1][3] = 0;
		m._m[2][0] = 2*x*z-2*w*y;		m._m[2][1] = 2*y*z-2*w*x;		m._m[2][2] = w*w-x*x-y*y+z*z;	m._m[2][3] = 0;
		m._m[3][0] = 0;					m._m[3][1] = 0;					m._m[3][2] = 0;					m._m[3][3] = 1;

		return m;
	}

	void Normalize() {
		float magnitude = Magnitude();
		if (magnitude == 0.0f) {
			*this = Quaternion(1,0,0,0);
			return;
		}
		w = w / magnitude;
		x = x / magnitude;
		y = y / magnitude;
		z = z / magnitude;
	}

	float x,y,z,w;
};

class OrientedBoundingBox {
private:

    void ComputeSpan(Vector axis, float &min, float &max) {
       
        float p = axis.Dot(position);
        float r =    fabs(axis.Dot(orientation[0])) * dimensions.x +
                    fabs(axis.Dot(orientation[1])) * dimensions.y +
                    fabs(axis.Dot(orientation[2])) * dimensions.z;
        min = p-r;
        max = p+r;
    }

    bool AxisOverlap(Vector axis, OrientedBoundingBox *box2) {
        float min0,max0;
        float min1,max1;
        ComputeSpan(axis,min0,max0);
        box2->ComputeSpan(axis,min1,max1);

        return !(min0 > max1 || max0 < min1);
    }

    bool AxisOverlap(Vector axis, OrientedBoundingBox *box2, float &minest, float &maxest) {
        float min0,max0;
        float min1,max1;
        ComputeSpan(axis,min0,max0);
        box2->ComputeSpan(axis,min1,max1);

        return !(min0 > max1 || max0 < min1);
    }
public:   
    OrientedBoundingBox() {
        world = false;
    }

    bool Intersect(OrientedBoundingBox *box2, Vector &collisionNormal)
    {
        int iterator = 0;
        float mins[16],maxs[16];
        Vector axis[16];

        axis[iterator++] = orientation[0];
        axis[iterator++] = orientation[1];
        axis[iterator++] = orientation[2];

        if (!AxisOverlap(axis[0], box2))
            return false;
        if (!AxisOverlap(axis[1], box2))
            return false;
        if (!AxisOverlap(axis[2], box2))
            return false;

        for (int i=0;i<3;i++) {
            for (int o=0;o<3;o++) {
                axis[iterator] = orientation[i]^box2->orientation[o];
                if (!AxisOverlap(axis[iterator], box2, mins[iterator], maxs[iterator]))
                    return false;   

                iterator++;
            }
        }
      
   
        float depth = 1.0E8f;

        for(i= 0; i < iterator; i ++) {            
            float d = maxs[i] - mins[i];
            float Alength = axis[i].len();

            if (Alength < 1.0E-8f)
                continue;

            d /= Alength;

            if (d < depth)
            {
                depth = d;
                collisionNormal = axis[i] / Alength;
            }
        }

        // make sure the normal points towards the box0

        Vector Diff = box2->position - position;
        if (Diff.Dot(collisionNormal) > 0.0f)
            collisionNormal *= -1.0f;

        return true;
    }

    void SetOrientation(Matrix m) {
        orientation[0] = Vector(m._m11,m._m21,m._m31);
        orientation[1] = Vector(m._m12,m._m22,m._m32);
        orientation[2] = Vector(m._m13,m._m23,m._m33);
    }

    Vector position;
    Vector dimensions;
    Vector orientation[3];
    bool world;    // true: this box is in a static position.
};

class RigidBody {
public:
    RigidBody() {
   
        position = Vector(0,0,0);
        momentum = Vector(0,0,0);
        orientation.identity();
        angularMomentum = Vector(0,0,0);
       
    }

    void SetSize(float s) {
        size = s;
    }

    void SetMass(float m) {
        mass = m;
        inverseMass = 1/m;

        inertiaTensor = mass * size;
        inverseInertiaTensor = 1/inertiaTensor;
    }

    void ApplyForce(Vector Force) {
        momentum += Force;
    }
       
    void ApplyTorque(Vector Torque) {
        angularMomentum += Torque;
    }

    void ApplyPositionalForce(Vector p, Vector f) {
        Vector t = p - position;
        ApplyTorque(t ^ f);
        float tl = t.len();
        if (tl > 1)
            ApplyForce(f/tl);
        else
            ApplyForce(f);
    }

    void Process(float dt) {
        velocity = momentum * inverseMass;
        angularVelocity = angularMomentum * inverseInertiaTensor;
           
		orientation.Normalize();
        spin = (Quaternion(0, angularVelocity.x, angularVelocity.y, angularVelocity.z) * 0.5) * orientation;
           
		obb.SetOrientation(orientation.GetMatrix());
        obb.position = position + velocity * dt;

        // test!

        position += velocity * dt;
        orientation += spin * dt;
    }

//private:
    Vector position, velocity, momentum, angularVelocity, angularMomentum;
    Quaternion spin, orientation;
    float size;
    float mass;                float inverseMass;
    float inertiaTensor;    float inverseInertiaTensor;
    OrientedBoundingBox obb;
};
/*

            for (int i=0;i<numBodies;i++) {
//                body[i].ApplyForce(Vector(0,-0.001f,0));

                for (int u=0;u<numBodies;u++) {
                    if (i == u) continue;
                    Vector collisionNormal;
                    if (body[i].obb.Intersect(&body[u].obb,collisionNormal)) {
                        if (!body[u].obb.world) {
                            body[u].ApplyPositionalForce( body[u].position, ( (-(body[u].momentum/body[u].mass) + (body[i].momentum/body[i].mass)) / 2 ) );
                            body[i].ApplyPositionalForce( body[i].position, ( (-(body[i].momentum/body[i].mass) + (body[u].momentum/body[u].mass)) / 2 ) );
                        } else {
                            body[i].ApplyPositionalForce( body[i].position-collisionNormal, -(body[i].momentum/body[i].mass) );
                        }
                    }
                }
                if (body[i].obb.world) continue;
                body[i].Process(dt);
            }
  

        float angle;
        Vector axis;   
       

        for (int i=0;i<numBodies;i++) {
            glPushMatrix();
                glLoadIdentity();
                glTranslatef(body[i].obb.position.x, body[i].obb.position.y, body[i].obb.position.z);
                body[i].orientation.angleAxis(angle, axis);
                glRotatef(angle/0.01745392f, axis.x, axis.y, axis.z);           
                glScalef(body[i].obb.dimensions.x,body[i].obb.dimensions.y,body[i].obb.dimensions.z);
                DrawCube(1);           
            glPopMatrix();
        }
*/