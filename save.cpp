#include "save.h"
#include "Timer.h"
#include "Entity.h"
#include "Terrain.h"
#include "graphics.h"

extern Graphics *renderer;
extern Terrain *terrain;
extern Timer *timer;
extern EntityContainer *ents;
using std::string;


void SaveWorldState(const char *filename) {
	MapHeader head;
	MapEntity ent;

	FILE * fout = fopen(filename,"w");

	strcpy(head.magic,"ASGE07");
	head.size_x = 512;
	head.size_y = 512;
	strcpy(head.terrainName,"PirateIsland");
	head.entityCount = (int)ents->entities.size();
	fwrite(&head,sizeof(MapHeader),1,fout);


	for (int i=0;i<head.entityCount;i++) {
		if (ents->entities[i] && ents->entities[i]->alive && ents->entities[i]->type != E_SHEEP) {
			ent.type = (ENT_TYPE)ents->entities[i]->type;
			ent.x = ents->entities[i]->position.x;
			ent.y = ents->entities[i]->position.z;
			fwrite(&ent,sizeof(ent),1,fout);
		}
	}

	fclose(fout);
}


void LoadWorldState(const char *filename) {
	MapHeader head;
	FILE *fin = fopen(filename,"r");
	memset(&head,0,sizeof(MapHeader));

	if (!fin) {
//		console->Printf("load world failed");
		return;
	}

	fread(&head,sizeof(MapHeader),1,fin);

	if (strcmp(head.magic,"ASGE07")!=0) {
		// error
		return;
	}

	terrain = new Terrain;
	char terrainName[255];
	sprintf(terrainName,"data/topographical/%s.top",head.terrainName);
	terrain->LoadHeightMap(terrainName);
	
	char textureName[255];
	sprintf(textureName,"data/topographical/%s.bmp",head.terrainName);
	terrain->SetTexture(renderer->LoadTexture(textureName),renderer->LoadTexture("data/models/water_surface.bmp"));
	
	ents->AddEntity((Entity*)terrain);
	MapEntity e;
	for (int i=0;i<head.entityCount && !feof(fin);i++) {
		fread(&e,sizeof(MapEntity),1,fin);	
//		if (e.type == E_SHEEP || e.type == E_PEASANT || e.type == E_VILLAGEWOMAN || e.type == E_SOLDIER) continue;
		SpawnEntity(e.type,Vector(e.x,0,e.y));
	}

	fclose(fin);
}
