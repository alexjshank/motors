#include "save.h"
#include "Timer.h"
#include "Entity.h"
#include "Terrain.h"
#include "graphics.h"
#include "camera.h"
#include "console.h"
#include "unit.h"

extern Graphics *renderer;
extern Camera * camera;
extern Terrain *terrain;
extern Console *console;
extern Timer *timer;
extern EntityContainer *ents;
using std::string;


void SaveWorldState(const char *filename) {
	MapHeader head;
	MapEntity ent;

	FILE * fout = fopen(filename,"w");

	strcpy(head.magic,"ASGE07");
	head.size_x = terrain->width;
	head.size_y = terrain->height;
	strcpy(head.terrainName,terrain->terrainName.c_str());
	head.entityCount = (int)ents->entities.size();

	fwrite(&head,sizeof(MapHeader),1,fout);

	for (int i=0;i<head.entityCount;i++) {
		if (ents->entities[i] && ents->entities[i]->alive && ents->entities[i]->family > 0) {
			strcpy(ent.entityName, ((Unit*)ents->entities[i])->classname.c_str());
			ent.x = ents->entities[i]->position.x;
			ent.y = ents->entities[i]->position.z;
			fwrite(&ent,sizeof(ent),1,fout);
		}
	}

	fclose(fout);
}


void LoadWorldState(const char *filename) {
	MapHeader head;

	console->Printf("Loading world: %s",filename);

	FILE *fin = fopen(filename,"r");
	memset(&head,0,sizeof(MapHeader));

	if (!fin) {
		console->Printf("*** Critical: Error opening world file ***");
		return;
	}

	fread(&head,sizeof(MapHeader),1,fin);

	if (strncmp(head.magic,"ASGE07",6)!=0) {
		// error
		console->Printf("world magic number incorrect!");
		return;
	}


	char textureName[255];
	sprintf(textureName,"data/topographical/%s.JPG",head.terrainName);
	
	terrain = new Terrain(head.terrainName, renderer->LoadTexture(textureName),renderer->LoadTexture("data/models/water_surface.JPG"), camera);
	
	
	ents->AddEntity((Entity*)terrain);
	MapEntity e;
	for (int i=0;i<head.entityCount && !feof(fin);i++) {
		fread(&e,sizeof(MapEntity),1,fin);	

		Unit *unit = new Unit(e.entityName);
		unit->position = Vector(e.x,0,e.y);
		unit->completed = 100;
		ents->AddEntity(unit);

	}

	fclose(fin);
}
