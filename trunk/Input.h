#include "task.h"
#include "Vector.h"

#ifndef _INPUT_
#define _INPUT_

enum possibleInputContexts {
	NormalInput = 0,
	ConsoleInput,
	PersonelMenu,
	BuildMenu,
	EditMode,
	Sailing
};

class Input : public Task
{
public:
	Input(void);
	~Input(void);

	void run();
	
	void SetKeyState(int key, int state);
	int GetKeyState(int key);
	bool GetKeyDown(int key);
	bool GetKeyPressed(int key);
	bool GetKeyReleased(int key);

	bool GetMButtonState(int button) { return (mousebuttons[button] == 1); }
	void SetMButtonState(int button, bool state) { mousebuttons[button] = state; }
	bool GetMButtonPushed(int button) { return ((GetMButtonState(button)) && lastbuttons[button] != mousebuttons[button]); }
	bool GetMButtonReleased(int button) { return ((!GetMButtonState(button)) && lastbuttons[button] != mousebuttons[button]); }

	int laststate[512];
	int keystate[512];

	int lastbuttons[6];
	int mousebuttons[6];

	Vector mousePosition;
	Vector mouseAbsolute;
	Vector mouseMovement;
	Vector mouse3dPosition;
	Vector mouseVector;

	int inputContext;

#define LeftMouseClick 1
#define MiddleMouseClick 2
#define RightMouseClick 3

private:
	int priority;
};

#endif