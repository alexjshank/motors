#include ".\input.h"
#include "graphics.h"
#include "camera.h"

Input::Input(void)
{
	priority = TP_ENGINE;
	ZeroMemory((void *)&keystate,sizeof(keystate));
	ZeroMemory((void *)&laststate,sizeof(laststate));
	mouseMovement = Vector(0,0,0);
	mouseAbsolute = Vector(0,0,0);
	mousePosition = Vector(0,0,0);
}

Input::~Input(void)
{
}

extern Graphics *renderer;
extern Camera *camera;

void Input::run() {
	SDL_Event event;
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY;
	GLdouble posX, posY, posZ;

	memcpy((void *)&laststate,(void *)&keystate,sizeof(keystate));
	memcpy((void *)&lastbuttons,(void *)&mousebuttons,sizeof(mousebuttons));
	mouseMovement = Vector(0,0,0);

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			exit(1);
			break;
		case SDL_KEYDOWN:
			SetKeyState(event.key.keysym.sym,1);
			break;
		case SDL_KEYUP:
			SetKeyState(event.key.keysym.sym,0);
			break;
		case SDL_MOUSEMOTION:
			mouseMovement.x = event.motion.xrel;
			mouseMovement.y = event.motion.yrel;
			mouseAbsolute.x = event.motion.x;
			mouseAbsolute.y = event.motion.y;
			mousePosition += mouseMovement;

			glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
			glGetDoublev( GL_PROJECTION_MATRIX, projection );
			glGetIntegerv( GL_VIEWPORT, viewport );

			winX = (float)mouseAbsolute.x;
			winY = (float)viewport[3] - (float)mouseAbsolute.y;
	//		glReadPixels( int(winX), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
			gluUnProject( winX, winY, 0.999985f, modelview, projection, viewport, &posX, &posY, &posZ);
			
			mouse3dPosition = Vector((float)posX, (float)posY, (float)posZ);
			mouseVector = mouse3dPosition - camera->GetPosition();
			mouseVector.normalize();
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == 4) {
				mouseAbsolute.z--;
			} else if (event.button.button == 5) {
				mouseAbsolute.z++;
			} else {
				mousebuttons[event.button.button] = 1;
			}
			
			break;
		case SDL_MOUSEBUTTONUP:
			mousebuttons[event.button.button] = 0;
			break;
		}
	}
}

void Input::SetKeyState(int key, int state) {
	if (key >= 0 && key < 256 && key != SDLK_RSHIFT && key != SDLK_LSHIFT && (GetKeyState(SDLK_RSHIFT) || GetKeyState(SDLK_LSHIFT))) {
		if (isalpha(key)) key = toupper(key);
		else if (key == '1') key = '!';
		else if (key == '2') key = '@';
		else if (key == '3') key = '#';
		else if (key == '4') key = '$';
		else if (key == '5') key = '%';
		else if (key == '6') key = '^';
		else if (key == '7') key = '&';
		else if (key == '8') key = '*';
		else if (key == '9') key = '(';
		else if (key == '0') key = ')';
		else if (key == '-') key = '-';
		else if (key == '=') key = '+';
		else if (key == '\\') key = '|';
		else if (key == ',') key = '<';
		else if (key == '.') key = '>';
		else if (key == '/') key = '?';
	} 
	keystate[key] = state;
}

int Input::GetKeyState(int key) {
	return keystate[key];
}

bool Input::GetKeyDown(int key) {
	return (bool)(keystate[key] != 0);
}

bool Input::GetKeyPressed(int key) {
	return (bool)(keystate[key] != 0 && laststate[key] == 0);
}

bool Input::GetKeyReleased(int key) {
	return (bool)(keystate[key] == 0 && laststate[key] != 0);
}
/*
class BindSystem : public Task {
public:
	BindSystem();
	
	void run();
	void Bind(int key, (void *)functor);
	void Unbind(int key);

private:
};
*/