#include "unit.h"
#include "timer.h"
#include "library.h"
#include "terrain.h"
#include "graphics.h"
#include "camera.h"
#include "console.h"
#include "variables.h"
#include "assert.h"


extern gamevars *vars;
extern Console *console;
extern Graphics *renderer;
extern Camera *camera;
extern Terrain *terrain;
extern Library *library;
extern Timer *timer;
extern EntityContainer *ents;

Unit::Unit() {
	model = NULL;
	state = Stopped;
	family = EF_UNIT;
	alive = true;
	frame = 0;
	position = Vector(0,0,0);
	ground_unit = true;
	blood.init(0.2f,1,position,Vector(0,0,0),Vector(0,0,0),Vector(0,0,0),Vector(1,0,0),20,2);
	blood.on = false;
	foot = 0;
	footprints = false;

	modelAnimations.fps = 15;
	modelAnimations.runStart = 1;
	modelAnimations.runEnd = 14;
	modelAnimations.walkStart = 1;
	modelAnimations.walkEnd = 14;
	modelAnimations.idleStart = 13;
	modelAnimations.idleEnd = 13;

	buildtime = 10;

	calibratedModelPosition = calibratedModelRotation = Vector(0,0,0);
}

Unit::~Unit(void)
{
}

void Unit::Think() { 
	char buffer[1024];
	sprintf(buffer,"$curID = %d;",id);
	console->RunLine(buffer);
	console->RunLine(script.c_str()); 
}


void Unit::SetScript(const char *szScript) {
	script = szScript;
}

bool Unit::LoadScript(const char *szFilename) {
	return console->LoadScript(szFilename);
}

void Unit::WalkTo(Vector t) {
	target = t;
	state = WalkToTarget;
}

void Unit::process() {
	// some sanity checks
	assert (this && terrain);

	blood.position = position;
	blood.Run();

	if (alive) {
		if (position.x < 0 || position.z < 0 || position.x > terrain->width || position.z > terrain->height) {
			// out of bounds! dirty traitor, trying to run away from the action!
			Kill();
			return;
		}

		if (footprints && (state == WalkToTarget || state == RunToTarget)) {
			if (timer->time - lastFootprintSpawn > 0.1f) {
				if ((++foot)%2 == 1) 
					particles->Spawn(1,Vector(0.5f,0.5f,0.5f),Vector(0.5f,0.5f,0.5f),position+Normalize(Vector(sin(atan2f(velocity.x,velocity.z)+45),0,cos(atan2f(velocity.x,velocity.z)+45)))*0.25f,Vector(0,0,0),Vector(0,0,0),Vector(0,0,0),5,0);
				else
					particles->Spawn(1,Vector(0.5f,0.5f,0.5f),Vector(0.5f,0.5f,0.5f),position-Normalize(Vector(sin(atan2f(velocity.x,velocity.z)+45),0,cos(atan2f(velocity.x,velocity.z)+45)))*0.25f,Vector(0,0,0),Vector(0,0,0),Vector(0,0,0),5,0);
				lastFootprintSpawn = timer->time;
			}
		}

		if (timer->time - lastRepositionTime > 2) {
			Entity *closest = ents->qtree.tree->getClosestEntity(position,-1,0,0,0);
			if (closest && (closest->position - position).len2() < 4) {
//				if (position == closest->position) position += Vector(0.01f,0,0);
				PathTo(position + (Normalize(position - closest->position)));
			}
			lastRepositionTime = timer->time;
		}

		// if the terrain we're standing in is marked as unwalkable - ie: we're in an invalid piece of terrain
		if (terrain->getContents((int)position.x,(int)position.z) & ((ground_unit)?TC_UNWALKABLE:0)) {
 			Entity *stuckIn = ents->qtree.tree->getClosestEntity(position,-1,0,0,0);
			if (stuckIn && dist2(stuckIn->position,position) < stuckIn->size.len2() + size.len2()) {	// if we're stuck in something
				position += Normalize(position - stuckIn->position) * timer->frameScalar;
			}
		}

		Vector p = position;
		p.y = 0;

		if (state == Stopped || ((p - target).len2() < size.len2())) {
			velocity.normalize();		
			state = Stopped;	
			ENT_TASK *newTask = GetTask();
			if (newTask) {
				switch (newTask->type) {
				case ENT_TASK::MOVE:
					WalkTo(newTask->position);
					break;
				}
			}
		}

		Entity *nearest;
		Vector adjust;

		switch (state) {
		case WalkToTarget:
			targetvelocity = Normalize(target - p) * walkspeed;
			velocity = velocity + (targetvelocity - velocity) * 0.15f;

			adjust = Vector(0,0,0);
			nearest = ents->qtree.tree->getClosestEntity(position,-1,EF_UNIT,true,team);
			if (nearest && dist2(position,nearest->position) <= size.len2()) 
				adjust = Normalize(nearest->position - position) * 0.25f * timer->frameScalar;

			position += (velocity + adjust) * timer->frameScalar;
			break;	

		case RunToTarget:
			velocity = Normalize(target - p) * runspeed;

			adjust = Vector(0,0,0);
			nearest = ents->qtree.tree->getClosestEntity(position,-1,EF_UNIT,true,team);
			if (nearest && dist2(position,nearest->position) <= size.len2()) 
				adjust = Normalize(nearest->position - position) * 0.25f * timer->frameScalar;

			position += (velocity + adjust) * timer->frameScalar;
			break;

		case FlankTargetLeft:
			velocity = (Vector(0,1,0) ^ Normalize(target - p)) * runspeed;
			position += velocity * timer->frameScalar;
			break;

		case FlankTargetRight:
			velocity = (Vector(0,1,0) ^ Normalize(target - p)) * -runspeed;
			position += velocity * timer->frameScalar;
			break;

		case Stopped:
			nearest = ents->qtree.tree->getClosestEntity(position,-1,EF_UNIT,true,team);
			if (nearest && dist2(position,nearest->position) <= size.len2()) 
				position -= Vector((float)(rand()%10) / 100,(float)(rand()%10) / 100,(float)(rand()%10) / 100) + Normalize(nearest->position - position) * timer->frameScalar;
			break;
		} 


		if (ground_unit) {
			position.y = terrain->getInterpolatedHeight(position.x,position.z);
		}

		if (model) {
			model->setPosition(calibratedModelPosition + position);		
			model->setRotation(calibratedModelRotation + Vector(0,0,atan2(velocity.x,velocity.z)/(3.1415f/180)));
		}

		if (script.length() > 0) {
			console->RunLine(script.c_str());
		}

		Think();
	} else {
		blood.on = true;
		state = Stopped;
		if (timer->time - deathTime > 60)
			model->setPosition(position+Vector(0,-1*(timer->time - deathTime),0));		
		else
			model->setPosition(position);		
		model->setRotation(Vector(90,0,atan2(velocity.x,velocity.z)/(3.1415f/180)));
		if (timer->time - deathTime > 60)
		{
			position.y -= 0.01f;	
			if (timer->time - deathTime > 120) {
				try {				
					Remove();
				} catch (...) {
					// FFFFFuccccccccccckkkkkkkkk... delete failed.... umm do something!!!
					console->Print("Yar, thar be holes in them thar code!");
				}
			}
		}
	}
}

void Unit::render() {
	if (camera->frustum.pointInFrustum(position)) {
		model->setScale(scale);
		glBindTexture(GL_TEXTURE_2D,texture);
		glColor3f(1,1,1);

		int animationStart = 0;
		int animationEnd = 0;

		switch (state) {
			case WalkToTarget:
				animationStart = modelAnimations.walkStart;
				animationEnd = modelAnimations.walkEnd;
				break;
			case RunToTarget:
				animationStart = modelAnimations.runStart;
				animationEnd = modelAnimations.runEnd;
				break;

			case Stopped:
			default:
				animationStart = modelAnimations.idleStart;
				animationEnd = modelAnimations.idleEnd;
				break;
		}

		if (alive) {
			frame += modelAnimations.fps*timer->frameDifference;
			if ((frame > animationEnd) || (frame < animationStart)) frame = (float)animationStart;
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glDisable(GL_DEPTH_TEST);
			glLineWidth(5);
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_LINES);
				glColor3f(1,0,0);
				glVertex3f(position.x,position.y+size.y,position.z);
				glVertex3f(position.x,position.y+size.y + 1,position.z);
			
				glColor3f(0,1,0);
				glVertex3f(position.x,position.y+size.y,position.z);
				glVertex3f(position.x,position.y+size.y + (((float)health/100)),position.z);
			glEnd();
			glPopAttrib();
		}

		model->drawObjectFrame(frame,model->kDrawImmediate);
	}

	renderToolTip();

	if (type != E_SHEEP && vars->getIntValue("unit_showpathfind") == 1 && taskQueue.size() > 0) {

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPointSize(4);
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBegin(GL_POINTS);
		for (std::list<ENT_TASK*>::iterator iter = taskQueue.begin(); iter != taskQueue.end(); iter++) {
			ENT_TASK *This = *iter;
			glColor3f(1,0,0);
			glVertex3f(This->position.x,terrain->getInterpolatedHeight(This->position.x,This->position.z)+1, This->position.z);
		}
		glEnd();
		glPopAttrib();
	}
}

void Unit::interact(Entity *source, int type) {
	switch (type) {
	case EI_ATTACK:
		particles->Spawn(20,Vector(1,0,0),Vector(1,0,0),position,Vector(0,0,0),Vector(0,2,0),Vector(2,1,2),10,0);
		health -= source->damage;
		if (health <= 0) {
			onDeath();
			Kill();
		} else {
			onAttacked(source);
		}

		break;
	}

}

void Unit::SetModel(const char *modelname, const char *texturename) {
	if (model) delete model;
	model = new Md2Object;
	model->setModel((Model_MD2*)library->Export(modelname));
	texture = renderer->LoadTexture(texturename);
}
