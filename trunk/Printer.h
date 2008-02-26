
#include "task.h"
#include "vector.h"


#ifndef __PRINTER__H__
#define __PRINTER__H__

#define SCREEN_MAXX 0.62f
#define SCREEN_MAXY 0.46f

class Printer {
public:
	Printer(void);
	~Printer(void);

	Vector FromMousePos(float a, float b) {  return Vector((a+SCREEN_MAXX)*64, ((-b)+SCREEN_MAXY)*32,0); }

	bool init();
	void print(float x, float y, const char *, ...);
	void printpositional(Vector position, const char *format, ...);

	bool Italic,Underline,Strikeout;
private:
	int base;
};

#endif