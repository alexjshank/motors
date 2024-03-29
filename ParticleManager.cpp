#include ".\particlemanager.h"
#include "graphics.h"
#include "terrain.h"

extern Graphics *renderer;
extern Terrain *terrain;

PFNGLPOINTPARAMETERFARBPROC  glPointParameterfARB  = NULL;
PFNGLPOINTPARAMETERFVARBPROC glPointParameterfvARB = NULL;

void Particle::Process() {
	if (alive) {
		if (timer->time - birthtime >= lifespan) {
			alive = false;
		}

		velocity += (acceleration/mass) * timer->frameScalar;
		if (velocity.len2() > terminalVelocity * terminalVelocity) {
			velocity = Normalize(velocity) * terminalVelocity;
		}

		position += velocity * timer->frameScalar;

/*		if (terrain) {
			float ground = terrain->getHeight((int)position.x,(int)position.z);
			if (position.y < ground + (size/100)) {
				position.y = terrain->getInterpolatedHeight(position.x,position.z)+(size/100);
			}
		}
*/		size += growthRate * timer->frameScalar;
	
		float i = (timer->time - birthtime) / lifespan;

		color = startcolor + (endcolor - startcolor) * i;
	}
}

ParticleManager::ParticleManager(void) {
	priority = TP_GAME;
	last_id = 0;
}

ParticleManager::~ParticleManager(void)
{
}

bool ParticleManager::init() {
	char *ext = (char*)glGetString( GL_EXTENSIONS );

    if( strstr( ext, "GL_ARB_point_parameters" ) == NULL )
    {
        MessageBox(NULL,"GL_ARB_point_parameters extension not found","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return false;
    }
    else
    {
        glPointParameterfARB  = (PFNGLPOINTPARAMETERFARBPROC)wglGetProcAddress("glPointParameterfARB");
        glPointParameterfvARB = (PFNGLPOINTPARAMETERFVARBPROC)wglGetProcAddress("glPointParameterfvARB");

        if(!glPointParameterfARB || !glPointParameterfvARB) {
			MessageBox(NULL,"problem finding GL_ARB_point_parameters functions (your video card sucks)","ERROR",MB_OK|MB_ICONEXCLAMATION);
            return false;
        }
    }

	particles = new Particle[MAX_PARTICLES];

	glGetFloatv( GL_POINT_SIZE_MAX_ARB, &spriteMaxSize );
	glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );
	glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, 0.01f );
	glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, spriteMaxSize );
	glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );

	particleTexture = renderer->LoadTexture("data/models/particle.JPG");

	return true;
}

void ParticleManager::run() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		float quadratic[] =  { 1.0f, 0.0f, 0.01f };
		glPointParameterfvARB( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );

		glEnable(GL_POINT_SPRITE_ARB);

	//	glDepthMask(0);
	//	glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_COLOR);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,particleTexture);

		for( int i = 0; i < MAX_PARTICLES; i++) {
			if (!particles[i].alive) continue;
			if (particles[i].size > spriteMaxSize) particles[i].size = spriteMaxSize;
				
			particles[i].Process();
			particles[i].Render();	
		}
		
	glPopAttrib();
}

void ParticleManager::Spawn(int count, Vector color, Vector endcolor, Vector position, Vector acceleration, Vector iv, Vector spread, float size, float gr) {
	int spawned = 0;
	for (int i=0;i<MAX_PARTICLES;i++) {
		int o = (i+last_id)%MAX_PARTICLES;
		if (!particles[o].alive) {
			float h=terrain->getInterpolatedHeight(position.x,position.z);
			if (h > position.y) position.y = h;
			particles[o].Spawn(particleTexture, color, endcolor, position, acceleration, iv + Vector((((float)(rand()%100)/100.0f)-0.5f)*spread.x,(((float)(rand()%100)/100.0f)-0.5f)*spread.y,(((float)(rand()%100)/100.0f)-0.5f)*spread.z),1.0f,600.0f,size,gr);
			if (spawned++ >= count) { 
				last_id += count % MAX_PARTICLES;			
				break;
			}
		}
	}
}











