#include ".\waypoint.h"
#include "graphics.h"
#include "input.h"

extern Input *input;

Waypoint::Waypoint(void)
{
	type = E_WAYPOINT;
	family = EF_ENVIRONMENT;
}

Waypoint::~Waypoint(void)
{
}

void Waypoint::render() {
	if (input->inputContext == EditMode) {
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glBegin(GL_LINES);
			glColor3f(1,1,1);
			glVertex3f(position.x,position.y,position.z);
			glVertex3f(position.x,position.y+10.0f,position.z);
		glEnd();
		glPopAttrib();
	}
}