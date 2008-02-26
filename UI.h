#include "task.h"
#include "vector.h"
#include <list>
#include "graphics.h"

#ifndef _UI_H_
#define _UI_H_

class UIObject {
public:
	UIObject();
	~UIObject();

	virtual void init() {}
	virtual void render();
	virtual void renderTexture(Vector p, Vector d, int t);

	virtual bool Test(float x, float y);

	virtual void onMouseDown(float x, float y);
	virtual void onMouseUp(float x, float y);
	virtual void onMouseFocus() { }
	virtual void onMouseBlur() { }
	virtual void onMouseMove(float x, float y);

	void AttachChild(UIObject *child);
	Vector CalculateAbsolutePosition();

	bool mouseOver;
	bool mouseDown;

	bool visible;
	
	bool absPositionSet;
	bool absolute;
	Vector absoluteposition;	// sum of this and all parent's positions
	Vector position;			// x and y components used only.
	float width;
	float height;
	std::list<UIObject*> children;
	UIObject *parent;
};

#define UI_Z_COORD -2.1


class UIText : public UIObject {
public:
	UIText() {
	}

	void render();

	std::string value;
};

class UIButton : public UIObject {
public:
	UIButton() {
		enabled = true;
		pressed = false;
	}

	bool loadButtonTextures(const char *filescheme); // "data/UI/button" -> button_01.bmp, button_02.bmp, button_03.bmp
	void render();
	void onMouseUp(float x, float y);
	void onMouseDown(float x, float y);
	virtual void OnButtonPressed() { pressed = true; }
	
	bool pressed;

	bool enabled;
	int texture[3];
	enum ButtonTextureFrames {
		Disabled=0,
		Enabled,
		Pressed
	};

	std::string pressedCommand;
	std::string releasedCommand;
};

class UIWindow : public UIObject {
public:

	void init();
	void render();
	void onMouseMove(float x, float y);

	std::string title;
	UIButton closeButton;
	int windowbg;
};

class UI : public Task {
public:
	UI(void);
	~UI(void);

	bool init();
	void run();

	UIObject * CreateFromFile(const char *uifilename);

//private:
	// textures:
	int windowbg;
	int buttonbg;
	int farmbutton;
	UIObject *desktop;
};

#endif