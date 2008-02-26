#include ".\terrain.h"
#include "graphics.h"
#include "timer.h"
#include "camera.h"

extern Camera *camera;
extern Graphics *renderer;

Terrain::Terrain(void) {
	size = Vector(0,0,0);
}

Terrain::~Terrain(void) {
	if (bHeightData) {
		delete [] bHeightData;
	}
	if (bTerrainContents) {
		delete [] bTerrainContents;
	}
}

bool Terrain::LoadHeightMap(const char * filename) {
	FILE *fmap = fopen(filename,"rb");

	fAvgWaterHeight = 4;
	SunDirection = Vector(0.65f,0.35f,0);

	if (!fmap) {
		// errrror
		return false;
	}

	fseek(fmap,0,SEEK_END);
	DWORD filesize = ftell(fmap);
	fseek(fmap,0,SEEK_SET);
	int dimensions = (int)sqrtf(filesize);
	width = dimensions;
	height = dimensions;
	int size = filesize;
	terrainName = filename;

	if (!(bTerrainContents = new unsigned char[size])) {
		// error
		return false;
	}
	ZeroMemory(bTerrainContents,size);
	if (!(bHeightData = new unsigned char[size])) {
		// error
		return false;
	}

	if (!(fSmoothedHeightData = new float[size])) {
		// error
		return false;
	}

	if (!(fWaterHeights = new float[size])) {
		return false;
	}
	for (int i=0;i<size;i++) { fWaterHeights[i] = fAvgWaterHeight; }

	if (!(ucColorData[0] = new unsigned char[size]) || 
		!(ucColorData[1] = new unsigned char[size]) || 
		!(ucColorData[2] = new unsigned char[size])) {
		return false;
	}

	if (!(vNormals = new Vector[size])) {
		// error
		return false;
	}

	if (!(ASnodes = new AS_Node[size])) {
		// error
		return false;
	}
	
	if (feof(fmap)) {
		// error - no terrain data
		return false;
	}

	fread(bHeightData,1,size,fmap);
	fclose(fmap);

	SmootheTerrain();
	CalculateNormals();
	CalculateColorData();
	
	// dont need compressed terrain data any more
	delete [] bHeightData;
	bHeightData = 0;

	waterReflection = renderer->LoadTexture("data/topographical/reflection.bmp");

	// set up the terrain contents (this should probably be its own file, but for now we're just generating it and checking for water

	memset(bTerrainContents,0,size);

	for (int y=1;y<height-1;y++) {
		for (int x=1;x<width-1;x++) {
			if (getHeight(x,y) <= fAvgWaterHeight) {
				bTerrainContents[(y*height)+x] |= TC_WATER;
			}
		}
	}

	CalculateWaveData();
	return true;
}


void Terrain::SmootheTerrain() {
	memset(fSmoothedHeightData,0,width*height);

	for (int y=1;y<height-1;y++) {
		for (int x=1;x<width-1;x++) {
			float blocksum = (float)(	(float)bHeightData[((y-1)*height)+(x-1)]	+ (float)bHeightData[((y-1)*height)+x]		+ (float)bHeightData[((y-1)*height)+(x+1)] + 
										(float)bHeightData[(y*height)+(x-1)]		+ (float)bHeightData[(y*height)+x]			+ (float)bHeightData[(y*height)+(x+1)] + 
										(float)bHeightData[((y+1)*height)+x-1]		+ (float)bHeightData[((y+1)*height)+x]		+ (float)bHeightData[((y+1)*height)+(x+1)]);
			int pos = (y*height)+x;
			fSmoothedHeightData[pos] = (blocksum/9) / 16;
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
			ucColorData[0][index] = ucColorData[1][index] = ucColorData[2][index] = 127+((unsigned char)(angle * 127));

			Vector test = RayTest(sp,nd,30,1,false);
			Vector end = sp+(nd*30);
			if ( (end - test).len() > 1) {
				// shaded
				ucColorData[0][index] = 64;
				ucColorData[1][index] = 64;
				ucColorData[2][index] = 64;
			}
		}
	}

}


void Terrain::SetTexture(int mT, int wT) {
	mainTexture = mT;
	waterTexture = wT;
}

void Terrain::CalculateWaveData() {
	waveTexture = renderer->LoadTexture("data\\topographical\\wave.bmp");
	int tilesize = 3;
	for (int xi=0;xi<width;xi+=tilesize) {
		for (int yi=0;yi<height;yi+=tilesize) {
			if (getHeight(xi,yi) <= fAvgWaterHeight) {
				Terrain::WaveData wd;
	
				float bestAngle = -1;
				float highestHeight = 0;
				int testStep = 10;
				float testDist = 2;

				for (int angle = 0;angle<360; angle+= testStep) {
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
	
	renderLand(20,1000,290);
	renderLand(8,300,90);
	renderLand(4,100,40);
	renderLand(2,50,0);
	renderWater();
}

void Terrain::renderLand(int step, int maxd, int mind) {
	int i,j;
	float xsize=(float)width;
	float ysize=(float)height;
	
	Vector cp = RayTest(camera->GetPosition(),camera->GetView(),70,4,false);
	int startX=cp.x - maxd,startY=cp.z - maxd,endX=cp.x + maxd,endY=cp.z + maxd;	
	if (startX < 1) startX = 1;
	if (startY < 1) startY = 1;
	if (endX > width-1) endX = width-1;
	if (endY > height-1) endY = height-1;

	startX -= (startX%step);
	startY -= (startY%step);
		
	glBegin(GL_QUADS);
	for(i=startY; i<endY-step; i+=step)
	{
		for(j=startX; j<endX-step; j+=step)
		{
			if (getContents(j,i) == TC_WATER && getContents(j+step,i+step) == TC_WATER && getContents(j+step,i) == TC_WATER && getContents(j,i+step) == TC_WATER) {
				continue;
			}
	
			if (mind != 0 && j > cp.x-mind && j < cp.x+mind && i > cp.z-mind && i < cp.z+mind) {
				continue;
			}

			float i_d_xsize = i / xsize;
			float iStepped_d_xsize = (i+step) / xsize;
			float j_d_ysize = j / ysize;
			float jStepped_d_ysize = (j+step) / ysize;

			int index = ((i*height)+j);
			glColor3ub((GLubyte)ucColorData[0][index],(GLubyte)ucColorData[1][index],(GLubyte)ucColorData[2][index]);
			glTexCoord2f((float)1-i_d_xsize,(float)j_d_ysize); 
			glVertex3f((float)j,	fSmoothedHeightData[index],		(float)i);

			index = ((i*height)+(j+step));
			glColor3ub((GLubyte)ucColorData[0][index],(GLubyte)ucColorData[1][index],(GLubyte)ucColorData[2][index]);
			glTexCoord2f((float)1-i_d_xsize,(float)jStepped_d_ysize); 
			glVertex3f((float)j+step,	fSmoothedHeightData[index],		(float)i);
			
			index = (((i+step)*height)+(j+step));
			glColor3ub((GLubyte)ucColorData[0][index],(GLubyte)ucColorData[1][index],(GLubyte)ucColorData[2][index]);
			glTexCoord2f((float)1-iStepped_d_xsize,(float)jStepped_d_ysize); 
			glVertex3f((float)j+step,	fSmoothedHeightData[index],		(float)i+step);
			
			index = (((i+step)*height)+j);
			glColor3ub((GLubyte)ucColorData[0][index],(GLubyte)ucColorData[1][index],(GLubyte)ucColorData[2][index]);
			glTexCoord2f((float)1-iStepped_d_xsize,(float)j_d_ysize); 
			glVertex3f((float)j,	fSmoothedHeightData[index],		(float)i+step);
		}
	}
	glEnd();
}


void Terrain::renderWater() {
	glBegin(GL_QUADS);	
		glColor4f(0,0,0,1);			
		glVertex3f(-500,0.25f,	-500);
		glVertex3f(-500,0.25f,	1000);
		glVertex3f(500,	0.25f,	1000);
		glVertex3f(500,	0.25f,	-500);
	glEnd();

	// draw water surface
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,waterTexture);

	glBegin(GL_QUADS);
		float a=sin(timer->time/25);
		float b=cos(timer->time/25);
		float c = timer->time*2;

		glColor4f(1,1,1,1);

		int tilesize = 50;
		for (int xi=-500;xi<width*2;xi+=tilesize) {
			for (int yi=-500;yi<height*2;yi+=tilesize) {

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
		float amt = -1 + sin(b);

		p += (d*amt);
		glTranslatef(p.x,0.1f,p.z);
		glRotatef(atan2f(d.x,d.z)/(3.1415926f/180),0,1,0);
		glBegin(GL_QUADS);	
			glColor4f(1.0f,1.0f,1.0f,0.5f+(cos(b)+1)/4);			
			glTexCoord2f(0,0);			glVertex3f(-tilesize,	fAvgWaterHeight,	0);
			glTexCoord2f(0,1.0f);		glVertex3f(-tilesize,	fAvgWaterHeight,	(float)tilesize);
			glTexCoord2f(1.0f,1.0f);	glVertex3f(tilesize,	fAvgWaterHeight,	(float)tilesize);
			glTexCoord2f(1.0f,0);		glVertex3f(tilesize,	fAvgWaterHeight,	0);
		glEnd();
	glPopMatrix();
	}	
	glPopAttrib();

	glDisable(GL_BLEND);
}

Vector Terrain::RayTest(Vector start, Vector direction, float length, float step, bool accurate = true) {
	float progress = 0;

	if (!this || this->fSmoothedHeightData == 0) return start;

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
	if (x < 0 || x >= width || y < 0 || y >= height) 
		return 0;

	return fSmoothedHeightData[(y*height)+x];
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
	return (float)((p-end).len2()+(p-start).len2());
}

void Terrain::ResetNodes() {
	for (int y=0;y<height;y++) {
		for (int x=0;x<width;x++) {
			ASnodes[(y*width)+x].G = -1;
			ASnodes[(y*width)+x].parent = NULL;
			ASnodes[(y*width)+x].position = Vector((float)x,0,(float)y);
			ASnodes[(y*width)+x].H = 1;
		}
	}
	openList.clear();
	closedList.clear();
}

AS_Node * Terrain::AStarSearch(Vector start, Vector end) {
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
			p = openNode->position+Vector(1,0,0);
			break;
		case 1:
			p = openNode->position+Vector(0,0,1);
			break;
		case 2:
			p = openNode->position+Vector(-1,0,0);
			break;
		case 3:
			p = openNode->position+Vector(0,0,-1);
			break;
		}
		if (p.x < 0 || p.z < 0 || p.x > width || p.z > height) continue;
		unsigned char contents = getContents((int)p.x,(int)p.z);
		if (!(contents&TC_UNWALKABLE)) {	// if we can walk here...
			AS_Node *sp = &ASnodes[((unsigned int)p.z * width)+(unsigned int)p.x];
			if (sp && sp->parent == NULL) {
				sp->parent=openNode;
				sp->position = p;
				sp->G = getGscore(startNode->position,sp->position,endNode->position);
				sp->H = 1.0f;//+getNormal((int)p.x,(int)p.z).Dot(Vector(0,1,0));	// larger slopes are harder to traverse...
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













