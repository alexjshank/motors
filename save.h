#include "entity.h"
#include <windows.h>
#include <string.h>

/*

magic 'ASGE07'
map size	(int x 2)
terrain name

camera start pos (int x 2)

entitycount int

entities[entitycount] {
	name
	type
	position
}


*/

struct MapHeader {
	char magic[6];	// ASGE07
	short size_x, size_y;
	char terrainName[32];
	int entityCount;
};



struct MapEntity {
	ENT_TYPE type;	
	float x,y;
};

void SaveWorldState(const char *filename);
void LoadWorldState(const char *filename);