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

#include <sstream>

extern gamevars *vars;
extern Graphics *renderer;
extern EntityContainer *ents;
extern Input *input;
extern Camera *camera;
extern LassoSelector *selector;

#define PYTHONMODULE(varname) static PyMethodDef varname[] = {
#define ENDPYTHONMOD {0,0,0,0} };
#define SCRIPTFUNC(name) static PyObject* name(PyObject *self, PyObject *args)
#define pydef(a,b)  {a, b, METH_VARARGS, ""},



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
SCRIPTFUNC(game_print) {
    const char *value;

    if (!PyArg_ParseTuple(args, "s", &value))
        return NULL;

	console->Print(value);

	Py_RETURN_NONE;
}

bool PlaceEntity(Entity *ent);

// spawn('peasant')
extern int currentID;
SCRIPTFUNC(game_spawn) {
    char *entityName;
	int x,y;

    if (!PyArg_ParseTuple(args, "s", &entityName,&x,&y))
        return NULL;

	Entity *ent = SpawnEntity(entityName, camera->GetPosition().flat() + Vector(0,terrain->getInterpolatedHeight(camera->GetPosition().x,camera->GetPosition().z),0) );
	
	if (!ent) return NULL;

	if (ent->family == EF_UNIT) {
		if (currentID >= 0 && currentID < (int)ents->entities.size()) {
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


	Py_RETURN_NONE;
}

SCRIPTFUNC(game_QueueUnitForBuild) {
    char *entityName;

    if (!PyArg_ParseTuple(args, "s", &entityName))
        return NULL;

	if (currentID >= 0 && currentID < (int)ents->entities.size()) {
		Entity *parent = ents->entities[currentID];
		if (parent) {
			((Building*)parent)->spawnQueue.push(entityName);
		}
	}
	
	Py_RETURN_NONE;
}

SCRIPTFUNC(game_killSelected) {

	for (int i=0;i<(int)selector->SelectedEntities.size();i++) {
		Entity *SelectedEntity = selector->SelectedEntities[i];
		SelectedEntity->Kill();
	}

	Py_RETURN_NONE;
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
	Py_RETURN_NONE;
}

SCRIPTFUNC(game_script_setposition) {
    DWORD value;
	DWORD loc;
 
    if (!PyArg_ParseTuple(args, "ii", &value,&loc))
        return NULL;

    if (value && ents->entities[value] && loc && ents->entities[loc]) {
        ents->entities[value]->position = ents->entities[loc]->position;
		Py_RETURN_NONE;
    }

	return NULL;
}

SCRIPTFUNC(game_script_getposition) {
    DWORD value;
 
    if (!PyArg_ParseTuple(args, "i", &value))
        return NULL;
 
      if (value && ents->entities[value] && ents->entities[value]->alive) {
            return PyComplex_FromDoubles(ents->entities[value]->position.x,ents->entities[value]->position.z);
      }
 
      return NULL;
}
 
SCRIPTFUNC(game_script_getstate) {
    DWORD value;
 
    if (!PyArg_ParseTuple(args, "i", &value))
        return NULL;

    if (value && ents->entities[value] && ents->entities[value]->alive) {
        return PyInt_FromLong(ents->entities[value]->state);
    }
	Py_RETURN_NONE;
}
 
SCRIPTFUNC(game_script_setstate) {
    DWORD value,state;
 
    if (!PyArg_ParseTuple(args, "ii", &value, &state))
        return NULL;
 
      if (value && ents->entities[value] && ents->entities[value]->alive) {
            ents->entities[value]->state = (ENT_STATES)state;
      }
	Py_RETURN_NONE;
}
 
SCRIPTFUNC(game_script_pathto) {
    DWORD value;
    DWORD x,y;

	if (!PyArg_ParseTuple(args, "iii", &value,&x,&y))
		return NULL;

    if (value && ents->entities[value] && ents->entities[value]->alive) {
        ents->entities[value]->PathTo(Vector((float)x,0,(float)y));
    }
	Py_RETURN_NONE;
}
 
SCRIPTFUNC(game_script_pathtoent) {
	DWORD value;
	DWORD target;

	if (!PyArg_ParseTuple(args, "ii", &value,&target))   
		return NULL;

	if (value && target && ents->entities[value] && ents->entities[target] && ents->entities[value]->alive && ents->entities[target]->alive) {
		ents->entities[value]->PathTo(ents->entities[target]->position);
	}
	Py_RETURN_NONE;
}
 
SCRIPTFUNC(game_script_getnearestentity) {
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

SCRIPTFUNC(game_script_GetRegs) {
	DWORD value;
	int ria,rib,ric,rid,rie,rif,rig,rih;
	float rfa,rfb,rfc,rfd,rfe,rff,rfg,rfh;

    if (!PyArg_ParseTuple(args, "iiiiiiiiiffffffff", &value,  &ria,&rib,&ric,&rid,&rie,&rif,&rig,&rih, &rfa,&rfb,&rfc,&rfd,&rfe,&rff,&rfg,&rfh))
        return NULL;

    if (value && ents->entities[value]) {
		if (ents->entities[value]->family == EF_UNIT) {
			((Unit*)ents->entities[value])->ScriptRegisters.ria = ria;
			((Unit*)ents->entities[value])->ScriptRegisters.rib = rib;
			((Unit*)ents->entities[value])->ScriptRegisters.ric = ric;
			((Unit*)ents->entities[value])->ScriptRegisters.rid = rid;
			((Unit*)ents->entities[value])->ScriptRegisters.rie = rie;
			((Unit*)ents->entities[value])->ScriptRegisters.rif = rif;
			((Unit*)ents->entities[value])->ScriptRegisters.rig = rig;
			((Unit*)ents->entities[value])->ScriptRegisters.rih = rih;
			
			((Unit*)ents->entities[value])->ScriptRegisters.rfa = rfa;
			((Unit*)ents->entities[value])->ScriptRegisters.rfb = rfb;
			((Unit*)ents->entities[value])->ScriptRegisters.rfc = rfc;
			((Unit*)ents->entities[value])->ScriptRegisters.rfd = rfd;
			((Unit*)ents->entities[value])->ScriptRegisters.rfe = rfe;
			((Unit*)ents->entities[value])->ScriptRegisters.rff = rff;
			((Unit*)ents->entities[value])->ScriptRegisters.rfg = rfg;
			((Unit*)ents->entities[value])->ScriptRegisters.rfh = rfh;
		}
    }
	Py_RETURN_NONE;
}
 
SCRIPTFUNC(game_script_distance) {
    DWORD value;
      DWORD target;
 
    if (!PyArg_ParseTuple(args, "ii", &value,&target))        return NULL;
 
      if (value && target && ents->entities[value] && ents->entities[target] && ents->entities[value]->alive && ents->entities[target]->alive) {
			float d = dist(ents->entities[value]->position,ents->entities[target]->position);
			return PyFloat_FromDouble((double)d);
      }
      return NULL;
}

SCRIPTFUNC(game_script_gettime) {
	return PyFloat_FromDouble(timer->time);
}

SCRIPTFUNC(game_killent) {
    DWORD value;

	if (!PyArg_ParseTuple(args, "i", &value))        return NULL;

	if (value && ents->entities[value] && ents->entities[value]->alive) {
		ents->entities[value]->Kill();
	}

	Py_RETURN_NONE;
}

PYTHONMODULE(GameMethods)
    pydef("echo",  game_print)
    pydef("spawn",   game_spawn)
	pydef("killent", game_killent)
    pydef("save",    game_save)
 
	pydef("GetScriptRegisters", game_script_GetRegs)

	pydef("setPosition", game_script_setposition)
    pydef("getPosition", game_script_getposition)
    pydef("setState", game_script_setstate)
    pydef("getState", game_script_getstate)
	pydef("getTime", game_script_gettime)

	pydef("distance", game_script_distance)
     
    pydef("getNearestEntity", game_script_getnearestentity)
 
    pydef("pathTo", game_script_pathto)
    pydef("pathToEnt", game_script_pathtoent)
ENDPYTHONMOD

 


bool Console::init() {
	history.clear();

	Py_Initialize();
	Py_InitModule("__main__", GameMethods);

	PyRun_SimpleString("echo 'Embedded Python console initialization complete'\necho 'Alex shank - 2008 - alexjshank@gmail.com'");

	// redirect python console output through the proper channels
	char *code =	"class Sout:\n"
					"    def write(self, s):\n"
					"        echo(s)\n"			
					"\n"
					"import sys\n"
					"sys.stdout = Sout()\n"
					"sys.stderr = Sout()\n";
//					"sys.stdin  = None\n";
	int r1 = PyRun_SimpleString(code);

	

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
	
		// this is a pretty shitty interface to the input class... input class should store keypress events in a list for each frame to be retrieved in one call to avoid this loop
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

#define min(x,y) ((x < y)?x:y)
void Console::RunLine(const char *src) {
	PyRun_SimpleString(src);
	PyObject * pyerr = PyErr_Occurred();

	if (pyerr) {
		printf("Embedded Python Error in script:\n------------------\n%s\n------------------\n\n",src);
		PyErr_Print();
	}
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
	std::string pbl(line);
	history.push_back(pbl);
	printf("%s\n",line);
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

std::string Console::LoadScript(const char *scriptFilename) {
	char scriptBuffer[100001];
	std::string script = "";
	FILE *fin = fopen(scriptFilename,"r");

	if (fin) {
		ZeroMemory(scriptBuffer,100000);
		fseek(fin,0,SEEK_END);
		int len = ftell(fin);
		fseek(fin,0,SEEK_SET);

		fread(scriptBuffer,1,len,fin);
		scriptBuffer[len-1] = 0;
		script = scriptBuffer;
	} else {
		Printf("Error opening script file '%s'",scriptFilename);
	}
	return script;
}