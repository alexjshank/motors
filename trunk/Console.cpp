#include ".\console.h"
#include "graphics.h"
#include "input.h"
#include "printer.h"
#include <python.h>
#include "entity.h"
#include "unit.h"
#include "Camera.h"
#include "variables.h"
#include "building.h"
#include "LassoSelector.h"

extern gamevars *vars;
extern Graphics *renderer;
extern EntityContainer *ents;
extern Input *input;
extern Camera *camera;
extern LassoSelector *selector;

Console::Console(void)
{
	enabled = true;
	toggled = false;
}

Console::~Console(void)
{
	Py_Finalize();
}



extern Console *console;



// gprint('hello world')
static PyObject * game_print(PyObject *self, PyObject *args) {
    const char *value;

    if (!PyArg_ParseTuple(args, "s", &value))
        return NULL;

	console->Print(value);

	return NULL;
}

bool PlaceEntity(Entity *ent);

// spawn('peasant')
extern int currentID;
static PyObject * game_spawn(PyObject *self, PyObject *args) {
    char *entityName;
	int x,y;

    if (!PyArg_ParseTuple(args, "s", &entityName,&x,&y))
        return NULL;

	Entity *ent = SpawnEntity(entityName, camera->GetPosition().flat() + Vector(0,terrain->getInterpolatedHeight(camera->GetPosition().x,camera->GetPosition().z),0) );
	
	if (!ent) return NULL;

	if (ent->family == EF_UNIT) {
		if (currentID >= 0 && currentID < ents->entities.size()) {
			Entity *parent = ents->entities[currentID];
			if (parent) {
					ent->position = parent->position - parent->size.flat();
			}
		}
	} else {
		PlaceEntity(ent);
		if (input->inputContext != EditMode)
			input->inputContext = NormalInput;
	} 


	return NULL;
}

static PyObject * game_QueueUnitForBuild(PyObject *self, PyObject *args) {
    char *entityName;

    if (!PyArg_ParseTuple(args, "s", &entityName))
        return NULL;

	if (currentID >= 0 && currentID < ents->entities.size()) {
		Entity *parent = ents->entities[currentID];
		if (parent) {
			((Building*)parent)->spawnQueue.push(entityName);
		}
	}
	
	return NULL;
}

static PyObject * game_killSelected(PyObject *self, PyObject *args) {

	for (int i=0;i<(int)selector->SelectedEntities.size();i++) {
		Entity *SelectedEntity = selector->SelectedEntities[i];
		SelectedEntity->Kill();
	}

	return NULL;
}

void SaveWorldState(const char *filename);
// save('savename')
static PyObject * game_save(PyObject *self, PyObject *args) {
    const char *lpstrSavename;
	std::string savename;

    if (!PyArg_ParseTuple(args, "s", &lpstrSavename))
        return NULL;
    savename="data/maps/";
	savename += lpstrSavename;

	std::string s = "saving world as '";
	s += savename;
	console->Print(s.c_str());

	SaveWorldState(savename.c_str());

	console->Print("Saved.");
	return NULL;
}


static PyObject * game_script_getposition(PyObject *self, PyObject *args) {
    DWORD value;
 
    if (!PyArg_ParseTuple(args, "i", &value))
        return NULL;
 
      if (value && ents->entities[value] && ents->entities[value]->alive) {
            return PyComplex_FromDoubles(ents->entities[value]->position.x,ents->entities[value]->position.z);
      }
 
      return PyComplex_FromDoubles(0,0);
}
 
static PyObject * game_script_getstate(PyObject *self, PyObject *args) {
    DWORD value;
 
    if (!PyArg_ParseTuple(args, "i", &value))
        return NULL;

    if (value && ents->entities[value] && ents->entities[value]->alive) {
        return PyInt_FromLong(ents->entities[value]->state);
    }
	return NULL;
}
 
static PyObject * game_script_setstate(PyObject *self, PyObject *args) {
    DWORD value,state;
 
    if (!PyArg_ParseTuple(args, "ii", &value, &state))
        return NULL;
 
      if (value && ents->entities[value] && ents->entities[value]->alive) {
            ents->entities[value]->state = (ENT_STATES)state;
      }
      return NULL;
}
 
static PyObject * game_script_pathto(PyObject *self, PyObject *args) {
    DWORD value;
      DWORD x,y;
 
    if (!PyArg_ParseTuple(args, "iii", &value,&x,&y))
        return NULL;
 
      if (value && ents->entities[value] && ents->entities[value]->alive) {
            ents->entities[value]->PathTo(Vector((float)x,0,(float)y));
      }
      return NULL;
}
 
static PyObject * game_script_pathtoent(PyObject *self, PyObject *args) {
    DWORD value;
      DWORD target;
 
    if (!PyArg_ParseTuple(args, "ii", &value,&target))        return NULL;
 
      if (value && target && ents->entities[value] && ents->entities[target] && ents->entities[value]->alive && ents->entities[target]->alive) {
            ents->entities[value]->PathTo(ents->entities[target]->position);
      }
      return NULL;
}
 
static PyObject * game_script_getnearestentity(PyObject *self, PyObject *args) {
    DWORD value;
      DWORD type,family,team;
 
    if (!PyArg_ParseTuple(args, "iiii", &value,&type,&family,&team))
        return NULL;
 
      if (value && ents->entities[value] && ents->entities[value]->alive) {
            Entity *entity = ents->qtree.tree->getClosestEntity(ents->entities[value]->position,type,family,true,team);
            if (entity) {
                  return PyInt_FromLong(entity->id);
            }
      } else {
            console->RunLine("echo('invalid entity id');");
      }
      return PyInt_FromLong(0);
}
 
static PyMethodDef GameMethods[] = {
    {"echo",  game_print, METH_VARARGS, "print a string to the console."},
      {"spawn",   game_spawn, METH_VARARGS, "spawn an entity into the game."},
      {"save",    game_save, METH_VARARGS,"Save the game"},
 
      {"getPosition", game_script_getposition, METH_VARARGS, "returns the position of a unit"},
      {"setState", game_script_setstate, METH_VARARGS, "set the current state of an entity"},
      {"getState", game_script_getstate, METH_VARARGS, "get the current state of an entity"},
     
      {"getNearestEntity", game_script_getnearestentity, METH_VARARGS, "used to access other entities"},
 
      {"PathTo", game_script_pathto, METH_VARARGS,"issues commands to a unit to pathfind a route to a location and then go there"},
      {"PathToEnt", game_script_pathtoent, METH_VARARGS, "issues commands to a unit to pathfind a route to a location and then go there"},

      {NULL, NULL, 0, NULL}        /* Sentinel */
};

 


bool Console::init() {
	Py_Initialize();
	Py_InitModule("__main__", GameMethods);

	PyRun_SimpleString("echo ('Embedded Python console initialization complete - Alex shank - 2008 - alexjshank@gmail.com')");

	return true;
}

void Console::run() {
	if (input->GetKeyPressed('`')) { Toggle(); }

	if (enabled && toggled) {
		int s=(int)history.size();

		glPushMatrix();
			glLoadIdentity();
			glBegin(GL_QUADS);
				glColor3f(0,0,0);
				glVertex3f(-1,1,-1.2f);
				glVertex3f(-1,0,-1.2f);
				glVertex3f( 1,0,-1.2f);
				glVertex3f( 1,1,-1.2f);
			glEnd();
		glPopMatrix();

		glColor3f(1,1,1);
		float p=14;
		if (s > 0) {
			for (int y=s-1; y>=0 && (p > 0); y--) {
				const char *line = history[y].c_str();
				if (history[y].size() > 0) {
					p -= 0.5f;
					renderer->text.print(0,p,"%s",line);
				}
			}
		}

		renderer->text.print(0,14,"] %s",currentline.c_str());
	
		if (input->GetKeyPressed(SDLK_RETURN)) { 
			ProcessLine(); 
		} else {
			for (int key=0;key<254;key++) {
				if ( key!='`' && (isalnum((unsigned char)key) || ispunct((unsigned char)key) || key==' ' || key==8) && input->GetKeyPressed(key)) {
					ProcessInput((char)key);
				}
			}
		}
	}

}

void Console::ProcessInput(char input) {
	if (input == 8) {	// backspace
		if (currentline.length() > 0)
			currentline = currentline.substr(0,currentline.length()-1);
	} else {
		currentline += input;
	}
}

void Console::ProcessLine() {
	RunLine(currentline.c_str());

	history.push_back(currentline);
	currentline = "";
}

void Console::RunLine(const char *line) {

	printf("%s",line);
	PyRun_SimpleString(line);

}

void Console::RunLinef(const char *format, ...) { 
	char text[1024];
	va_list args;

	if (!format) return;

	va_start(args,format);
		vsprintf(text,format,args);
	va_end(args);

	RunLine(text);
}

void Console::Print(const char *line) {
	history.push_back(line);
}

void Console::Printf(const char *format, ...) { 
	char text[1024];
	va_list args;

	if (!format) return;

	va_start(args,format);
		vsprintf(text,format,args);
	va_end(args);

	Print(text);
}

bool Console::LoadScript(const char *scriptFilename) {
	char buffer[8192];
	FILE *fin = fopen(scriptFilename,"r");
	if (fin) {
		while (!feof(fin)) {
			ZeroMemory(buffer,8192);
			fgets(buffer,8192,fin);
			strcat(buffer,"\n");
			RunLine(buffer);
		}
		return true;
	}
	return false;
}