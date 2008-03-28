#include ".\graphics.h"
#include "Variables.h"
#include "Camera.h"
#include "console.h"
#include "SDL_image.h"

extern Camera *camera;
extern gamevars *vars;	
extern Console *console;

#pragma comment(lib,"sdl.lib")
#pragma comment(lib,"sdlmain.lib")
#pragma comment(lib,"sdl_image.lib")
#pragma comment(lib,"opengl32.lib")

PFNGLMULTITEXCOORD2FARBPROC	glMultiTexCoord2fARB	= NULL;
PFNGLACTIVETEXTUREARBPROC	glActiveTextureARB	= NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB= NULL;

void DrawCube(Vector pos, Vector scale) {
	glPushMatrix();
	glTranslatef(pos.x,pos.y,pos.z);
	glScalef(scale.x/2,scale.y/2,scale.z/2);

	glBegin(GL_QUADS);							
		// top
		glTexCoord2f(1.0f,1.0f);		glVertex3f( 1.0f, 1.0f,-1.0f);		
		glTexCoord2f(0.0f,1.0f);		glVertex3f(-1.0f, 1.0f,-1.0f);		
		glTexCoord2f(0.0f,0.0f);		glVertex3f(-1.0f, 1.0f, 1.0f);		
		glTexCoord2f(1.0f,0.0f);		glVertex3f( 1.0f, 1.0f, 1.0f);		
		
		// bottom
		glTexCoord2f(1.0f,1.0f);		glVertex3f( 1.0f,-1.0f, 1.0f);		
		glTexCoord2f(0.0f,1.0f);		glVertex3f(-1.0f,-1.0f, 1.0f);		
		glTexCoord2f(0.0f,0.0f);		glVertex3f(-1.0f,-1.0f,-1.0f);		
		glTexCoord2f(1.0f,0.0f);		glVertex3f( 1.0f,-1.0f,-1.0f);		
			
		// back
		glTexCoord2f(0.0f,0.0f);		glVertex3f( 1.0f, 1.0f, 1.0f);		
		glTexCoord2f(1.0f,0.0f);		glVertex3f(-1.0f, 1.0f, 1.0f);		
		glTexCoord2f(1.0f,1.0f);		glVertex3f(-1.0f,-1.0f, 1.0f);		
		glTexCoord2f(0.0f,1.0f);		glVertex3f( 1.0f,-1.0f, 1.0f);		
	
		// front
		glTexCoord2f(1.0f,1.0f);		glVertex3f( 1.0f,-1.0f,-1.0f);		
		glTexCoord2f(0.0f,1.0f);		glVertex3f(-1.0f,-1.0f,-1.0f);		
		glTexCoord2f(0.0f,0.0f);		glVertex3f(-1.0f, 1.0f,-1.0f);		
		glTexCoord2f(1.0f,0.0f);		glVertex3f( 1.0f, 1.0f,-1.0f);		
	
		// left
		glTexCoord2f(0.0f,0.0f);		glVertex3f(-1.0f, 1.0f, 1.0f);		
		glTexCoord2f(1.0f,0.0f);		glVertex3f(-1.0f, 1.0f,-1.0f);		
		glTexCoord2f(1.0f,1.0f);		glVertex3f(-1.0f,-1.0f,-1.0f);		
		glTexCoord2f(0.0f,1.0f);		glVertex3f(-1.0f,-1.0f, 1.0f);		
	
		//right
		glTexCoord2f(0.0f,0.0f);		glVertex3f( 1.0f, 1.0f,-1.0f);		
		glTexCoord2f(1.0f,0.0f);		glVertex3f( 1.0f, 1.0f, 1.0f);		
		glTexCoord2f(1.0f,1.0f);		glVertex3f( 1.0f,-1.0f, 1.0f);		
		glTexCoord2f(0.0f,1.0f);		glVertex3f( 1.0f,-1.0f,-1.0f);		
	glEnd();		
	glPopMatrix();
}


void DrawQuad(Vector pos, Vector scale) {
	glPushMatrix();
	glTranslatef(pos.x,pos.y,pos.z);
	glScalef(scale.x/2,scale.y/2,scale.z/2);

	glBegin(GL_QUADS);							
		// top
		glTexCoord2f(1.0f,1.0f);		glVertex3f( 1.0f, 1.0f,-1.0f);		
		glTexCoord2f(0.0f,1.0f);		glVertex3f(-1.0f, 1.0f,-1.0f);		
		glTexCoord2f(0.0f,0.0f);		glVertex3f(-1.0f, 1.0f, 1.0f);		
		glTexCoord2f(1.0f,0.0f);		glVertex3f( 1.0f, 1.0f, 1.0f);			
	glEnd();		
	glPopMatrix();
}


int skybox;

void DrawSkybox() {
	glPushMatrix();
		glLoadIdentity();
		glScalef(10,10,10);
		glRotatef(camera->GetRotation().x,1,0,0);
		glRotatef(camera->GetRotation().y,0,1,0);
		glRotatef(camera->GetRotation().z,0,0,1);

		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);	
		glDepthMask(0);
		glBindTexture(GL_TEXTURE_2D,skybox);

		glColor4f(1,1,1,1);

		glBegin(GL_QUADS);		
			// top
			glTexCoord2f(0.249f,0.249f);		glVertex3f( 1.0f, 1.0f,-1.0f);		
			glTexCoord2f(0.001f,0.249f);		glVertex3f(-1.0f, 1.0f,-1.0f);		
			glTexCoord2f(0.001f,0.001f);		glVertex3f(-1.0f, 1.0f, 1.0f);		
			glTexCoord2f(0.249f,0.001f);		glVertex3f( 1.0f, 1.0f, 1.0f);		
			
			// bottom
			glTexCoord2f(0.249f,0.749f);		glVertex3f( 1.0f,-1.0f, 1.0f);		
			glTexCoord2f(0.001f,0.749f);		glVertex3f(-1.0f,-1.0f, 1.0f);		
			glTexCoord2f(0.001f,0.501f);		glVertex3f(-1.0f,-1.0f,-1.0f);		
			glTexCoord2f(0.249f,0.501f);		glVertex3f( 1.0f,-1.0f,-1.0f);		
				
			// back
			glTexCoord2f(0.501f,0.251f);		glVertex3f( 1.0f, 1.0f, 1.0f);		
			glTexCoord2f(0.749f,0.251f);		glVertex3f(-1.0f, 1.0f, 1.0f);		
			glTexCoord2f(0.749f,0.499f);		glVertex3f(-1.0f,-1.0f, 1.0f);		
			glTexCoord2f(0.501f,0.499f);		glVertex3f( 1.0f,-1.0f, 1.0f);		
		
			// front
			glTexCoord2f(0.249f,0.499f);		glVertex3f( 1.0f,-1.0f,-1.0f);		
			glTexCoord2f(0.001f,0.499f);		glVertex3f(-1.0f,-1.0f,-1.0f);		
			glTexCoord2f(0.001f,0.251f);		glVertex3f(-1.0f, 1.0f,-1.0f);		
			glTexCoord2f(0.249f,0.251f);		glVertex3f( 1.0f, 1.0f,-1.0f);		
		
			// left
			glTexCoord2f(0.751f,0.251f);		glVertex3f(-1.0f, 1.0f, 1.0f);		
			glTexCoord2f(0.999f,0.251f);		glVertex3f(-1.0f, 1.0f,-1.0f);		
			glTexCoord2f(0.999f,0.499f);		glVertex3f(-1.0f,-1.0f,-1.0f);		
			glTexCoord2f(0.751f,0.499f);		glVertex3f(-1.0f,-1.0f, 1.0f);		
		
			//right
			glTexCoord2f(0.251f,0.251f);		glVertex3f( 1.0f, 1.0f,-1.0f);		
			glTexCoord2f(0.499f,0.251f);		glVertex3f( 1.0f, 1.0f, 1.0f);		
			glTexCoord2f(0.499f,0.499f);		glVertex3f( 1.0f,-1.0f, 1.0f);		
			glTexCoord2f(0.251f,0.499f);		glVertex3f( 1.0f,-1.0f,-1.0f);		
		glEnd();		
		glDepthMask(1);
		glEnable(GL_DEPTH_TEST);
	glPopMatrix();
}
/*
void setShaders() {	
	char *vs,*fs;

	v = glCreateShader(GL_VERTEX_SHADER);
	vs = textFileRead("toon.vert");
	const char * vv = vs;
	glShaderSource(v, 1, &vv,NULL);
	free(vs);
	glCompileShader(v);

	f = glCreateShader(GL_FRAGMENT_SHADER);	
	fs = textFileRead("toon.frag");
	const char * ff = fs;
	glShaderSource(f, 1, &ff,NULL);
	free(fs);
	glCompileShader(f);


	p = glCreateProgram();
	
	glAttachShader(p,v);
	glAttachShader(p,f);

	glLinkProgram(p);
	glUseProgram(p);
}
*/
Graphics::Graphics(void)
{
}

Graphics::~Graphics(void)
{
}

#define GL_FOG_COORDINATE_SOURCE_EXT	0x8450					
#define GL_FOG_COORDINATE_EXT		0x8451	
typedef void (APIENTRY * PFNGLFOGCOORDFEXTPROC) (GLfloat coord);
PFNGLFOGCOORDFEXTPROC glFogCoordfEXT = NULL;

bool Graphics::init() {
	SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );

    /* Fetch the video info */
    const SDL_VideoInfo *videoInfo = SDL_GetVideoInfo( );
	
    if ( !videoInfo )
	{
	    fprintf( stderr, "Video query failed: %s\n",
		     SDL_GetError( ) );
	    return false;
	}

    int videoFlags  = SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE | SDL_RESIZABLE;
	if (vars->getIntValue("screen_fullscreen") == 1) { 
		videoFlags |= SDL_FULLSCREEN;    
	}
    videoFlags |= ( videoInfo->hw_available )?SDL_HWSURFACE:SDL_SWSURFACE;
    if ( videoInfo->blit_hw ) videoFlags |= SDL_HWACCEL;

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );


    /* Verify there is a surface */
    if (!SDL_SetVideoMode(vars->getIntValue("screen_width"), vars->getIntValue("screen_height"), 24, videoFlags))
	{
	    fprintf( stderr,  "Video mode set failed: %s\n", SDL_GetError( ) );
	    return false;
	}

	SDL_WM_SetCaption("MOTORS Engine", NULL );

	glClearColor(0.8f,0.8f,0.8f, 1.0f);  
	glClear(GL_COLOR_BUFFER_BIT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   
	glShadeModel(GL_SMOOTH);                

 	glEnable(GL_DEPTH_TEST);                             
	glDepthFunc(GL_LEQUAL);                              
	glClearDepth(9999.0f); 
   	
	glEnable(GL_TEXTURE_2D);	

	glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) wglGetProcAddress("glMultiTexCoord2fARB");
	glActiveTextureARB   = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
	glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");
	glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC) wglGetProcAddress("glFogCoordfEXT");

	GLfloat fogColor[4]= {0.8f, 0.8f, 0.8f, 1};	
	glFogi(GL_FOG_MODE, GL_EXP2);				// Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);			// Set Fog Color
	glFogf(GL_FOG_DENSITY, 0.005f);				// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_NICEST);				// Fog Hint Value
	glFogf(GL_FOG_START, 200.0f);				// Fog Start Depth
	glFogf(GL_FOG_END, 300.0f);					// Fog End Depth
	glEnable(GL_FOG);	

//	SDL_ShowCursor(0);
//	SDL_WM_GrabInput(SDL_GRAB_ON);

	text.init();

	skybox = LoadTexture("data/topographical/skybox.JPG");

	console->Printf("Finished renderer initialization");

	return true;
}

void Graphics::pause() {

}

bool Graphics::stop() {
	SDL_Quit();
	console->Printf("Renderer shutdown complete");

	return true;
}


void Graphics::run() {
	SDL_GL_SwapBuffers();	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	DrawSkybox();
	glLoadIdentity();	
//	glScalef(0.001f,0.001f,0.001f);
}


int Graphics::LoadTexture(const char *filename) {
	GLuint texture;

	SDL_Surface* imageFile = IMG_Load(filename);

	if (!imageFile) {
		console->Printf("IMG_Load(%s) failed. reason: %s", filename,IMG_GetError());	
		return -1;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
 	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);
 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 		
	gluBuild2DMipmaps(GL_TEXTURE_2D,3,imageFile->w, imageFile->h, GL_RGB, GL_UNSIGNED_BYTE,imageFile->pixels);
 
	SDL_FreeSurface(imageFile);

	return (int)texture;
}