#include ".\printer.h"
#include "Graphics.h"
#include <stdio.h>
#include <stdarg.h>	
#include <GL\gl.h>

Printer::Printer(void)
{
	Strikeout=false;
	Italic=false;
	Underline=false;
}

Printer::~Printer(void)
{
}

bool Printer::init() {
	HFONT font,oldfont;
	HDC hDC = wglGetCurrentDC();

	base = glGenLists(256);
	font = CreateFont(-14,NULL,0,0,FW_BOLD,Italic,Underline,Strikeout, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 2, DRAFT_QUALITY, FF_DONTCARE|DEFAULT_PITCH, "Arial Narrow");
	
	oldfont = (HFONT)SelectObject(hDC,font);
	wglUseFontBitmaps(hDC,0,255,base);

	SelectObject(hDC,oldfont);
	DeleteObject(font);
	return true;
}

void Printer::print(float x, float y, const char *format, ...) {
	char text[256];
	va_list args;

	if (!format) return;

	va_start(args,format);
		vsprintf(text,format,args);
	va_end(args);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glListBase(base);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LINE_STIPPLE);

		glLoadIdentity();
		glTranslatef(0,0,-1);
		glRasterPos2f(-0.615f+0.015f*x, 0.44f - 0.03f*y);
		glCallLists((GLsizei)strlen(text),GL_UNSIGNED_BYTE,text);
	glPopMatrix();
	glPopAttrib();
}

void Printer::printpositional(Vector p, const char *format, ...) {
	char text[256];
	va_list args;

	if (!format) return;

	va_start(args,format);
		vsprintf(text,format,args);
	va_end(args);

	GLdouble outX, outY, outZ;
	GLdouble modelview[16], projection[16];
	GLint viewport[4];

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	gluProject(p.x,p.y,p.z,modelview,projection,viewport,&outX,&outY,&outZ);
	
	print((float)outX/10.0f,(float)(viewport[3] - outY)/20.0f,"%s",text);
}