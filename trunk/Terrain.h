#pragma once
#include "entity.h"
#include <assert.h>
#include "Camera.h"

#define TC_LAND 0			// empty land
#define TC_UNBUILDABLE 1	
#define TC_UNWALKABLE 2
#define TC_UNDRIVABLE 4
#define TC_UNSAILABLE 8
#define TC_UNFLYABLE 16


#define TC_STRUCTURE (TC_UNBUILDABLE|TC_UNWALKABLE|TC_UNDRIVABLE|TC_UNSAILABLE)

#define TC_WATER (TC_LAND|TC_UNBUILDABLE|TC_UNWALKABLE|TC_UNDRIVABLE)
#define TC_MUD (TC_LAND|TC_UNBUILDABLE|TC_UNDRIVABLE)

class AS_Node {
public:
	AS_Node() {
		parent = NULL;
		H = 0;
		G = 0;
	}

	AS_Node *parent;
	Vector position;
	float H;
	float G;
};

class Terrain :
	public Entity
{
public:
	Terrain(void);
	~Terrain(void);

	Terrain(const char *heightmap);
	Terrain(const char *heightmap, int texture);
	Terrain(const char *heightmap, int mainTexture, int detailTexture);
	Terrain(const char *heightmap, int mainTexture, int detailTexture, Camera *camera);

	bool LoadHeightMap(const char * filename);
	void SmootheTerrain();
	void CalculateNormals();
	void CalculateWaveData();
	void CalculateColorData();

	void SetTexture(int mT, int wT);
	void SetCamera(Camera * cam);

	void render();
	void renderLand(int detail, int insideDetail, int maxDistanceFromCamera, int minDistanceFromCamera);
	void renderWater();

	Vector RayTest(Vector start, Vector direction, float length, float step, bool accurate);
	unsigned char getContents(int x, int y);
	void setContents(int x, int y, int w, int h, unsigned char contents);
	void setContents(int x, int y, unsigned char contents);
	inline float getHeight(int x,int y);
	Vector getNormal(int x, int y);
	float getInterpolatedHeight(float x, float y);

// A* searching stuff:
	float getGscore(Vector start, Vector p, Vector end);
	void ResetNodes();
	AS_Node * AStarSearch(Vector start, Vector end);
	AS_Node *getClosestOpenNode(Vector p);
	bool CloseNode(AS_Node *openNode);

private:
	AS_Node *endNode, *startNode;
	std::list<AS_Node *> openList;
	std::list<AS_Node *> closedList;
	AS_Node *ASnodes;

public:
	Vector AmbientColor;
	Vector SunDirection;
	float fAvgWaterHeight;

	int width;
	int height;
	std::string terrainName;

private:
	int mainTexture;
	int detailTexture;
	int waterTexture;
	int waterReflection;
	int displayList;

	Camera *ourCamera;
	Vector lastLODUpdatePosition;
	

	float scale;

	struct WaveData {
		Vector waveBreakPosition;
		Vector waveBreakDirection;
		float waveSize;
		float i;
	};
	std::vector<WaveData> waves;
	int waveTexture;

	unsigned char *ucColorData[3];	// red green blue
	float *fHeightData;
	float *fWaterHeights;
	Vector *vNormals;
	unsigned char *bTerrainContents;
};
