#include "task.h"
#include "timer.h"
#include "vector.h"
#include "graphics.h"
#include "terrain.h"

extern Terrain *terrain;
extern Timer * timer;

#ifndef __PARTICLEMANAGER__H__
#define __PARTICLEMANAGER__H__

class Particle {
public:
	Particle() {
		lifespan = 5;
	}

	void Process();

	void Render() {
		glPointSize(size);	
		glBegin(GL_POINTS);
			float a = 1-((timer->time - birthtime) / lifespan);

			glColor4f(color.x, color.y, color.z, a);			
			glVertex3f(position.x,position.y,position.z);
		glEnd();
	}

	void Spawn(int t, Vector c, Vector ec, Vector sp, Vector a, Vector v, float m, float tv, float s, float gr) {
		alive = true;
		birthtime = timer->time;
		startcolor = color = c;
		endcolor = ec;

		texture = t;
		position = sp;
		acceleration = a;
		velocity = v;
		mass = m;
		terminalVelocity = tv;

		growthRate = gr;
		size = s;

	}

	bool alive;

	float lifespan;
	float birthtime;

	float terminalVelocity;
	float mass;
	Vector position;
	Vector velocity;
	Vector acceleration;

	Vector startcolor;
	Vector color;		// x=r, y=g, z=b
	Vector endcolor;

	float size;
	float growthRate;		
	int texture;
};

#define MAX_PARTICLES 3000

class ParticleManager :
	public Task
{
public:
	ParticleManager(void);
	~ParticleManager(void);

	bool init();
	void run();

	void Spawn(int count, Vector color, Vector endcolor, Vector position, Vector accel, Vector iv, Vector spread, float size, float gr);

private:
	Particle *particles;
	float spriteMaxSize;
	int particleTexture;

	int priority;
	int last_id;
};
extern ParticleManager *particles;
class ParticleEmitter {
public:
	void init(float f, int b, Vector p, Vector a, Vector v, Vector s, Vector c, float si, float grow) {
		size = si;
		gr = grow;
		frequency = f;
		burstCount = b;
		position = p;
		accel = a;
		velocity = v;
		spread = s;
		color = c;
		lastSpawnTime = timer->time;

		on = true;
	}

	void Run() {
		if (on && timer->time - lastSpawnTime > frequency) {
			particles->Spawn(burstCount,color,color,position,accel,velocity,spread,size,gr);
			lastSpawnTime = timer->time;
		}
	}

	bool on;
	float lastSpawnTime;

	float frequency;		// how often to fire
	int burstCount;		// how many to fire each time
	Vector position;
	Vector accel;
	Vector velocity;
	Vector spread;
	Vector color;
	float size;
	float gr;
};

#endif