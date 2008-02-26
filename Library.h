#include "task.h"
#include <string.h>
#include <map>

#ifndef __LIBRARY__H__
#define __LIBRARY__H__
/*
object types:
0 - Texture
1 - Model
2 - Sound

*/

class LibraryObject {
public:

	std::string name;
	int type;
};

class TextureObject : public LibraryObject {
public:
	int texture;
};

class Library :	public Task
{
public:
	Library(void);
	~Library(void);

	bool Import(const char *filename, int type);
	LibraryObject * Export(const char *filename);

private:
	std::map<std::string,LibraryObject *> objects;
};

#endif