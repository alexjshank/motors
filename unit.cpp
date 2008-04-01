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

void Unit::Construct() {
	model = NULL;
	menu = 0;
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
	completed = 0;

	size=Vector(1,2,1);
	runspeed = 10.0f;
	walkspeed = 8.5f;
	scale = 0.05f;
	
	calibratedModelPosition.y = 1;
	calibratedModelRotation.z = -90;

	modelAnimations.fps = 15;
	modelAnimations.walkStart = 40;
	modelAnimations.walkEnd = 46;
	modelAnimations.runStart = 40;
	modelAnimations.runEnd = 46;
	modelAnimations.idleStart = 13;
	modelAnimations.idleEnd = 13;

	buildtime = 10;
	lastThinkTime = -100;	// think asap
	updateInterval = 1;

	calibratedModelPosition = calibratedModelRotation = Vector(0,0,0);
}

Unit::Unit() {
	Construct();
}

Unit::~Unit(void)
{
}


Unit::Unit(const char *cn) {
	Construct();

	Initialize(cn);
}


Unit::Unit(const char *cn, const char *strmodelname, const char *texturename)  {
	Construct();
	
	Initialize(cn,strmodelname,texturename);
}

void Unit::Initialize(const char *cn, const char *strmodelname, const char *texturename) {
	classname = cn;

	tooltip.enabled = true;
	tooltip.tooltip = cn;

	console->LoadScriptf("data/scripts/%s.py",cn);

	if (texturename && strmodelname) {
		std::string modelname = "data/models/", texture = "data/models/";
		modelname += strmodelname;
		texture += texturename;
		SetModel(modelname.c_str(),texture.c_str());
	}
}


void Unit::init() {
	console->RunLinef("%s_onInit(%d)\n",classname.c_str(), id);
}

void Unit::WalkTo(Vector t) {
	target = t;
	state = WalkToTarget;
}

void Unit::process() {
//	assert (this && terrain);

	blood.position = position;
	blood.Run();

	if (Attached && AttachedTo) {
		position = AttachedTo->position;
	}

	if (alive) {
		if (terrain && (position.x < 0 || position.z < 0 || position.x > terrain->width || position.z > terrain->height)) {
			// out of bounds! dirty traitor, trying to run away from the action!
			Kill();
			Remove();
			return;
		}

		if (footprints && (state == WalkToTarget || state == RunToTarget)) {
			if (timer->time - lastFootprintSpawn > 0.25f) {
				if ((++foot)%2 == 1) 
					particles->Spawn(1,Vector(0.5f,0.5f,0.5f),Vector(0.5f,0.5f,0.5f),position+Normalize(Vector(sin(atan2f(velocity.x,velocity.z)+45),0,cos(atan2f(velocity.x,velocity.z)+45)))*0.25f,Vector(0,0,0),Vector(0,0,0),Vector(0,0,0),25,0);
				else
					particles->Spawn(1,Vector(0.5f,0.5f,0.5f),Vector(0.5f,0.5f,0.5f),position-Normalize(Vector(sin(atan2f(velocity.x,velocity.z)+45),0,cos(atan2f(velocity.x,velocity.z)+45)))*0.25f,Vector(0,0,0),Vector(0,0,0),Vector(0,0,0),25,0);
				lastFootprintSpawn = timer->time;
			}
		}

		// if the terrain we're standing in is marked as unwalkable - ie: we're in an invalid piece of terrain (e.g. water, or the ground under a building)
		if (terrain && terrain->getContents((int)position.x,(int)position.z) & ((ground_unit)?TC_UNWALKABLE:0)) {
 			Entity *stuckIn = ents->qtree.tree->getClosestEntity(position,-1,EF_BUILDING,0,0);
			if (stuckIn && dist2(stuckIn->position,position) < (stuckIn->size.len2() + size.len2())/2) {	// if we're stuck in something
				position += Normalize(position - stuckIn->position) * walkspeed * timer->frameScalar;
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
				case ENT_TASK::SCRIPTEDACTION:
					console->RunLinef("curID = %d\n%s\n",id,newTask->
						scriptedAction.c_str());
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

			if (timer->time - lastRepositionTime > 0.25f) {
				lastRepositionTime = timer->time;			
				nearest = ents->qtree.tree->getClosestEntity(position,-1,EF_UNIT,true,team);
				if (nearest && dist2(position,nearest->position) <= size.len2()) 
					adjust = Normalize(nearest->position - position) * 0.25f * timer->frameScalar;
			}
			position += (velocity + adjust) * timer->frameScalar;
			break;	

		case RunToTarget:
			velocity = Normalize(target - p) * runspeed;

			adjust = Vector(0,0,0);
			if (timer->time - lastRepositionTime > 0.25f) {
				lastRepositionTime = timer->time;			
				nearest = ents->qtree.tree->getClosestEntity(position,-1,EF_UNIT,true,team);
				if (nearest && dist2(position,nearest->position) <= size.len2()) 
					adjust = Normalize(nearest->position - position) * 0.25f * timer->frameScalar;
			}
			position += (velocity + adjust) * timer->frameScalar;
			break;

		case Stopped:
			nearest = ents->qtree.tree->getClosestEntity(position,-1,EF_UNIT,true,team);
			if (nearest && dist2(position,nearest->position) <= size.len2()) {
				if (nearest->position == position) position += Vector(0.01f,0,0);			
				position -= Normalize(nearest->position - position) * walkspeed * timer->frameScalar;
			}
			break;
		} 


		if (ground_unit) {
			position.y = terrain->getInterpolatedHeight(position.x,position.z);
		}

		if (model) {
			model->setPosition(calibratedModelPosition + position);		
			if (velocity.len2() > 0.5f)
				model->setRotation(calibratedModelRotation + Vector(0,0,atan2(velocity.x,velocity.z)/(3.1415f/180)));
		}

		if (timer->time - lastThinkTime > updateInterval) {
			lastThinkTime = timer->time;
			Think();
		}
	} else {	// dead
		blood.on = true;
		state = Stopped;

		model->setPosition(position+Vector(0,-0.01f * (timer->time - deathTime),0));		
		model->setRotation(Vector(90,0,atan2(velocity.x,velocity.z)/(3.1415f/180)));

		if (timer->time - deathTime > 60) {
			try {				
				Remove();
			} catch (...) {
				console->Print("Yar, thar be holes in them thar code!");
			}
		}
	}
}

void Unit::render() {
	if (camera->frustum.pointInFrustum(position)) {
		if (model) {
			if (this->completed < 100) {
//				glBindTexture(GL_TEXTURE_2D,buildTexture);
				DrawCube(position+Vector(0,100-completed,0),Vector(size.x,(size.x+size.z)/2,size.z));
			}

			if (completed < 1 || completed > 99) {
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
				if (completed < 100) {
					glColor4f(1,1,1,0.4f);
					glEnable(GL_BLEND);
				}
				model->drawObjectFrame(frame,model->kDrawImmediate);
				glDisable(GL_BLEND);
			}
		}
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

void Unit::Think() { 
	console->RunLinef("%s_onThink(%d)\n",classname.c_str(), id);
}

void Unit::onAttacked(Entity *source) {
	console->RunLinef("%s_onAttacked(%d,%d)\n",classname.c_str(), id, source->id);
}

void Unit::onDeath() {
	console->RunLinef("%s_onDeath(%d)\n",classname.c_str(), id);
}

void Unit::onSelected() {
	console->RunLinef("%s_onSelected(%d)\n",classname.c_str(), id);
}

void Unit::onUnSelected() {
	console->RunLinef("%s_onUnselected(%d)\n",classname.c_str(), id);
}
