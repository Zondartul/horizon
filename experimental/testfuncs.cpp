#include "testfuncs.h"
#include "../math/convexhull.h"
#include "../game/camera.h"
#include "../main/control.h"
#include "../gui/newconsole.h"
#include "../input/keybinds.h"
void bindtests();

void testfunc1(void* arg){ //draws the view frustum
	int time = 600;
	frustum frust = camera.getFrustum();
	scene.push_back(new floatingtext(frust.A, "A", time));
	scene.push_back(new floatingtext(frust.B, "B", time));
	scene.push_back(new floatingtext(frust.C, "C", time));
	scene.push_back(new floatingtext(frust.D, "D", time));
	scene.push_back(new floatingtext(frust.E, "E", time));
	scene.push_back(new floatingtext(frust.F, "F", time));
	scene.push_back(new floatingtext(frust.G, "G", time));
	scene.push_back(new floatingtext(frust.H, "H", time));
	
	printf("point A: [%f, %f, %f], H: [%f, %f, %f]\n", frust.A.x, frust.A.y, frust.A.z, frust.H.x, frust.H.y, frust.H.z);
	scene.push_back(new point(frust.A, time));
	scene.push_back(new point(frust.B, time));
	scene.push_back(new point(frust.C, time));
	scene.push_back(new point(frust.D, time));
	scene.push_back(new point(frust.E, time));
	scene.push_back(new point(frust.F, time));
	scene.push_back(new point(frust.G, time));
	scene.push_back(new point(frust.H, time));
	
	scene.push_back(new line(frust.A, frust.B, time));
	scene.push_back(new line(frust.B, frust.C, time));
	scene.push_back(new line(frust.C, frust.D, time));
	scene.push_back(new line(frust.D, frust.A, time));
	scene.push_back(new line(frust.E, frust.F, time));
	scene.push_back(new line(frust.F, frust.G, time));
	scene.push_back(new line(frust.G, frust.H, time));
	scene.push_back(new line(frust.H, frust.E, time));
	scene.push_back(new line(frust.A, frust.E, time));
	scene.push_back(new line(frust.B, frust.F, time));
	scene.push_back(new line(frust.C, frust.G, time));
	scene.push_back(new line(frust.D, frust.H, time));
	scene.push_back(new line(frust.E, frust.G, time));
	scene.push_back(new line(frust.F, frust.H, time));
	
	/*
	scene.push_back(new floatingtext(camera.pos, "Hello World!", 300));
	line *l = new line(camera.pos, camera.pos+camera.angle.forward(), 300);
	l->color1 = {255,0,0};
	l->color2 = {0,255,0};
	scene.push_back(l);
	quat testQ = quat::fromAngleAxis(1,{0,0,1});//((quat){0,{0,0,1}});
	l = new line(camera.pos+vec(0.1,0.1,0.1), camera.pos+vec(0.1,0.1,0.1)+testQ.rotateVector(camera.angle.forward()), 300);
	l->color1 = {255,0,0};
	l->color2 = {0,0,255};
	scene.push_back(l);*/
	
	/*
	frustum frust = camera.getFrustum();
	
	*/
	/*
	scene.push_back(new rtriangle(frust.A, frust.C, frust.B, 1600));
	scene.push_back(new rtriangle(frust.C, frust.A, frust.D, 1600));
	scene.push_back(new rtriangle(frust.A, frust.E, frust.D, 1600));
	scene.push_back(new rtriangle(frust.D, frust.E, frust.H, 1600));
	scene.push_back(new rtriangle(frust.E, frust.F, frust.H, 1600));
	scene.push_back(new rtriangle(frust.H, frust.F, frust.G, 1600));
	scene.push_back(new rtriangle(frust.F, frust.B, frust.G, 1600));
	scene.push_back(new rtriangle(frust.G, frust.B, frust.C, 1600));
	scene.push_back(new rtriangle(frust.A, frust.B, frust.F, 1600));
	scene.push_back(new rtriangle(frust.F, frust.E, frust.A, 1600));
	scene.push_back(new rtriangle(frust.H, frust.G, frust.D, 1600));
	scene.push_back(new rtriangle(frust.D, frust.G, frust.C, 1600));
	*/
	/*
	for(int I = 0; I<12;I++){
		cout << I<<",1: "<< frust.getTri(0,0).toString() << "\n";
		scene.push_back(new rtriangle(frust.getTri(I,0),frust.getTri(I,1),frust.getTri(I,2)));
	}
	*/
	//printf("point E distance: %f\n", (frust.H-camera.pos).length());
	
}

void testfunc2(void* arg){ // orientation helper
	int time = 600;
	line *myline = new line(camera.pos, camera.pos+camera.angle.localX());
	myline->color2 = {255,0,0};
	myline->lifetime = time;
	scene.push_back(myline);
	scene.push_back(new floatingtext(camera.pos+camera.angle.localX(),"x",time));
	myline = new line(camera.pos, camera.pos+camera.angle.forward());
	myline->color2 = {0,255,0};
	myline->lifetime = time;
	scene.push_back(myline);
	scene.push_back(new floatingtext(camera.pos+camera.angle.localY(),"y",time));
	myline = new line(camera.pos, camera.pos+camera.angle.up());
	myline->color2 = {0,0,255};
	myline->lifetime = time;
	scene.push_back(myline);
	scene.push_back(new floatingtext(camera.pos+camera.angle.localZ(),"z",time));
	myline = new line(camera.pos, camera.pos+quat::fromAngleAxis(30,camera.angle.localZ()).rotateVector(camera.angle.localY()));
	myline->color2 = {255,255,255};
	myline->lifetime = time;
	scene.push_back(myline);
	myline = new line(camera.pos+vec(0.1,0.1,0.1), camera.pos+vec(0.1,0.1,0.1)+camera.angle.localX().cross(camera.angle.localY()));
	myline->color2 = {0,255,255};
	myline->lifetime = time;
	scene.push_back(myline);
	vec intersex;
	if(ray_triangle_intersection(camera.pos, camera.angle.forward(), vec(0,1,0),vec(0.87, -0.5,0), vec(-0.87, -0.5,0), intersex))
		scene.push_back(new point(intersex, time));
	//scene.push_back(myline);
}

void testfunc3(void *arg){ // quaternion direction test + floating text
	int time = 600;
	scene.push_back(new floatingtext(camera.pos, "Hello World!", time));
	line *l = new line(camera.pos, camera.pos+camera.angle.forward(), time);
	l->color1 = {255,0,0};
	l->color2 = {0,255,0};
	scene.push_back(l);
	quat testQ = quat::fromAngleAxis(1,{0,0,1});//((quat){0,{0,0,1}});
	l = new line(camera.pos+vec(0.1,0.1,0.1), camera.pos+vec(0.1,0.1,0.1)+testQ.rotateVector(camera.angle.forward()), time);
	l->color1 = {255,0,0};
	l->color2 = {0,0,255};
	scene.push_back(l);
}


void testfunc4(void *arg){ // convex hull
	createconvexhull();
}
void testfunc5(void *arg){
	throw except("Hello World!");
}
void bindtests(){
	confuncs["test1"] = testfunc1;
	confuncs["test2"] = testfunc2;
	confuncs["test3"] = testfunc3;
	confuncs["test4"] = testfunc4;
	confuncs["test5"] = testfunc5;
	//confuncs["test6"] = testfunc6;
	KeyBinds["t"] = "test1";
	KeyBinds["y"] = "test2";
	KeyBinds["u"] = "test3";
	KeyBinds["i"] = "test4";
	KeyBinds["o"] = "test5";
	//KeyBinds["p"] = "test6";
	
}