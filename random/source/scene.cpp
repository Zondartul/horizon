#include "scene.h"
#include "GL/gl.h"
//#include <vector>
//using std::vector;
//vector<renderable> scene;
void sceneKind::feedVerts(){
	for(int I = 0; I < verts->size(); I++){
		if(I < colors->size()){
			auto c = (*colors)[I];
			glColor3ub(c.r,c.g,c.b);					
		}else{
			glColor3ub(0,0,0);
		}
		auto v = (*verts)[I];
		glVertex3f(v.x,v.y,v.z);	
	}
}
sceneKind::sceneKind(){}
void sceneKind::render(){
	auto Begin = renderQueue.begin();
	auto End = renderQueue.end();
	for(auto I = Begin; I != End; I++){
		switch(I->type){
			case(S_COLOR):
				colors = I->color;			
				break;
			case(S_VERTEX):
				verts = I->vertex;
				break;
			case(S_DRAWPOINTS):
				glBegin(GL_POINTS);
				feedVerts();			
				glEnd();
				break;
			case(S_DRAWLINES):
				glBegin(GL_LINES);
				feedVerts();
				glEnd();
				break;
			case(S_DRAWTRIANGLES):
				glBegin(GL_TRIANGLES);
				feedVerts();
				glEnd();
				break;
			case(S_DRAWTEXT):
				setTextPos({100,100});
				printw("%s",I->text->c_str());
				break;
			default:
				break;
		}	
	}
}
void sceneKind::setColors(vector<vec3ub> colors){
	sceneCommand cmd;
	cmd.type = S_COLOR;
	cmd.color = new vector<vec3ub>(colors);
	renderQueue.push_back(cmd);
}
void sceneKind::setVertices(vector<vec3f> vv){
	sceneCommand cmd;
	cmd.type = S_VERTEX;
	cmd.vertex = new vector<vec3f>(vv);
	renderQueue.push_back(cmd);
}
void sceneKind::drawPoints(){
	sceneCommand cmd;
	cmd.type = S_DRAWPOINTS;
	renderQueue.push_back(cmd);
}
void sceneKind::drawLines(){
	sceneCommand cmd;
	cmd.type = S_DRAWLINES;
	renderQueue.push_back(cmd);
}
void sceneKind::drawTriangles(){
	sceneCommand cmd;
	cmd.type = S_DRAWTRIANGLES;
	renderQueue.push_back(cmd);
}
void sceneKind::drawText(string text){
	sceneCommand cmd;
	cmd.type = S_DRAWTEXT;
	cmd.text = new string(text);
	renderQueue.push_back(cmd);
}

sceneKind scene;
