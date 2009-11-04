#include ".\ui.h"
#include "graphics.h"
#include "input.h"
#include "variables.h"
#include "timer.h"
#include "console.h"

extern Console *console;
extern Graphics *renderer;
extern Input *input;
extern Timer *timer;
extern gamevars *vars;
extern UI *ui;

UI::UI(void)
{
}

UI::~UI(void)
{
}

bool UI::init() {
	windowbg = renderer->LoadTexture("data/UI/wood.JPG");
	buttonbg = renderer->LoadTexture("data/UI/button_bg.JPG");

	desktop = new UIObject;
	
	return true;
}

void UI::run() {
	if (input->GetMButtonPushed(LeftMouseClick)) {
		desktop->onMouseDown(input->mouseAbsolute.x,input->mouseAbsolute.y);
	}

	if (input->mouseMovement.x || input->mouseMovement.y) {
		desktop->onMouseMove(input->mouseAbsolute.x,input->mouseAbsolute.y);
	}

	if (input->GetMButtonReleased(LeftMouseClick)) {
		desktop->onMouseUp(input->mouseAbsolute.x,input->mouseAbsolute.y);
	}

	glPushMatrix();
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);
		desktop->render();
		glEnable(GL_DEPTH_TEST);
	glPopMatrix();
}

UIObject * UI::CreateFromFile(const char *uifilename) {
	if (!uifilename || !*uifilename) return NULL;

	FILE *uifile = fopen(uifilename,"r");

	if (!uifile || feof(uifile)) {
		console->Printf("Warning: error loading UI file	%s",uifilename);
		return NULL;
	}

	UIObject * root = 0;

	while (!feof(uifile)) {
		char buffer[8192];
		char *preadpos = buffer;
		fgets(buffer,8192,uifile);

		int i;
		for (i=0; buffer[i]==' ' || buffer[i]=='\t'; i++);
		preadpos = &buffer[i];
		
		for (i=0;i<(int)strlen(buffer);i++) {
			if (buffer[i] == '#')
				buffer[i] = 0;
		}

		if (strlen(preadpos) <= 1) {
			continue;
		}

		if (buffer[0] != '\t' && buffer[0] != ' ') {	
			char objectType[32];
			char objectName[32];
			int objectX, objectY, objectW, objectH;
			char objectScript[512]; 

			if (root != 0) {
				// only one UI tree (window) can be created per ui file (since we only return one pointer to a ui object)
				console->Printf("Warning: only one root UI object definition is allowed per .ui file loaded. %s",uifilename);
				break;
			}

			sscanf(buffer,"%s\t%s\t%d\t%d\t%d\t%d\t%s",objectType,objectName,&objectX,&objectY,&objectW,&objectH,objectScript);

			if (_stricmp(objectType,"window") == 0) {
				UIWindow *window = new UIWindow;
				root = window;

				window->windowbg = ui->windowbg;
				window->position = Vector((float)objectX,(float)objectY,0);
				window->width = (float)objectW;
				window->height = (float)objectH;
				window->title = objectName;
			} else if (_stricmp(objectType,"somethingelse") == 0) {

			} else {
				console->Printf("Warning: unknown UI root object specified in UI file %s",uifilename);
				break;
			}
		} else {
			// child
			char objectType[32];
			char objectName[32];
			int objectX, objectY, objectW, objectH;
			char objectScript[512]; 

			sscanf(preadpos,"%s\t%s\t%d\t%d\t%d\t%d\t%s",objectType,objectName,&objectX,&objectY,&objectW,&objectH,objectScript);

			if (_stricmp(objectType,"button") == 0) {
				UIButton *button = new UIButton;

				button->position = Vector((float)objectX,(float)objectY,0);
				button->width = (float)objectW;
				button->height = (float)objectH;
				button->loadButtonTextures(objectName);
				button->pressedCommand = objectScript;
				root->AttachChild(button);
			} else if (_stricmp(objectType,"text") == 0) {
				UIText *text = new UIText;

				text->position = Vector((float)objectX,(float)objectY,0);
				text->width = (float)objectW;
				text->height = (float)objectH;
				text->value = objectScript;
				
				root->AttachChild(text);
			} else {
				console->Printf("Warning: unknown UI object specified in UI file %s",uifilename);
				break;
			}
		}
	}

	fclose(uifile);

	ui->desktop->AttachChild(root);
	root->visible = false;
	return root;
}

UIObject::UIObject() {
	absolute = false;
	absPositionSet = false;
	position = Vector(0,0,0);
	absoluteposition = Vector(0,0,0);
	children.clear();
	mouseOver = false;
	mouseDown = false;
	parent = 0;
}

UIObject::~UIObject() {
	if (children.size() > 0) {
		for (std::list<UIObject*>::iterator iter=children.begin(); iter!=children.end(); iter++) {
			delete *iter;
		}
	}
}

bool UIObject::Test(float x, float y) {
	if (!visible) return false;
	Vector absposition = CalculateAbsolutePosition();
	x -= absposition.x;
	y -= absposition.y;
	if ((0 < x && x <= width) && (0 < y && y <= height)) {
		return true;
	}
	return false;
}

void UIObject::onMouseDown(float x, float y) {
	mouseDown = true;

	for (std::list<UIObject*>::iterator iter=children.begin(); iter!=children.end(); iter++) {
		if ((*iter)->Test(x,y) && (!(*iter)->mouseDown)) {
			(*iter)->onMouseDown(x,y);
		} 
	}
}

void UIObject::onMouseUp(float x, float y) {
	mouseDown = false;

	for (std::list<UIObject*>::iterator iter=children.begin(); iter!=children.end(); iter++) {
		if ((*iter)->Test(x,y) && (*iter)->mouseDown) {
			(*iter)->onMouseUp(x,y);
		}
	}
}

void UIObject::onMouseMove(float x, float y) {
	for (std::list<UIObject*>::iterator iter=children.begin(); iter!=children.end(); iter++) {
		if ((*iter)->Test(x,y) || (*iter)->mouseDown) {
			if (!(*iter)->mouseOver) {
				(*iter)->mouseOver = true;
				(*iter)->onMouseFocus();			
			}
			(*iter)->onMouseMove(x,y);
		} else if ((*iter)->mouseOver) {
			(*iter)->mouseOver = false;
			(*iter)->onMouseBlur();
		}
	}
}

void UIObject::AttachChild(UIObject *child) {
	if (child) {
		children.push_back(child);
		child->parent = this;
		child->init();
	}
}

Vector UIObject::CalculateAbsolutePosition() {
	if (parent) {
		return position+parent->CalculateAbsolutePosition();
	}
	return position;
}

void UIObject::render() {
	if (!absPositionSet) {
		absoluteposition = CalculateAbsolutePosition();
		absPositionSet = true;
	}

	for (std::list<UIObject*>::iterator iter=children.begin(); iter!=children.end(); iter++) {
		if ((*iter)->visible) {
			(*iter)->render();
		} else {
			(*iter)->mouseOver = false;
			(*iter)->mouseDown = false;
		}
	}
}

void UIObject::renderTexture(Vector p, Vector d, int t) {
	glBindTexture(GL_TEXTURE_2D,t);
	glBegin(GL_QUADS);
		float hd2 = (float)vars->getIntValue("screen_height")/2;
		float px = (p.x / hd2) - 1.333f;	// aspect ratio
		float py = (p.y / hd2) - 1;
		float pw = d.x / hd2;
		float ph = d.y / hd2;

		glColor3f(1,1,1);
		glTexCoord2f(0,0); glVertex3f(px,-py,(float)UI_Z_COORD);
		glTexCoord2f(0,1); glVertex3f(px,-py-ph,(float)UI_Z_COORD);
		glTexCoord2f(1,1); glVertex3f(px+pw,-py-ph,(float)UI_Z_COORD);
		glTexCoord2f(1,0); glVertex3f(px+pw,-py,(float)UI_Z_COORD);
	glEnd();
}


void UIWindow::init() {
	closeButton.loadButtonTextures("close_button");
	closeButton.width = 25; 
	closeButton.height = 25;
	closeButton.position = Vector(width - 27,3,0);
	AttachChild(&closeButton);
}

void UIWindow::render() {
	if (closeButton.pressed) {
		closeButton.pressed = false;
		closeButton.mouseDown = false;
		closeButton.mouseOver = false;
		mouseDown = false;
		mouseOver = false;
		visible = false;
	}

//	if (!absPositionSet) {
		absoluteposition = CalculateAbsolutePosition();
//		absPositionSet = true;
//	}

	renderTexture(absoluteposition, Vector(width,height,0),windowbg);

	for (std::list<UIObject*>::iterator iter=children.begin(); iter!=children.end(); iter++) {
		if ((*iter)->visible) {
			if (!mouseOver && !input->GetMButtonPushed(1)) {
				(*iter)->mouseOver = false;
				(*iter)->mouseDown = false;
			}
			(*iter)->render();
		} else {
			(*iter)->mouseOver = false;
			(*iter)->mouseDown = false;
		}
	}
}

void UIWindow::onMouseMove(float x, float y) {
	bool hit_child = false;
	if (!visible) return;
	for (std::list<UIObject*>::iterator iter=children.begin(); iter!=children.end(); iter++) {
		if ((*iter)->Test(x,y)) {
			hit_child = true;
			if (!(*iter)->mouseOver) {
				(*iter)->mouseOver = true;
				(*iter)->onMouseFocus();			
			} 
			(*iter)->onMouseMove(x,y);
		} else if ((*iter)->mouseOver) {
			(*iter)->mouseOver = false;
			(*iter)->onMouseBlur();
		}
	}

	if (mouseDown && !hit_child) {
		position.x += input->mouseMovement.x;
		position.y += input->mouseMovement.y;
	}
}

void UIText::render() {
	if (!visible) return;
	absoluteposition = CalculateAbsolutePosition();

	glColor3f(1,1,1);
	renderer->text.print(absoluteposition.x/10.0f,absoluteposition.y/20.0f,"%s",value.c_str());
}


bool UIButton::loadButtonTextures(const char *filelocation) { // "data/UI/button" -> button_01.bmp, button_02.bmp, button_03.bmp
	char buf[100];
	for (int i=0;i<3;i++) {
		sprintf(buf,"data/UI/%s%d.JPG",filelocation,i);
		texture[i] = renderer->LoadTexture(buf);
	}

	return true;
}

void UIButton::render() {
	if (!visible) return;
	if (enabled) {
		if (!mouseDown) {
			renderTexture(CalculateAbsolutePosition(),Vector(width,height,0),texture[Enabled]);
		} else {
			renderTexture(CalculateAbsolutePosition(),Vector(width,height,0),texture[Pressed]);
		}	
	} else {
		renderTexture(CalculateAbsolutePosition(),Vector(width,height,0),texture[Disabled]);
	}
}

void UIButton::onMouseUp(float x, float y) {
	mouseDown = false;
	if (enabled) {
		console->RunLinef("curID = %d\n",parent->ownerID);
		console->RunLine(releasedCommand.c_str());
		OnButtonPressed();
	}
}

void UIButton::onMouseDown(float x, float y) {
	if (!visible) return;
	if (enabled) {
		mouseDown = true;
		console->RunLinef("curID = %d\n",parent->ownerID);
		console->RunLine(pressedCommand.c_str());
	}
}