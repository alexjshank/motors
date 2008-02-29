#include ".\library.h"
#include "md2.h"
#include <map>
#include "graphics.h"
#include "console.h"

extern Graphics *renderer;
extern Console *console;

Library::Library(void)
{
}

Library::~Library(void)
{
}

bool Library::Import(const char *filename, int type) {
	Model_MD2 *model;
	TextureObject *texture;
	std::string name = filename;

	console->Printf("Caching resource: %s",filename);

	switch (type) {
	case 0:	// texture
		texture = new TextureObject;
		texture->texture = renderer->LoadTexture(filename);
		texture->name = filename;
		texture->type = 0;
		objects[name] = texture;
		break;

	case 1: // model
		model = new Model_MD2(filename);
		objects[name] = model;
		return true;
		break;

	case 2: // sound

		break;
	}
	return false;
}
LibraryObject * Library::Export(const char *filename) {
	std::string name = filename;

	LibraryObject * libCopy = objects[name];
	if (!libCopy) { // if we don't have this model already cached...
//		Import(filename,1);
		return 0;
	}
	return libCopy;
}