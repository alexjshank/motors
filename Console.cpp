#include ".\console.h"
#include "graphics.h"
#include "input.h"
#include "printer.h"
#include <python.h>
#include "entity.h"
#include "unit.h"
#include "Camera.h"
#include "variables.h"
#include "LassoSelector.h"
#include "ui.h"

#include <sstream>

extern gamevars *vars;
extern Graphics *renderer;
extern EntityContainer *ents;
extern Input *input;
extern Camera *camera;
extern LassoSelector *selector;
extern UI *ui;

#define PYTHONMODULE(varname) static PyMethodDef varname[] = {
#define ENDPYTHONMOD {0,0,0,0} };
#define SCRIPTFUNC(name) static PyObject* name(PyObject *self, PyObject *args)
#define pydef(a,b)  {a, b, METH_VARARGS, ""},



Console::Console(void)
{
	enabled = true;
	toggled = false;
	fout = 0;
}

Console::~Console(void)
{
	Py_Finalize();
}

extern Console *console;


SCRIPTFUNC(game_print) {
    const char *value;

    if (!PyArg_ParseTuple(args, "s", &value))
        return NULL;

	console->Print(value);

	Py_RETURN_NONE;
}


SCRIPTFUNC(game_getxpos) {
	DWORD a,b;

	if (!PyArg_ParseTuple(args, "i", &a))        
		return NULL;

	if (a && ents->entities[a]) {
		return PyFloat_FromDouble(ents->entities[a]->position.x);
	}
	return NULL;
}
SCRIPTFUNC(game_getzpos) {
	DWORD a,b;

	if (!PyArg_ParseTuple(args, "i", &a))        
		return NULL;

	if (a && ents->entities[a]) {
		return PyFloat_FromDouble(ents->entities[a]->position.z);
	}
	return NULL;
}
SCRIPTFUNC(game_getypos) {
	DWORD a,b;

	if (!PyArg_ParseTuple(args, "i", &a))        
		return NULL;

	if (a && ents->entities[a]) {
		return PyFloat_FromDouble(ents->entities[a]->position.y);
	}
	return NULL;
}

bool PlaceEntity(Entity *ent, const char *script);

SCRIPTFUNC(game_placeent) {
	DWORD id;
	const char *placescript;

    if (!PyArg_ParseTuple(args, "is", &id, &placescript))
        return NULL;

	if (id >= 0 && ents->entities[id]) {
		PlaceEntity(ents->entities[id],placescript);
	}

	Py_RETURN_NONE;
}

SCRIPTFUNC(game_spawnunit) {
	char *entityName;
	float x,y;

    if (!PyArg_ParseTuple(args, "sff", &entityName,&x,&y))
        return NULL;

	try {
		Unit *unit = new Unit(entityName);
		unit->position = Vector(x,0,y);
		ents->AddEntity(unit);
		return PyInt_FromLong(unit->id);
	} catch (...) {
		console->Printf("super bad error creating new unit(%s)",entityName);
		return 0;
	}
}

SCRIPTFUNC(game_queuescriptedtask) {
	int i;
	char *script;

    if (!PyArg_ParseTuple(args, "is", &i, &script))
        return NULL;

	if (i && ents->entities[i] && ents->entities[i]->family == EF_UNIT) {
		ENT_TASK task;
		ents->entities[i]->QueueTask(task.CreateScriptedTask(script),false);
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

SCRIPTFUNC(game_script_use) {
	DWORD a,b;

	if (!PyArg_ParseTuple(args, "ii", &a, &b))        
		return NULL;

	if (a && ents->entities[a] && ents->entities[a]->alive) {
	
	}

	Py_RETURN_NONE;
}

SCRIPTFUNC(game_script_attach) {
	DWORD a,b;

	if (!PyArg_ParseTuple(args, "ii", &a, &b))        
		return NULL;

	if ((a && ents->entities[a] && ents->entities[a]->alive) && (b && ents->entities[b] && ents->entities[b]->alive)) {
		ents->entities[a]->Attach(ents->entities[b]);
	}

	Py_RETURN_NONE;
}

SCRIPTFUNC(game_loadscript) {
	const char *scriptname;

	if (!PyArg_ParseTuple(args,"s",&scriptname))
		return NULL;

	console->LoadScript(scriptname);

	Py_RETURN_NONE;
}

SCRIPTFUNC(game_script_getteam) {
	DWORD a,b;

	if (!PyArg_ParseTuple(args, "i", &a))        
		return NULL;

	if (a && ents->entities[a]) {
		return PyInt_FromLong(ents->entities[a]->team);
	}
	return NULL;
}


SCRIPTFUNC(game_script_gethealth) {
	DWORD a,b;

	if (!PyArg_ParseTuple(args, "i", &a))        
		return NULL;

	if (a && ents->entities[a]) {
		return PyInt_FromLong(ents->entities[a]->health);
	}
	return NULL;
}


SCRIPTFUNC(game_script_getcompleted) {
	DWORD a,b;

	if (!PyArg_ParseTuple(args, "i", &a))        
		return NULL;

	if (a && ents->entities[a]) {
		return PyInt_FromLong(ents->entities[a]->completed);
	}
	return NULL;
}

SCRIPTFUNC(game_script_setcompleted) {
	DWORD a,b;

	if (!PyArg_ParseTuple(args, "ii", &a, &b))        
		return NULL;

	if (a && ents->entities[a]) {
		ents->entities[a]->completed = b;
	}

	Py_RETURN_NONE;
}


SCRIPTFUNC(game_script_gettype) {
	DWORD a,b;

	if (!PyArg_ParseTuple(args, "i", &a))        
		return NULL;

	if (a && ents->entities[a]) {
		return PyInt_FromLong(ents->entities[a]->type);
	}
	return NULL;
}

SCRIPTFUNC(game_script_getclassname) {
	DWORD a,b;

	if (!PyArg_ParseTuple(args, "i", &a))        
		return NULL;

	if (a && ents->entities[a] && ents->entities[a]->family == EF_UNIT) {
		return PyString_FromString(((Unit*)ents->entities[a])->classname.c_str());
	}
	return NULL;
}


SCRIPTFUNC(game_script_setmodel) {
	DWORD id;
	const char *modelname, *texturename;

	if (!PyArg_ParseTuple(args, "iss", &id, &modelname, &texturename))        
		return NULL;

	if (id && ents->entities[id] && ents->entities[id]->family == EF_UNIT) {
		((Unit*)ents->entities[id])->SetModel(modelname, texturename);
	}

	Py_RETURN_NONE;
}

SCRIPTFUNC(game_script_setThinkInterval) {
	DWORD id;
	float interval;

	if (!PyArg_ParseTuple(args,"if", &id, &interval)) return 0;

	if (id && ents->entities[id] && ents->entities[id]->family == EF_UNIT) {
		((Unit*)ents->entities[id])->updateInterval = interval;
	}

	Py_RETURN_NONE;
}

SCRIPTFUNC(game_script_setspeed) {
	DWORD id;
	float walkspeed, runspeed;

	if (!PyArg_ParseTuple(args,"iff", &id, &walkspeed, &runspeed)) return 0;

	if (id && ents->entities[id] && ents->entities[id]->family == EF_UNIT) {
		((Unit*)ents->entities[id])->walkspeed = walkspeed;
		((Unit*)ents->entities[id])->runspeed = runspeed;
	}

	Py_RETURN_NONE;
}

SCRIPTFUNC(game_setsubtitle) {
	DWORD id;
	const char *subtitle;

	if (!PyArg_ParseTuple(args,"is", &id, &subtitle)) return 0;

	if (id && ents->entities[id] && ents->entities[id]->family == EF_UNIT) {
		ents->entities[id]->tooltip.subtitle = subtitle; 
	}

	Py_RETURN_NONE;
}

SCRIPTFUNC(game_loadmenu) {
	DWORD id;
	const char *menuname;

	if (!PyArg_ParseTuple(args,"is", &id, &menuname)) return 0;

	if (id && ents->entities[id] && ents->entities[id]->family == EF_UNIT) {
		((Unit*)ents->entities[id])->menu = (UIWindow *)ui->CreateFromFile(menuname);
		if (((Unit*)ents->entities[id])->menu)
			((Unit*)ents->entities[id])->menu->ownerID = id;
	}

	Py_RETURN_NONE;
}

SCRIPTFUNC(game_showmenu) {
	DWORD id;

	if (!PyArg_ParseTuple(args,"i", &id)) return 0;

	if (id && ents->entities[id] && ents->entities[id]->family == EF_UNIT) {
		if (((Unit*)ents->entities[id])->menu) {
			((Unit*)ents->entities[id])->menu->visible = true;
			((Unit*)ents->entities[id])->menu->closeButton.pressed = false;
		}
	}

	Py_RETURN_NONE;
}


SCRIPTFUNC(game_hidemenu) {
	DWORD id;

	if (!PyArg_ParseTuple(args,"i", &id)) return 0;

	if (id && ents->entities[id] && ents->entities[id]->family == EF_UNIT) {
		((Unit*)ents->entities[id])->menu->visible = false;
	}

	Py_RETURN_NONE;
}


SCRIPTFUNC(game_script_random) {
	return PyFloat_FromDouble((float)(rand()%100)/100);
}

PYTHONMODULE(GameMethods)
	pydef("loadscript", game_loadscript)
    pydef("echo",  game_print)
	pydef("random", game_script_random)
	pydef("spawnunit", game_spawnunit)
	pydef("placeent", game_placeent)
	pydef("killent", game_killent)
    pydef("save",    game_save)

	pydef("loadMenu", game_loadmenu)
	pydef("showMenu", game_showmenu)
	pydef("hideMenu", game_hidemenu)
 
	pydef("getClassname", game_script_getclassname)

	pydef("setUpdateInterval", game_script_setThinkInterval)

	pydef("setSubtitle", game_setsubtitle)
	pydef("queueScriptedTask", game_queuescriptedtask)
	pydef("setSpeed", game_script_setspeed)
	pydef("setModel", game_script_setmodel)
	pydef("setPosition", game_script_setposition)
    pydef("getPosition", game_script_getposition)
	pydef("getXpos", game_getxpos)
	pydef("getYpos", game_getypos)
	pydef("getZpos", game_getzpos)
    pydef("setState", game_script_setstate)
    pydef("getState", game_script_getstate)
	pydef("getTime", game_script_gettime)
	pydef("getTeam", game_script_getteam)
	pydef("getHealth", game_script_gethealth)
	pydef("getCompleted", game_script_getcompleted)
	pydef("setCompleted", game_script_setcompleted)
	pydef("getType", game_script_gettype)

	pydef("distance", game_script_distance)
     
    pydef("getNearestEntity", game_script_getnearestentity)
 
    pydef("pathTo", game_script_pathto)
    pydef("pathToEnt", game_script_pathtoent)

	pydef("use", game_script_use)
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

	fout = fopen("console.log","w");

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
	
	if (fout) {
		fprintf(fout,"%s\n",line);
		fflush(fout);
	}
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

std::string Console::LoadScriptf(const char *format, ...) {
	char text[1024];
	va_list args;

	if (!format) return "";

	va_start(args,format);
		vsprintf(text,format,args);
	va_end(args);

	return LoadScript(text);
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

	RunLine(script.c_str());
	return script;
}