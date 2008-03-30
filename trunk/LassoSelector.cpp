#include ".\lassoselector.h"
#include "graphics.h"
#include "input.h"
#include "entity.h"
#include "terrain.h"
#include "camera.h"
#include "timer.h"
#include "console.h"
#include "variables.h"

extern Graphics *renderer;
extern gamevars *vars;
extern Timer *timer;
extern Camera *camera;
extern Terrain *terrain;
extern EntityContainer *ents;
extern Input *input;
extern Console *console;
bool spirograph = false;

LassoSelector::LassoSelector(void)
{
	enabled = true;
	maxSelectionCount = 8;
}

LassoSelector::~LassoSelector(void)
{
}

void LassoSelector::Select(Entity *e) {
	if ((int)SelectedEntities.size() >= maxSelectionCount || !e->alive) return;
	SelectedEntities.push_back(e);
	e->onSelected();
}

void LassoSelector::Unselect(Entity *e) {
	for (int i=0;i < (int)SelectedEntities.size(); i++) {
		if (SelectedEntities[i] == e) {
			SelectedEntities[i] = 0;
			return;
		}
	}
}

void LassoSelector::ClearSelection() {
	for (int i=0,s=(int)SelectedEntities.size();i<s;i++) {
		try {
			if (!SelectedEntities[i]) continue;
			SelectedEntities[i]->onUnSelected();
		} catch(...) {
			console->Printf("Arrrg, thar be callin onUnSelected with an invalid pointer, yeearg.");
		}	
	}
			
	SelectedEntities.clear();
}

bool LassoSelector::init() {
	selectionPoints.clear();

	selectionBroke=false;
	selectTime=0;
	selectFadeTime=1.0f;
	
	texSelectedEntity = renderer->LoadTexture("data/selectedEntity.JPG");

	return true;
}


void LassoSelector::run() {
	if (!terrain || !enabled) return;

	Node *node;
	Entity *closestEntity;

	LassoPosition = terrain->RayTest(camera->GetActualPosition(),input->mouseVector,1000,1,true);
	gridAlignedLassoPosition = Vector(floor(LassoPosition.x),terrain->getHeight((int)LassoPosition.x,(int)LassoPosition.z),floor(LassoPosition.z));

/*	glBegin(GL_LINES);
	glColor3f(0,1,0);
	glVertex3f(LassoPosition.x,LassoPosition.y, LassoPosition.z);
	glVertex3f(LassoPosition.x,LassoPosition.y+100, LassoPosition.z);
	glEnd();
*/
	if (input->GetMButtonPushed(1)) {							// initial push
		
		node = ents->qtree.tree->getLeaf(gridAlignedLassoPosition);
		closestEntity = node->getClosestEntity(gridAlignedLassoPosition,-1,0,0,0);		
		if (closestEntity && dist2(gridAlignedLassoPosition,closestEntity->position) < closestEntity->size.len2() && closestEntity->family != EF_ENVIRONMENT) {
			if (!input->GetKeyState(SDLK_RSHIFT) && !input->GetKeyState(SDLK_LSHIFT)) {
				ClearSelection();
			}
			Select(closestEntity);
		} else {
			ClearSelection();
		}
		if (timer->time - selectTime < selectFadeTime) {
			selectionPoints.clear();
			selectTime = 0;
			selectionBroke = false;
		}

	} else if (input->GetMButtonState(1) && !selectionBroke) {	// holding past first frame
		if (selectionPoints.size() == 0) {
			selectionPoint newSelectionPoint;
			newSelectionPoint.v = LassoPosition;
			newSelectionPoint.a = true;

			selectionPoints.push_back(newSelectionPoint);
		} else {					
			if (dist2(selectionPoints[selectionPoints.size()-1].v,LassoPosition) > vars->getFloatValue("selection_maxlinkdist")) {
				selectionPoint newSelectionPoint;
				newSelectionPoint.v = LassoPosition;
				newSelectionPoint.a = true;

				selectionPoints.push_back(newSelectionPoint);

				for (int i=0,s=(int)selectionPoints.size()-2;i<s;i++) {
					if (dist2(selectionPoints[i].v,LassoPosition) <= vars->getFloatValue("selection_maxlinkdist")) {
						// break selection drawing
						selectionBroke = true;
						for (int o=i;o>=0;o--)
							selectionPoints[o].a = false;
					}
				}
			}
		}
		selectTime = timer->time;
	} else {													// finished drawing (let go mouse or connected automatically
		
		
		
		if (selectionPoints.size() > 0) {	
			if (timer->time - selectTime > selectFadeTime) {
				selectionPoints.clear();
				selectionBroke = false;
			} else if (selectionPoints.size() > 2) {	// released this frame
				if (!selectionBroke) {
					// finish up the selection.
					Vector spP = selectionPoints[selectionPoints.size()-1].v;
					Vector v = spP - selectionPoints[selectionPoints.size()-2].v;
					float mld = vars->getFloatValue("selection_maxlinkdist");
					int countAdded = 0;
					while (dist2(spP,selectionPoints[0].v) > mld && countAdded++ < 30) {
						if (spirograph) {
							v += (Normalize(selectionPoints[0].v - spP)) / 8;
						} else {
							v = (Normalize(selectionPoints[0].v - spP)) * mld;
	//						v /= 1.25f;
						}
						spP += v;
						selectionPoint nsp;
						nsp.v = spP;
						nsp.a = true;
						selectionPoints.push_back(nsp);
					}
				}
			} else {
				selectionPoints.clear();
			}
		}


		Vector selectionCenter;
		float farthestBoundingDistance = 0;
		int selectionCount = 0;
		for (int i = 0; i<(int)selectionPoints.size()-1; i++) {
			if (selectionPoints[i].a) {
				selectionCenter += selectionPoints[i].v;
				selectionCount++;
			}
		}

		if (selectionCount > 5) {
			selectionCenter /= (float)selectionCount;
			for (int firstValid = 0; firstValid<(int)selectionPoints.size()-1; firstValid++) {
				if (selectionPoints[firstValid].a)
					break;				
			}
			for (int c = 0;c<10;c++) {
				int last_sp=firstValid;
				for (int i=1;i<(int)selectionPoints.size()-1;i++) {
					if (selectionPoints[i].a) {	
						for (int next_sp = i+1; i<(int)selectionPoints.size()-1; i++) {
							if (selectionPoints[i].a)
								break;
						}
						Vector p = selectionPoints[i].v;
						Vector d1 = Normalize(selectionPoints[firstValid].v - p).flat();
						Vector d2 = Normalize(selectionPoints[firstValid].v - selectionCenter).flat();
						
						Vector cpv = -Normalize(p - selectionCenter).flat();

						float boundingDistance = dist2(selectionCenter, selectionPoints[i].v);

						if (boundingDistance < dist2(selectionCenter, (selectionPoints[last_sp].v+selectionPoints[next_sp].v)/2)) {
							// concave 
							selectionPoints[i].v = (selectionPoints[i-1].v+selectionPoints[i+1].v)/2;
						} 

						if (boundingDistance > farthestBoundingDistance) {
							farthestBoundingDistance = boundingDistance; 
						}

						last_sp = i;
					}
				}
			}

			// now select the proper entities
			for (int i=0;i<(int)ents->entities.size()-1;i++) {
				if (ents->entities[i] && ents->entities[i]->alive && dist2(ents->entities[i]->position, selectionCenter) < farthestBoundingDistance) {
					float closestBoundingDistance = 999;
					int closestBoundingPoint = 0;
					for (int o=1;o<(int)selectionPoints.size()-1;o++) {
						if (selectionPoints[o].a) {	
							float d = dist2(ents->entities[i]->position, selectionPoints[o].v);
							if (d < closestBoundingDistance) {
								closestBoundingDistance = d;
								closestBoundingPoint = o;
							}
						}
					}
					if (dist2(selectionPoints[closestBoundingPoint].v,selectionCenter) > dist2(ents->entities[i]->position, selectionCenter)) {
						if (ents->entities[i]->family == EF_UNIT && ents->entities[i]->team == 1) {
							Select(ents->entities[i]);
						}
					}
				}
			}
		}
	}


	int spSize = (int)selectionPoints.size();
	if (spSize > 0) {
		glPushAttrib(GL_ALL_ATTRIB_BITS);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			glLineWidth((GLfloat)vars->getIntValue("selection_thickness"));
			glLineStipple(1,0x07ff);
			glEnable(GL_LINE_STIPPLE);
			int firstValid = -1;

			glColor4f(0,1,0,1);
			Vector a = selectionPoints[0].v;
			Vector selectionCenter = a;
			int selectionCount = 0;
			glBegin(GL_LINE_STRIP);
				for (int i=0;i<spSize;i++) {
					if (selectionPoints[i].a) {
						a = selectionPoints[i].v;
						selectionCenter += a;
						selectionCount++;
						if (spirograph) glColor3f(sinf((float)i),cosf((float)i),tanf((float)i));
						glVertex3f(a.x,a.y+0.5f,a.z);
						if (firstValid == -1) firstValid = i;
					}
				}

				if (firstValid > 0) {
					glColor3f(0,1,0);	
					a = selectionPoints[firstValid].v;
					glVertex3f(a.x,a.y,a.z);
				}
			glEnd();
		glPopAttrib();
	}
		
	if (SelectedEntities.size() > 0) {
		glPushAttrib(GL_ALL_ATTRIB_BITS);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR);
			glDisable(GL_DEPTH_TEST);
			glBindTexture(GL_TEXTURE_2D,texSelectedEntity);
			glColor3f(1,1,1);
			try {
				for (int i=0,s=(int)SelectedEntities.size();i<s;i++) {
					if (!SelectedEntities[i]) continue;
					if (!SelectedEntities[i]->alive) {
						SelectedEntities[i] = 0;
					}
					DrawQuad(SelectedEntities[i]->position,SelectedEntities[i]->size.flat());	
				}
			} catch(...) {
				SelectedEntities.clear();
			}

		glPopAttrib();
	}		
}