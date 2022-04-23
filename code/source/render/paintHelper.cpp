#include "paintHelper.h"
#include "rmodel.h"
#include "paint.h"
#include "camera.h"

#define rmTri(A,B,C) \
	rm->vertices->push_back(A); rm->uvs->push_back(vec2(0,0)); rm->colors->push_back(vec3(1,0,0));	\
	rm->vertices->push_back(B); rm->uvs->push_back(vec2(0,1)); rm->colors->push_back(vec3(0,1,0));	\
	rm->vertices->push_back(C); rm->uvs->push_back(vec2(1,1)); rm->colors->push_back(vec3(0,0,1));
	
#define rmLine(A,B) \
	rm->vertices->push_back(A);	\
	rm->vertices->push_back(B); 
	
#define rmQuad(A,B,C,D) \
	rmTri(D,A,B) \
	rmTri(B,C,D)
void paintArrow2D(vec3 from, vec3 to, float size=1.f, float width=1.f){
	//              E
	//              |\
	// A------------B \
	// |(from)      |  F(to)
	// D------------C /
	//              |/
	//              G
	
	//(z)-->(x)
	// |
	// |
	// v
	//(y)
	
	if(width){
		vec3 A,B,C,D,E,F,G;
		vec3 tribase = from+normalizeSafe(to-from)*size;
		vec3 up = normalizeSafe(cross(to-from,vec3(0,0,1)));
		vec3 down = -up;
		vec3 offsetUp = up*width;
		vec3 offsetDown = down*width;
		A = from	+ offsetUp;
		D = from	+ offsetDown;
		B = tribase + offsetUp;
		C = tribase + offsetDown;
		E = tribase + offsetUp*2;
		G = tribase + offsetDown*2;
		
		rmodel *rm = new rmodel();
		rmQuad(A,B,C,D);
		rmTri(B,E,F); rmTri(C,B,F); rmTri(G,C,F);
		rm->finalize();
		uploadRmodel(rm);
		setRenderMode(3);
		drawRmodel(rm);
		deleteRmodel(rm);
	}else{
		vec3 E,G;
		vec3 tribase = from+normalizeSafe(to-from)*size;
		E = tribase + vec3(0,-1,0)*size;
		G = tribase + vec3(0, 1,0)*size;
			
		rmodel *rm = new rmodel(); 
		rmLine(from,to);
		rmLine(E,to);
		rmLine(G,to);
		rm->finalize(); 
		uploadRmodel(rm);
		setRenderMode(2);
		drawRmodel(rm); 
		deleteRmodel(rm);
	}
}
void paintArrow3D(vec3 from, vec3 to, float size){
	
	vec3 trans = from;
	vec3 dv = to-from;
	float angZ = angle(dv,vec3(0,0,1));
	float angY = angle(dv,vec3(0,1,0));
	float angX = angle(dv,vec3(1,0,0));
	from = vec3(0,0,0);
	to = vec3(0,0,length(dv));
	
	vec3 E,G,H,I;
	vec3 tribase = from+normalizeSafe(to-from)*size;
	vec3 north = vec3( 1, 0, 0);
	vec3 south = vec3(-1, 0, 0);
	vec3 west  = vec3( 0, 1, 0);
	vec3 east  = vec3( 0,-1, 0);
	E = tribase + north*size;
	G = tribase + south*size;
	H = tribase + west*size;
	I = tribase + east*size;
	
	rmodel *rm = new rmodel(); 
	rmLine(from,to);
	rmLine(E,to);
	rmLine(G,to);
	rmLine(H,to);
	rmLine(I,to);
	rm->finalize(); 
	uploadRmodel(rm);
	setRenderMode(2);
	setPosition(trans);
	setRotation(vec3(angX,angY,angZ));
	drawRmodel(rm); 
	deleteRmodel(rm);
}

void paintCube(vec3 from, vec3 to){
	vec3 A,B,C,D,E,F,G,H;
	
	//    A------B
	//   /|     /|
	//  D-+----C |
	//  | |    | |
	//  | E----+-F
	//  |/     |/
	//  H------G
	//
	
	vec3 dv = to-from;
	A = from + dv*vec3(0,0,0);
	B = from + dv*vec3(1,0,0);
	C = from + dv*vec3(1,1,0);
	D = from + dv*vec3(0,1,0);
	E = from + dv*vec3(0,0,1);
	F = from + dv*vec3(1,0,1);
	G = from + dv*vec3(1,1,1);
	H = from + dv*vec3(0,1,1);
	
	
	rmodel *rm = new rmodel(); 
	rmLine(A,B);rmLine(B,C);rmLine(C,D);rmLine(D,A);
	rmLine(A,E);rmLine(B,F);rmLine(C,G);rmLine(D,H);
	rmLine(E,F);rmLine(F,G);rmLine(G,H);rmLine(H,E);
	rm->finalize(); 
	uploadRmodel(rm);
	setRenderMode(2);
	drawRmodel(rm); 
	deleteRmodel(rm);
	
}

void paint3d2dText(vec3 pos, string text){
	vec3 scrpos = g_camera.worldToScreen(pos);
	setTextPos(vec2(scrpos.x,scrpos.y));
	printText(text);
}

