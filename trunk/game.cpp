#include "Variables.h"
#include "Task.h"
#include "Timer.h"
#include "Graphics.h"
#include "Input.h"
#include "Camera.h"
#include "UI.h"
#include "console.h"
#include "Entity.h"
#include "Terrain.h"
#include "save.h"
#include "md2.h"
#include "ParticleManager.h"
#include "LassoSelector.h"

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#include "peasant.h"
#include "farm.h"
#include "tower.h"
#include "lumbermill.h"
#include "lumbertree.h"
#include "soldier.h"
#include "rigidbody.h"

bool active = true;
Kernel tasks;
gamevars *vars;
Timer *timer;
Input *input;
Graphics *renderer;
UI *ui;
Camera *camera;
Library *library;
EntityContainer *ents;
ParticleManager *particles;
Terrain *terrain;
Console *console;
LassoSelector *selector;

Entity *entControlling;

int modelsRenderedThisFrame;
int currentID = 0;
bool placingEntity = false;
bool rotatingEntity = false;
Entity *entityToPlace=NULL;
float beginPlacingTime = 0;

bool PlaceEntity(Entity *ent) { 
	if (!ent) return false;

	entityToPlace = ent;
	placingEntity = true;
	beginPlacingTime = timer->time;
	return true;
}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	srand(timeGetTime());
	AllocConsole();
	freopen("CONOUT$", "wb", stdout);

	vars = new gamevars;
	vars->loadCfgFile("config.cfg");

	timer = new Timer;
	tasks.AddTask(timer);

	input = new Input;
	tasks.AddTask(input);

	renderer = new Graphics;
	tasks.AddTask(renderer);

	camera = new Camera;
	tasks.AddTask(camera);
	camera->SetPosition(Vector(122, 30, 160));
	camera->LookAt(Vector(122, 5, 188));
	camera->StopFollowing();

	library = new Library;

	ents = new EntityContainer;
	tasks.AddTask(ents);

	particles = new ParticleManager;
	tasks.AddTask(particles);
	
	ui = new UI;
	tasks.AddTask(ui);

	selector = new LassoSelector;
	tasks.AddTask(selector);

	console = new Console;
	console->enabled = true;
	tasks.AddTask(console);

	tasks.Init();
	tasks.Run();	

	int loadingTexture = renderer->LoadTexture("data/loadingTexture.bmp");

	UIWindow loadingScreen;
	glPushMatrix();
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);
		loadingScreen.renderTexture(Vector(0,0,0),Vector((float)vars->getIntValue("screen_width"),(float)vars->getIntValue("screen_height"),0),loadingTexture);
	glPopMatrix();


	renderer->run();
	input->mouseAbsolute = Vector(100,100, 100);
	input->mouseMovement = Vector(0,0,0);


	library->Import("data/topographical/PirateIsland.bmp",0);
	library->Import("data/models/house.BMP",0);
	library->Import("data/models/Sheep.BMP",0);
	library->Import("data/models/soldier.bmp",0);
	library->Import("data/models/barracks.bmp",0);

	library->Import("data/models/farm.md2",1);
	library->Import("data/models/tower/tower.md2",1);
	library->Import("data/models/mill.md2",1);
	library->Import("data/models/dock.md2",1);
	library->Import("data/models/lightpost.md2",1);
	library->Import("data/models/barracks.md2",1);

	library->Import("data/models/peasant.md2",1);
	library->Import("data/models/villagewoman.md2",1);
	library->Import("data/models/sheep.md2",1);
	library->Import("data/models/vulture.md2",1);
	library->Import("data/models/soldier.md2",1);
	library->Import("data/models/ship.md2",1);

	library->Import("data/models/plant2.md2",1);
	library->Import("data/models/felwoodbush1.md2",1);

	UIWindow *waypointEditor = (UIWindow*)ui->CreateFromFile("data/ui/waypointeditor.ui");
	waypointEditor->visible = false;

    bool camerafollowing = false;
	input->mouseAbsolute = Vector(100,100, 100);
	input->mouseMovement = Vector(0,0,0);
	timer->frameScalar = 0.0001f;
	


	LoadWorldState(vars->getValue("default_map")->value.c_str());

	Unit *unit = new Unit();
	unit->classname = "Custom";
	unit->health = 100;
	unit->updateInterval = 1;	// update once per second
	unit->Scripts.onThink = "echo(str(curID))";

	ents->AddEntity((Entity *)unit);


	while (active) {
		tasks.Run();			
		
		/*
		int e = glGetError();
		if (e) {
			char t[255];
			sprintf(t,"Rendering error code: %d (0x%x)",e,e);
			console->Print(t);
		} 
		*/

		// TODO: make key binding system to replace hard-coded tests
			
		if (console->toggled) {
			input->inputContext = ConsoleInput;
		} else if (input->inputContext == ConsoleInput) {
			input->inputContext = NormalInput;
		}

		if (input->GetKeyDown(SDLK_ESCAPE)) {
			active = false;
		}
		static float moveSpeed = vars->getFloatValue("cameraSpeed");

		switch (input->inputContext) {
		case NormalInput: {
			camera->StopFollowing();

			selector->enabled = true;
			selector->maxSelectionCount = 8;
			if (input->GetKeyDown(SDLK_LEFT) || input->GetKeyDown('a') || (input->mouseAbsolute.x <= 30)) {
				camera->MoveRelative(Vector(-moveSpeed*input->mouseAbsolute.z,0,0)*timer->frameScalar);
			}
			if (input->GetKeyDown(SDLK_RIGHT) || input->GetKeyDown('d') || (input->mouseAbsolute.x >= 770)) {
				camera->MoveRelative(Vector(moveSpeed*input->mouseAbsolute.z,0,0)*timer->frameScalar);
			}
			if (input->GetKeyDown(SDLK_UP) || input->GetKeyDown('w') || (input->mouseAbsolute.y <= 30)) {
				camera->MoveRelative(Vector(0,0,moveSpeed*input->mouseAbsolute.z)*timer->frameScalar);
			}
			if (input->GetKeyDown(SDLK_DOWN) || input->GetKeyDown('s') || (input->mouseAbsolute.y >= 570)) {
				camera->MoveRelative(Vector(0,0,-moveSpeed*input->mouseAbsolute.z)*timer->frameScalar);
			}
			if (input->GetKeyReleased(SDLK_F1)) {
				input->inputContext = EditMode;
			}


			if (entityToPlace) {
				if (placingEntity) {
					entityToPlace->completed = 0;
					entityToPlace->position = selector->gridAlignedLassoPosition;
				} else if (rotatingEntity) {
					entityToPlace->completed = 0;
					entityToPlace->rotation.z += input->mouseMovement.x;
				}
			}
			if (input->GetMButtonPushed(1)) {							// initial push
				if (entityToPlace && timer->time - beginPlacingTime > 1) {
					if (placingEntity) {
						placingEntity = false;
						rotatingEntity = true;
						beginPlacingTime = timer->time;
					} else if (rotatingEntity) {
						rotatingEntity = false;
						entityToPlace = 0;
					}
				}
			}


			if (input->GetMButtonReleased(3)) {
				// right-click:
				if (selector->SelectedEntities.size() > 0) {
					for (int i=0;i<(int)selector->SelectedEntities.size();i++) {
						Entity *SelectedEntity = selector->SelectedEntities[i];
						if (SelectedEntity->family == EF_UNIT) {	// anything that can move here...
							// if we aren't adding to the task queue, we have to clear it first.
							if (!input->GetKeyState(SDLK_RSHIFT) && !input->GetKeyState(SDLK_LSHIFT)) {
								SelectedEntity->ClearTasks();
							}
							SelectedEntity->PathTo(selector->gridAlignedLassoPosition);
						}
					}
				}
			}
			break;	
		}
		case BuildMenu: 
		case PersonelMenu: 
			selector->enabled = false;
			break;

		case EditMode:
			camera->StopFollowing();
			waypointEditor->visible = true;
			selector->enabled = true;
			selector->maxSelectionCount = 1;

			renderer->text.print(5,5,"---EDIT MODE---");

			if (input->GetKeyDown(SDLK_LEFT)) {
				camera->MoveRelative(Vector(-moveSpeed,0,0)*timer->frameScalar);
			}
			if (input->GetKeyDown(SDLK_RIGHT)) {
				camera->MoveRelative(Vector(moveSpeed,0,0)*timer->frameScalar);
			}
			if (input->GetKeyDown(SDLK_UP)) {
				camera->MoveRelative(Vector(0,0,moveSpeed)*timer->frameScalar);
			}
			if (input->GetKeyDown(SDLK_DOWN)) {
				camera->MoveRelative(Vector(0,0,-moveSpeed)*timer->frameScalar);
			}
			if (input->GetKeyReleased(SDLK_F1)) {
				input->inputContext = NormalInput;
				waypointEditor->visible = false;
			}


			if (entityToPlace) {
				if (placingEntity) {
					entityToPlace->position = selector->gridAlignedLassoPosition;
				} else if (rotatingEntity) {
					entityToPlace->rotation.z += input->mouseMovement.x;
				}
			}

			if (input->GetMButtonPushed(1)) {							// initial push
				if (entityToPlace && timer->time - beginPlacingTime > 1) {
					if (placingEntity) {
						placingEntity = false;
						rotatingEntity = true;
						beginPlacingTime = timer->time;
					} else if (rotatingEntity) {
						rotatingEntity = false;
						entityToPlace = 0;
					}
				}
			}

			if (input->GetMButtonReleased(3)) {
				// right-click:
				if (selector->SelectedEntities.size() > 0) {
					Entity *SelectedEntity = selector->SelectedEntities[0];
					PlaceEntity(SelectedEntity);
				}
			}
			break;
		case Sailing:
			if (!entControlling) {
				input->inputContext = NormalInput;
				break;
			}


			selector->enabled = true;
			if ((input->mouseAbsolute.x <= 30)) {
				camera->MoveRelative(Vector(-moveSpeed,0,0)*timer->frameScalar);
			}
			if ((input->mouseAbsolute.x >= 770)) {
				camera->MoveRelative(Vector(moveSpeed,0,0)*timer->frameScalar);
			}
			if ((input->mouseAbsolute.y <= 30)) {
				camera->MoveRelative(Vector(0,0,moveSpeed)*timer->frameScalar);
			}
			if ((input->mouseAbsolute.y >= 570)) {
				camera->MoveRelative(Vector(0,0,-moveSpeed)*timer->frameScalar);
			}

			if (input->GetKeyDown(SDLK_LEFT) || input->GetKeyDown('a')) {				
				entControlling->velocity -= (((entControlling->velocity ^ Vector(0,1,0)) + entControlling->velocity)/2)*timer->frameScalar;
			}
			if (input->GetKeyDown(SDLK_RIGHT) || input->GetKeyDown('d')) {				
				entControlling->velocity += (((entControlling->velocity ^ Vector(0,1,0)) + entControlling->velocity)/2)*timer->frameScalar;
			}
			if (input->GetKeyDown(SDLK_UP) || input->GetKeyDown('w')) {
				entControlling->velocity += (Normalize(camera->GetView().flat())) * timer->frameScalar;
			}
			if (input->GetKeyDown(SDLK_DOWN) || input->GetKeyDown('s')) {
				entControlling->velocity -= entControlling->velocity * timer->frameScalar;
			}

			camera->Follow(&entControlling->position,2);

			break;
		}


		if (input->mouseAbsolute.z < 1) input->mouseAbsolute.z = 1;
		if (input->mouseAbsolute.z > 13) input->mouseAbsolute.z  = 13;

		Vector cp = camera->GetPosition();
		camera->SetPosition(Vector(cp.x,15,cp.z));
		camera->SetZoom(input->mouseAbsolute.z * input->mouseAbsolute.z);

		if (input->GetKeyDown(SDLK_LCTRL) || input->GetKeyDown(SDLK_RCTRL) || input->GetMButtonState(2)) {
			camera->RotateView(input->mouseMovement.y/2,input->mouseMovement.x/2,0);
			if (camera->GetRotation().x < -20) camera->RotateView(-camera->GetRotation().x,0,0);
//			camera->MoveRelative(Vector(-input->mouseMovement.x/4,0,0));
		}	
	}

	tasks.Stop();
	return 0;
}