#include ".\terrain.h"
#include "graphics.h"
#include "timer.h"
#include "camera.h"
#include "console.h"

//#define USE_PROCEDURAL_TERRAIN_INSTEAD

extern Graphics *renderer;
extern Console *console;

Terrain::Terrain(void) {
	size = Vector(0,0,0);
}

Terrain::~Terrain(void) {
	if (bTerrainContents) {
		delete [] bTerrainContents;
	}
	glDeleteLists(displayList,1);
}


Terrain::Terrain(const char *heightmap) {
	LoadHeightMap(heightmap);
}

Terrain::Terrain(const char *heightmap, int texture) {
	LoadHeightMap(heightmap);
	SetTexture(texture,0);
}

Terrain::Terrain(const char *heightmap, int mainTexture, int detailTexture) {
	LoadHeightMap(heightmap);
	SetTexture(mainTexture,detailTexture);
}

Terrain::Terrain(const char *heightmap, int mainTexture, int detailTexture, Camera *camera) {
	LoadHeightMap(heightmap);
	SetTexture(mainTexture,detailTexture);
	SetCamera(camera);
}



bool Terrain::LoadHeightMap(const char * filename) {
	console->Printf("Loading terrain file '%s'",filename);

	std::string fullpath = "data/topographical/"; fullpath += filename; fullpath += ".top";


	fAvgWaterHeight = 4;
	SunDirection = Vector(0.75f,0.25f,0);

	AmbientColor = Vector(24,24,48); // dark bluish

	FILE *fmap;
	if (fopen_s(&fmap,fullpath.c_str(),"rb") != 0) {
		console->Printf("Error opening terrain file '%s'",filename);
		return false;
	}

	fseek(fmap,0,SEEK_END);
	DWORD filesize = ftell(fmap);
	fseek(fmap,0,SEEK_SET);
	int dimensions = (int)sqrtf((float)filesize);
	width = dimensions;
	height = dimensions;
	int size = filesize;
	terrainName = filename;

	if (!(bTerrainContents = new unsigned char[size])) {
		// error
		console->Printf("Error allocating bterraincontents memory [%d]", size);
		return false;
	}
	ZeroMemory(bTerrainContents,size);

	if (!(fHeightData = new float[size])) {
		// error
		console->Printf("Error allocating fheightdata memory [%d]", size);
		return false;
	}

	if (!(fWaterHeights = new float[size])) {
		console->Printf("Error allocating fwaterheights memory [%d]", size);
		return false;
	}
	for (int i=0;i<size;i++) { fWaterHeights[i] = fAvgWaterHeight; }

	if (!(ucColorData[0] = new unsigned char[size]) || 
		!(ucColorData[1] = new unsigned char[size]) || 
		!(ucColorData[2] = new unsigned char[size])) {
		console->Printf("Error allocated uccolordata memory [%d]", size);
		return false;
	}

	if (!(vNormals = new Vector[size])) {
		// error
		console->Printf("Error allocated vnormals memory [%d]", size);
		return false;
	}

	if (!(ASnodes = new AS_Node[size])) {
		// error
		console->Printf("Error allocated asnodes memory [%d]", size);
		return false;
	}
	
	if (feof(fmap)) {
		// error - no terrain data
		console->Printf("Error no terrain data");
		return false;
	}

	
	unsigned char *bHeightData = new unsigned char[size];
	fread(bHeightData,1,size,fmap);										// read in the compressed height data (bytes)
	fclose(fmap);
	for (int y=0;y<height;y++) {
		for (int x=0;x<width;x++) {
			fHeightData[x+(y*width)] = (float)bHeightData[x+(y*width)] / 8;	// and load it into the float array
		}
	}
	delete [] bHeightData;

	SmootheTerrain();
	CalculateNormals();
	CalculateColorData();

	waterReflection = renderer->LoadTexture("data/topographical/reflection.JPG");

	displayList = glGenLists(1);

	// set up the terrain contents (this should probably be its own file, but for now we're just generating it and checking for water

	memset(bTerrainContents,0,size);

	for (int y=1;y<height-1;y++) {
		for (int x=1;x<width-1;x++) {
			if (getHeight(x,y) <= fAvgWaterHeight) {
				setContents(x,y, TC_WATER);
			}
		}
	}

	CalculateWaveData();

	
	console->Printf("Finished loading terrain");
	return true;
}


void Terrain::SmootheTerrain() {
	float * sourceData = fHeightData;
	
	for (int samples = 0; samples < 3; samples++) {
		for (int y=1;y<height-1;y++) {
			for (int x=1;x<width-1;x++) {
				float blocksum = (float)(	(float)sourceData[((y-1)*height)+(x-1)]	+ (float)sourceData[((y-1)*height)+x]		+ (float)sourceData[((y-1)*height)+(x+1)] + 
											(float)sourceData[(y*height)+(x-1)]		+ (float)sourceData[(y*height)+x]			+ (float)sourceData[(y*height)+(x+1)] + 
											(float)sourceData[((y+1)*height)+x-1]	+ (float)sourceData[((y+1)*height)+x]		+ (float)sourceData[((y+1)*height)+(x+1)]);
				int pos = (y*height)+x;
				fHeightData[pos] = (blocksum/9);
			}
		}
	}
}

void Terrain::CalculateNormals() {
	for (int y=1;y<height-1;y++) {
		for (int x=1;x<width-1;x++) {
			Vector a = Vector((float)x,getInterpolatedHeight((float)x,(float)y),	(float)y) -		Vector((float)x+1, getInterpolatedHeight((float)x+1,(float)y),(float)y);
			Vector b = Vector((float)x,getInterpolatedHeight((float)x,(float)y+1),	(float)y+1) -	Vector((float)x+1, getInterpolatedHeight((float)x+1,(float)y),(float)y);
			int index = (y*height)+x;
			vNormals[index] = a^b;
		}
	}
}

#define MAX(x,y) ((x>y)?x:y)
#define MIN(x,y) ((x<y)?x:y)


void Terrain::CalculateColorData() {
	SunDirection.normalize();
	Vector nd = SunDirection;
	nd.z = -nd.z;

	for (int y=1;y<height;y++) {
		for (int x=1;x<width;x++) {
			int index = (y*width)+x;
			Vector sp((float)x,getHeight(x,y)+1,(float)y);

			// base color based on angle to the sun!
			Vector normal = getNormal(x,y);
			float angle = normal.Dot(SunDirection);
			if (angle > 1) angle = 1;
			
			int AngleColor = 127+((unsigned char)(angle * 127));

			ucColorData[0][index] = (unsigned char)MAX(AmbientColor.x, AngleColor);
			ucColorData[1][index] = (unsigned char)MAX(AmbientColor.y, AngleColor);
			ucColorData[2][index] = (unsigned char)MAX(AmbientColor.z, AngleColor);

		

			int numPasses = 8;
			int blockSize = 255 / numPasses;
			Vector Ray = (nd*30);

			for (int i=numPasses; i>0; i--) {
				Vector origin = sp+Vector(0,2.0f * i,0);

				Vector test = RayTest(origin,nd,30,1,false);

				if ( ((origin + Ray) - test).len2() > 2) {

					int ShadeValue = (i * blockSize);

					ucColorData[0][index] = (unsigned char)MAX(AmbientColor.x, (ucColorData[0][index] - MAX(ShadeValue,ucColorData[0][index])));
					ucColorData[1][index] = (unsigned char)MAX(AmbientColor.y, (ucColorData[1][index] - MAX(ShadeValue,ucColorData[1][index])));
					ucColorData[2][index] = (unsigned char)MAX(AmbientColor.z, (ucColorData[2][index] - MAX(ShadeValue,ucColorData[2][index])));
					break;
				}
			}
		}
	}

	// smoothe it out
	for (int smoothePass=0; smoothePass < 4; smoothePass++) {
		for (int y=0;y<height-1;y++) {
			for (int x=0;x<width-1;x++) {
				int indicies[] = {(y*width)+x,
								  ((y+1)*width)+x,
								  (y*width)+x+1,
								  ((y+1)*width)+x+1};

				ucColorData[0][indicies[0]] = (ucColorData[0][indicies[0]] + ucColorData[0][indicies[1]] + ucColorData[0][indicies[2]] + ucColorData[0][indicies[3]]) / 4;
				ucColorData[1][indicies[0]] = (ucColorData[1][indicies[0]] + ucColorData[1][indicies[1]] + ucColorData[1][indicies[2]] + ucColorData[1][indicies[3]]) / 4;
				ucColorData[2][indicies[0]] = (ucColorData[2][indicies[0]] + ucColorData[2][indicies[1]] + ucColorData[2][indicies[2]] + ucColorData[2][indicies[3]]) / 4;


			}
		}
	}

}


void Terrain::SetTexture(int mT, int wT) {
	mainTexture = mT;
	waterTexture = wT;
}

void Terrain::SetCamera(Camera *cam) {
	ourCamera = cam;
}

void Terrain::CalculateWaveData() {
	waveTexture = renderer->LoadTexture("data\\topographical\\wave.JPG");
	int tilesize = 3;
	for (int xi=0;xi<width;xi+=tilesize) {
		for (int yi=0;yi<height;yi+=tilesize) {
			if (getHeight(xi,yi) <= fAvgWaterHeight) {
				Terrain::WaveData wd;
	
				float bestAngle = -1;
				float highestHeight = 0;
				int testStep = 10;
				float testDist = 2;

				for (int angle = 0;angle<180; angle+= testStep) {
					Vector tp = Vector((float)xi,fAvgWaterHeight,(float)yi) + (Vector(sinf((float)angle),0,cosf((float)angle)) * testDist);
					float h = getInterpolatedHeight(tp.x,tp.z);
					if (h > highestHeight) {
						highestHeight = h;
						bestAngle = (float)angle;
					}
				}

				if (highestHeight > fAvgWaterHeight+0.2f) {
					wd.waveBreakDirection = Vector(-sinf(bestAngle),0,-cosf(bestAngle));
					wd.waveBreakPosition = Vector((float)xi,fAvgWaterHeight,(float)yi);
					waves.push_back(wd);
				}
			}
		}
	}
}

extern Graphics *renderer;
extern Timer *timer;

void Terrain::render() {
	glEnable(GL_DEPTH_TEST);	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,mainTexture);

	Vector delta = (lastLODUpdatePosition - ourCamera->GetPosition());
	if (delta.len2() > 5*5) {
		lastLODUpdatePosition = ourCamera->GetPosition();

		glNewList(displayList, GL_COMPILE);
		renderLand(16,16, 256,128);
		renderLand(8,16,128,64);
		renderLand(4,8,64,32);
		renderLand(2,4,32,16);
		renderLand(1,2,16,0);
		glEndList();
	}
		
	glCallList(displayList);

	renderWater();
}

#if !defined USE_PROCEDURAL_TERRAIN_INSTEAD // procedural terrain
/*
void Terrain::renderLand(int step, int insideStep,  int maxd, int mind) {
	int i,j;
	float xsize=(float)width;
	float ysize=(float)height;
	
	Vector cp = ourCamera->GetActualPosition() + (ourCamera->GetView() * 10.0f);
	int startX=(int)cp.x - maxd,startY=(int)cp.z - maxd,endX=(int)cp.x + maxd,endY=(int)cp.z + maxd;	
	
	startX = startX - (startX % 16);
	startY = startY - (startY % 16);
		
	
	for(i=startY; i<=endY-step; i+=step)
	{
		if (i < 0 || i > height-1) continue;

		for(j=startX; j<=endX-step; j+=step)
		{
			if (j < 0 || j > width-1) continue;

			glBegin(GL_QUADS);
			
		//	if (getContents(j,i) == TC_WATER && getContents(j+step,i+step) == TC_WATER && getContents(j+step,i) == TC_WATER && getContents(j,i+step) == TC_WATER) {
		//		continue;
		//	}
	
			bool onLeftEdge = false;
			bool onRightEdge = false;
			bool onTopEdge = false;
			bool onBottomEdge = false;
				if (j < endX && j+step >= endX) { 
					onRightEdge = true;
				}
				if (j == startX) { 
					onLeftEdge = true;
				}				
				if (i < endY && i+step >= endY) { 
					onTopEdge = true;
				}
				if (i == startY) { 
					onBottomEdge = true;
				}

			if (mind != 0) {
				if (j+step > cp.x-mind && 
					j+step      < cp.x+mind && 
					i+step > cp.z-mind && 
					i+step      < cp.z+mind) {
					continue;
				}
			}

			

			float i_d_xsize = i / xsize;
			float iStepped_d_xsize = (i+step) / xsize;
			float j_d_ysize = j / ysize;
			float jStepped_d_ysize = (j+step) / ysize;

			int floorX = ((j-startX) - ((j-startX)%insideStep)+startX);
			int ceilX = floorX + insideStep;
			int floorZ = (i - (i%insideStep));
			int ceilZ = floorZ + insideStep;

			int index = ((i*height)+j);
			glColor3ub((GLubyte)ucColorData[0][index],(GLubyte)ucColorData[1][index],(GLubyte)ucColorData[2][index]);
			glTexCoord2f((float)1-i_d_xsize,(float)j_d_ysize); 
			if (onBottomEdge && j != floorX) {
				glVertex3f((float)j,	(fHeightData[(i*height)+(floorX)]+fHeightData[(i*height)+(ceilX)])/2 ,		(float)i);
			} else {
				if (onRightEdge) {
					glVertex3f((float)j,	(fHeightData[(i*height)+(floorX)]+fHeightData[(i*height)+(ceilX)])/2 ,		(float)i);
				} else {
					glVertex3f((float)j,	fHeightData[index],		(float)i);
				}
			} 

			

			index = ((i*height)+(j+step));
			glColor3ub((GLubyte)ucColorData[0][index],(GLubyte)ucColorData[1][index],(GLubyte)ucColorData[2][index]);
			glTexCoord2f((float)1-i_d_xsize,(float)jStepped_d_ysize); 
			if (onBottomEdge && j+step != ceilX) {
				glVertex3f((float)j+step,	(fHeightData[(i*height)+(floorX)]+fHeightData[(i*height)+(ceilX)])/2 ,		(float)i);
			} else {
				glVertex3f((float)j+step,	fHeightData[index],		(float)i);
			}

			index = (((i+step)*height)+(j+step));
			glColor3ub((GLubyte)ucColorData[0][index],(GLubyte)ucColorData[1][index],(GLubyte)ucColorData[2][index]);
			glTexCoord2f((float)1-iStepped_d_xsize,(float)jStepped_d_ysize);
			if (onTopEdge && j+step != floorX && j+step != ceilX) {
				glVertex3f((float)j+step,	(fHeightData[((i+step)*height)+(floorX)]+fHeightData[((i+step)*height)+(ceilX)])/2 ,		(float)i+step);
			} else {
				glVertex3f((float)j+step,	fHeightData[index],		(float)i+step);
			}	

			index = (((i+step)*height)+j);
			glColor3ub((GLubyte)ucColorData[0][index],(GLubyte)ucColorData[1][index],(GLubyte)ucColorData[2][index]);
			glTexCoord2f((float)1-iStepped_d_xsize,(float)j_d_ysize); 
			if (onTopEdge && j != floorX && j != ceilX) {
				glVertex3f((float)j,	(fHeightData[((i+step)*height)+(floorX)]+fHeightData[((i+step)*height)+(ceilX)])/2 ,		(float)i+step);
			} else {
				glVertex3f((float)j,	fHeightData[index],		(float)i+step);
			}

			
			
	glEnd();
		}
	}
}
*/

void Terrain::renderLand(int step, int insideStep,  int maxd, int mind) {
	int i,j;
	float xsize=(float)width;
	float ysize=(float)height;
	
	Vector cp = ourCamera->GetPosition();
	cp = Vector((float)(floor(cp.x) - ((int)cp.x % 16)), 
				cp.y, 
				(float)(floor(cp.z) - ((int)cp.z % 16)));

	int startX = (int)cp.x - maxd; 
	int endX = (int)cp.x + maxd; 
	int startZ = (int)cp.z - maxd; 
	int endZ = (int)cp.z + maxd;
	
		
	
	for(i=startZ; i<endZ; i+=step)
	{
		if (i < 0 || i > height-1) continue;

		for(j=startX; j<endX; j+=step)
		{
			if (j < 0 || j > width-1) continue;

			//
			glBegin(GL_QUADS);			
			//glBegin(GL_LINE_STRIP);
			
			/*
			if (getContents(j,i) == TC_WATER && getContents(j+step,i+step) == TC_WATER && getContents(j+step,i) == TC_WATER && getContents(j,i+step) == TC_WATER) {
				continue;
			}
			*/

			bool onLeftEdge = false;
			bool onRightEdge = false;
			bool onTopEdge = false;
			bool onBottomEdge = false;
			
				if (j < endX && j+step >= endX) { 
					onRightEdge = true;
				}
				if (j == startX) { 
					onLeftEdge = true;
				}				
				if (i < endZ && i+step >= endZ) { 
					onTopEdge = true;
				}
				if (i == startZ) { 
					onBottomEdge = true;
				}
			

			if (mind != 0) {
				if (j+step > cp.x-mind && 
					j+step <= cp.x+mind && 
					i+step > cp.z-mind && 
					i+step <= cp.z+mind) {
					continue;
				}
			}

			

			float i_d_xsize = i / xsize;
			float iStepped_d_xsize = (i+step) / xsize;
			float j_d_ysize = j / ysize;
			float jStepped_d_ysize = (j+step) / ysize;

			int floorX = ((j-startX) - ((j-startX)%insideStep)+startX);
			int ceilX = floorX + insideStep;
			int floorZ = (i - (i%insideStep));
			int ceilZ = floorZ + insideStep;

			int index = ((i*height)+j);
			glColor3ub((GLubyte)ucColorData[0][index],(GLubyte)ucColorData[1][index],(GLubyte)ucColorData[2][index]);
			glTexCoord2f((float)1-i_d_xsize,(float)j_d_ysize); 
			if (onBottomEdge && j != floorX) {
				glVertex3f((float)j,	(getHeight(floorX,i)+getHeight(ceilX,i))/2 ,		(float)i);
			} else if (onLeftEdge && i != ceilZ && i != floorZ) {
				glVertex3f((float)j,	(getHeight(j,floorZ)+getHeight(j,ceilZ))/2 ,		(float)i);
			} else {
				glVertex3f((float)j,	getHeight(j,i),		(float)i);
			}

			

			index = ((i*height)+(j+step));
			glColor3ub((GLubyte)ucColorData[0][index],(GLubyte)ucColorData[1][index],(GLubyte)ucColorData[2][index]);
			glTexCoord2f((float)1-i_d_xsize,(float)jStepped_d_ysize); 
			if (onBottomEdge && j+step != ceilX) {
				glVertex3f((float)j+step,	(getHeight(floorX,i)+getHeight(ceilX,i))/2 ,		(float)i);
			} else if (onRightEdge && i != floorZ) {
				glVertex3f((float)j+step,	(getHeight(j+step,floorZ)+getHeight(j+step,ceilZ))/2 ,		(float)i);
			} else {
				glVertex3f((float)j+step,	getHeight(j+step,i),		(float)i);
			}

			index = (((i+step)*height)+(j+step));
			glColor3ub((GLubyte)ucColorData[0][index],(GLubyte)ucColorData[1][index],(GLubyte)ucColorData[2][index]);
			glTexCoord2f((float)1-iStepped_d_xsize,(float)jStepped_d_ysize);
			if (onTopEdge && j+step != ceilX) {
				glVertex3f((float)j+step,	(getHeight(floorX,i+step)+getHeight(ceilX,i+step))/2 ,		(float)i+step);
			} else if (onRightEdge && i+step != ceilZ) {
				glVertex3f((float)j+step,	(getHeight(j+step,floorZ)+getHeight(j+step,ceilZ))/2 ,		(float)i+step);
			} else {
				glVertex3f((float)j+step,	getHeight(j+step,i+step),		(float)i+step);
			}	

			index = (((i+step)*height)+j);
			glColor3ub((GLubyte)ucColorData[0][index],(GLubyte)ucColorData[1][index],(GLubyte)ucColorData[2][index]);
			glTexCoord2f((float)1-iStepped_d_xsize,(float)j_d_ysize); 
			if (onTopEdge && j != floorX) {
				glVertex3f((float)j,	(getHeight(floorX,i+step)+getHeight(ceilX,i+step))/2 ,		(float)i+step);
			} else if (onLeftEdge && i+step != ceilZ && i+step != floorZ) {
				glVertex3f((float)j,	(getHeight(j,floorZ)+getHeight(j,ceilZ))/2 ,		(float)i+step);
			} else {
				glVertex3f((float)j,	getHeight(j,i+step),		(float)i+step);
			}

			
			
	glEnd();
		}
	}
}
#else
void Terrain::renderLand(int step, int insideStep,  int maxd, int mind) {
	int i,j;
	float xsize=(float)width;
	float ysize=(float)height;
	
	Vector cp = ourCamera->GetActualPosition();
	cp = Vector((int)cp.x - ((int)cp.x % 16), cp.y, (int)cp.z - ((int)cp.z % 16));

	int startX = (int)cp.x - maxd; 
	int endX = (int)cp.x + maxd; 
	int startZ = (int)cp.z - maxd; 
	int endZ = (int)cp.z + maxd;
	
		
	
	for(i=startZ; i<endZ; i+=step)
	{
	//	if (i < 0 || i > height-1) continue;

		for(j=startX; j<endX; j+=step)
		{
	//		if (j < 0 || j > width-1) continue;

			//
			glBegin(GL_QUADS);			
			//glBegin(GL_LINE_STRIP);
			
			/*
			if (getContents(j,i) == TC_WATER && getContents(j+step,i+step) == TC_WATER && getContents(j+step,i) == TC_WATER && getContents(j,i+step) == TC_WATER) {
				continue;
			}
			*/

			bool onLeftEdge = false;
			bool onRightEdge = false;
			bool onTopEdge = false;
			bool onBottomEdge = false;
				if (j < endX && j+step >= endX) { 
					onRightEdge = true;
				}
				if (j == startX) { 
					onLeftEdge = true;
				}				
				if (i < endZ && i+step >= endZ) { 
					onTopEdge = true;
				}
				if (i == startZ) { 
					onBottomEdge = true;
				}

			if (mind != 0) {
				if (j+step > cp.x-mind && 
					j+step < cp.x+mind && 
					i+step > cp.z-mind && 
					i+step < cp.z+mind) {
					continue;
				}
			}

			

			float i_d_xsize = i / xsize;
			float iStepped_d_xsize = (i+step) / xsize;
			float j_d_ysize = j / ysize;
			float jStepped_d_ysize = (j+step) / ysize;

			int floorX = ((j-startX) - ((j-startX)%insideStep)+startX);
			int ceilX = floorX + insideStep;
			int floorZ = (i - (i%insideStep));
			int ceilZ = floorZ + insideStep;

			//int index = ((i*height)+j);
			//glColor3ub((GLubyte)ucColorData[0][index],(GLubyte)ucColorData[1][index],(GLubyte)ucColorData[2][index]);
			glTexCoord2f((float)1-i_d_xsize,(float)j_d_ysize); 
			if (onBottomEdge && j != floorX) {
				glVertex3f((float)j,	(getHeight(floorX,i)+getHeight(ceilX,i))/2 ,		(float)i);
			} else if (onLeftEdge && i != floorZ) {
				glVertex3f((float)j,	(getHeight(j,floorZ)+getHeight(j,ceilZ))/2 ,		(float)i);
			} else {
				glVertex3f((float)j,	getHeight(j,i),		(float)i);
			}

			

			//index = ((i*height)+(j+step));
			//glColor3ub((GLubyte)ucColorData[0][index],(GLubyte)ucColorData[1][index],(GLubyte)ucColorData[2][index]);
			glTexCoord2f((float)1-i_d_xsize,(float)jStepped_d_ysize); 
			if (onBottomEdge && j+step != ceilX) {
				glVertex3f((float)j+step,	(getHeight(floorX,i)+getHeight(ceilX,i))/2 ,		(float)i);
			} else if (onRightEdge && i != floorZ) {
				glVertex3f((float)j+step,	(getHeight(j+step,floorZ)+getHeight(j+step,ceilZ))/2 ,		(float)i);
			} else {
				glVertex3f((float)j+step,	getHeight(j+step,i),		(float)i);
			}

			//index = (((i+step)*height)+(j+step));
			//glColor3ub((GLubyte)ucColorData[0][index],(GLubyte)ucColorData[1][index],(GLubyte)ucColorData[2][index]);
			glTexCoord2f((float)1-iStepped_d_xsize,(float)jStepped_d_ysize);
			if (onTopEdge && j+step != ceilX) {
				glVertex3f((float)j+step,	(getHeight(floorX,i+step)+getHeight(ceilX,i+step))/2 ,		(float)i+step);
			} else if (onRightEdge && i != ceilZ) {
				glVertex3f((float)j+step,	(getHeight(j+step,floorZ)+getHeight(j+step,ceilZ))/2 ,		(float)i+step);
			} else {
				glVertex3f((float)j+step,	getHeight(j+step,i+step),		(float)i+step);
			}	

			//index = (((i+step)*height)+j);
			//glColor3ub((GLubyte)ucColorData[0][index],(GLubyte)ucColorData[1][index],(GLubyte)ucColorData[2][index]);
			glTexCoord2f((float)1-iStepped_d_xsize,(float)j_d_ysize); 
			if (onTopEdge && j != floorX) {
				glVertex3f((float)j,	(getHeight(floorX,i+step)+getHeight(ceilX,i+step))/2 ,		(float)i+step);
			} else if (onLeftEdge && i != ceilZ) {
				glVertex3f((float)j,	(getHeight(j,floorZ)+getHeight(j,ceilZ))/2 ,		(float)i+step);
			} else {
				glVertex3f((float)j,	getHeight(j,i+step),		(float)i+step);
			}

			
			
	glEnd();
		}
	}
}
#endif 

void Terrain::renderWater() {
	Vector p = ourCamera->GetPosition();
	glBegin(GL_QUADS);	
		glColor4f(0,0,0,1);	
		glVertex3f(-500 - p.x,	0.25f,	-500 - p.z);
		glVertex3f(-500 - p.x,	0.25f,	 500 + p.z);
		glVertex3f( 500 + p.x,	0.25f,	 500 + p.z);
		glVertex3f( 500 + p.x,	0.25f,	-500 - p.z);
	glEnd();

	// draw water surface
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,waterTexture);

	glBegin(GL_QUADS);
		float a=sin(timer->time/25);
		float b=cos(timer->time/25);
		float c = timer->time*2;

		glColor4f(1,1,1,0.75f);

		int tilesize = 50;
		for (int xi=-500 + (int)p.x;xi<500  + p.x;xi+=tilesize) { 
			for (int yi=-500 + (int)p.z;yi<500  + p.z;yi+=tilesize) {
				
				glTexCoord2f((xi)*0.1f + a,(yi)*0.1f+b);						glVertex3f((float)xi +0,	fAvgWaterHeight,(float)yi+	0);
				glTexCoord2f((xi)*0.1f + a,(yi+tilesize)*0.1f+b);				glVertex3f((float)xi +0,	fAvgWaterHeight,(float)yi+tilesize);
				glTexCoord2f((xi + tilesize)*0.1f + a,(yi+tilesize)*0.1f+b);	glVertex3f((float)xi +tilesize,	fAvgWaterHeight,(float)yi+	tilesize);
				glTexCoord2f((xi + tilesize)*0.1f + a,(yi)*0.1f+b);				glVertex3f((float)xi +tilesize,	fAvgWaterHeight,(float)yi+	0);
			
			}
		}	
		
	glEnd();


	glBindTexture(GL_TEXTURE_2D,waveTexture);	
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	// render waves washing ashore
	tilesize = 4;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_FOG);
	glDepthMask(0);
	for (int i=0,s=(int)waves.size();i<s;i++) {
	glPushMatrix();
		Vector p = waves[i].waveBreakPosition;
		Vector d =- waves[i].waveBreakDirection;

		float b = timer->time+(waves[i].waveBreakPosition.x+waves[i].waveBreakPosition.z)/30;
		float amt = -7 + (sin(b) * 5);

		p += (d*amt);

		glTranslatef(p.x,0.1f,p.z);
		glRotatef(atan2f(d.x,d.z)/(3.1415926f/180.0f),0,1,0);
		glBegin(GL_QUADS);	
			glColor4f(1.0f,1.0f,1.0f,((sin(b)+1)*(cos(b)+1))/4);			
			glTexCoord2f(0,1.0f);			glVertex3f((float)-tilesize,	fAvgWaterHeight,	0);
			glTexCoord2f(0,0);		glVertex3f((float)-tilesize,	fAvgWaterHeight,	(float)tilesize);
			glTexCoord2f(1.0f,0);	glVertex3f((float)tilesize,	fAvgWaterHeight,	(float)tilesize);
			glTexCoord2f(1.0f,1.0f);		glVertex3f((float)tilesize,	fAvgWaterHeight,	0);
		glEnd();
	glPopMatrix();
	}	
	glPopAttrib();

	glDisable(GL_BLEND);
}

Vector Terrain::RayTest(Vector start, Vector direction, float length, float step, bool accurate = true) {
	float progress = 0;

	if (!this || fHeightData == 0) return start;

	direction.normalize();
	Vector testpoint = start;

	do {
        testpoint+=direction * step;
		progress += step;
	} while ((progress < length) && getInterpolatedHeight(testpoint.x,testpoint.z) < testpoint.y);
	
	if (accurate) {
		int accuracy = 10;
		float newStep = step/accuracy;
		testpoint -= direction * step;	// bring it back 

		// and retest more accurately
		for (int i=0;i<accuracy;i++) {
			testpoint += direction * newStep;
			if (getInterpolatedHeight(testpoint.x,testpoint.z) < testpoint.y) 
				break;
		}
	}

	return testpoint;
}

unsigned char Terrain::getContents(int x, int y) { 
	assert(x>=0 && x<width && y>=0 && y<height); 
	
	return bTerrainContents[(y*height)+x]; 
}

void Terrain::setContents(int x, int y, int w, int h, unsigned char contents) {
	for (int u=-w/2;u<w/2;u++) {
		for (int v=-h/2;v<h/2;v++) {
			setContents(x+u,y+v,contents);
		}
	}
}

void Terrain::setContents(int x, int y, unsigned char contents) { 
	if (!this || !this->bTerrainContents) return;
	bTerrainContents[(y*height)+x] = contents; 

}

Vector Terrain::getNormal(int x, int y) {
	if (x < 0 || x >= width || y < 0 || y >= height) 
		return Vector(0,0,0);

	return vNormals[(y*height)+x];
}

float Terrain::getHeight(int x, int y) {
#if !defined USE_PROCEDURAL_TERRAIN_INSTEAD
	if (x < 0 || x >= width || y < 0 || y >= height) 
		return 0;
	
	return fHeightData[(y*height)+x];
#else
	return ((sinf(x / 24.0f) * cosf(x / 14.0f) * sinf(x / 9.0f)) * (cosf(y / 24.0f) * sinf(y / 14.0f) * cosf(y / 9.0f))) * 32;
#endif
}


float Terrain::getInterpolatedHeight(float xReal, float yReal) {
	if (!this) return 0;

	if (xReal < 1 || xReal >= width-1 || yReal < 1 || yReal >= height-1) {
		return 1.0f;
	}

	int x0 = int(xReal), y0 = int(yReal);
	float dx = xReal-x0, dy = yReal-y0, omdx = 1-dx, omdy = 1-dy;
	float result =	omdx*omdy*getHeight(x0,y0) +
					omdx*dy*getHeight(x0,y0+1) +
					dx*omdy*getHeight(x0+1,y0) +
					dx*dy*getHeight(x0+1,y0+1);

	if (result < fAvgWaterHeight)
		return fAvgWaterHeight;
	
	return result;
}


/*************************** a star crap ***********************/

float Terrain::getGscore(Vector start, Vector p, Vector end) {
	if (getContents((int)p.x,(int)p.z) != 0) {
		return 1000;
	}
	return (float)((p-end).len()+(p-start).len());
}

void Terrain::ResetNodes() {
	for (int y=0;y<height;y++) {
		for (int x=0;x<width;x++) {
			ASnodes[(y*width)+x].G = -1;
			ASnodes[(y*width)+x].parent = NULL;
			ASnodes[(y*width)+x].position = Vector((float)x,0,(float)y);
			ASnodes[(y*width)+x].H = 1 + getNormal(x,y).Dot(Vector(0,1,0));
		}
	}
	openList.clear();
	closedList.clear();
}

AS_Node * Terrain::AStarSearch(Vector start, Vector end) {
	try {

	start.y = 0;
	end.y = 0;
	int c=0;

	ResetNodes();

	if (start.x < 0 || start.z < 0 || start.x > width || start.z > height ||
		end.x < 0 || end.z < 0 || end.x > width || end.z > height)
		return 0;

	if (/*getContents((int)start.x,(int)start.z) ||*/ getContents((int)end.x,(int)end.z)) {
		return NULL;
	}

	endNode = &ASnodes[((int)end.z * width)+(int)end.x];
	startNode = &ASnodes[((int)start.z * width)+(int)start.x];
	openList.push_back(startNode);
	
	AS_Node *n = (AS_Node*)1;
	while (n>0 && c++ < 300) {
		if (!CloseNode((n = getClosestOpenNode(end))))
			break;
	}
	
	// trace back from endNode through parents
	return endNode;

	} catch(...) {
		console->Printf("Exception caught in A-Star search code. :(");
		return 0;
	}
}


AS_Node *Terrain::getClosestOpenNode(Vector p) {
	if (openList.size() > 0)
		return openList.back();
	
	return (AS_Node*)NULL;
}

bool compare_nodes(AS_Node *a, AS_Node*b) {
	if (a && b && (a->G*a->H) > (b->G*b->H)) {
		return true;
	} else {
		return false;
	}
}

// CloseNode should be called on the closest open node -- will return false if called on end node
bool Terrain::CloseNode(AS_Node *openNode) {
	Vector p;

	if (!openNode) return false;

	if (openNode == endNode) {
		return false;
	}

	int nSides = 4;
	for (int i=0;i<nSides;i++) {
		switch (i) {
		case 0:
			p = openNode->position+Vector(2,0,0);
			break;
		case 1:
			p = openNode->position+Vector(0,0,2);
			break;
		case 2:
			p = openNode->position+Vector(-2,0,0);
			break;
		case 3:
			p = openNode->position+Vector(0,0,-2);
			break;
		}
		if (p.x < 0 || p.z < 0 || p.x > width || p.z > height) continue;
		unsigned char contents = getContents((int)p.x,(int)p.z);
		if (!(contents&TC_UNWALKABLE)) {	// if we can walk here...
			AS_Node *sp = &ASnodes[((unsigned int)p.z * width)+(unsigned int)p.x];
			if (sp && sp->parent == NULL) {
				sp->parent=openNode;
		//		sp->position = p;
				sp->G = getGscore(startNode->position,sp->position,endNode->position);
		//		sp->H = 1.0f + getNormal((int)p.x,(int)p.z).Dot(Vector(0,1,0));	// larger slopes are harder to traverse...
				openList.push_back(sp);
			}
		}
	}
	openList.sort(compare_nodes);
	if (openList.size() > 1) {
		openList.remove(openNode);
	}
	closedList.push_back(openNode);

	return true;
}













