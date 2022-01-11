#include "modules/display/renderqueue.h"
#include "GL/gl.h"

void r_point2D::render(){
	glBegin(GL_POINTS);
		glColor3ub(color.r,color.g,color.b);
		glVertex2i(pos.x,pos.y);
	glEnd();
}

void r_line2D::render(){
	glBegin(GL_LINES);
		glColor3ub(cA.r,cA.g,cA.b);
		glVertex2i(A.x,A.y);
		glColor3ub(cB.r,cB.g,cB.b);
		glVertex2i(B.x,B.y);
	glEnd();
}

void r_rect2D::render(){
	vec2i a,b,c,d;
	a = area.start;
	b = {area.end.x,area.start.y};
	c = area.end;
	d = {area.start.x,area.end.y};
	glBegin(GL_TRIANGLES);
		glColor3ub(color.r,color.g,color.b);
		glVertex2i(a.x,a.y);
		glVertex2i(b.x,b.y);
		glVertex2i(c.x,c.y);
		
		glVertex2i(c.x,c.y);
		glVertex2i(d.x,d.y);
		glVertex2i(a.x,a.y);
	glEnd();
}
void r_rect2D_outline::render(){
	vec2i a,b,c,d;
	a = area.start;
	b = {area.end.x,area.start.y};
	c = area.end;
	d = {area.start.x,area.end.y};
	glBegin(GL_LINES);
		glColor3ub(color.r,color.g,color.b);
		glVertex2i(a.x,a.y);
		glVertex2i(b.x,b.y);
		
		glVertex2i(b.x,b.y);
		glVertex2i(c.x,c.y);
		
		glVertex2i(c.x,c.y);
		glVertex2i(d.x,d.y);
		
		glVertex2i(d.x,d.y);
		glVertex2i(a.x,a.y);
	glEnd();
}

vector<renderable*> renderQueue;

void render_the_renderQueue(){
	for(int I = 0; I < renderQueue.size(); I++){
		renderQueue[I]->render();
		delete renderQueue[I];
	}
	renderQueue.clear();
}