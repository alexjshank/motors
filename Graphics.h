#include "task.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library

#include "vector.h"
#include "printer.h"	// text printing (ie- font rendering)


#ifndef __GRAPHICS__H__
#define __GRAPHICS__H__

class Graphics : public Task
{
public:
	Graphics(void);
	~Graphics(void);

	bool init();
	void pause();
	bool stop();
	void run();

	int LoadTexture(const char *filename);

	Printer text;
};

void DrawCube(Vector pos, Vector scale);
void DrawQuad(Vector,Vector);
void DrawSkybox();

extern PFNGLFOGCOORDFEXTPROC glFogCoordfEXT;
extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
extern PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;

#endif
