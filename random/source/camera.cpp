#include "camera.h"
#include "math.h"
#include "GL/gl.h"
//#include "SDL2/SDL.h"
#include "simplemath.h"
#include "selfaware.h"
#include "stdlib.h"
#include "bitmap.h"
#include <string>
using std::string;
#include "stringUtils.h"
#include "globals.h"
#include "physics.h"

cameraKind::cameraKind(){
	pos = {0,0,0};
	rot = {0,0,0};
	fov = 90;
	scale = 1;
	perspective = true;
	mode3D = true;
}
vec3f cameraKind::forward(){
	return (vec3f){1,0,0}.rotate(camera.rot);
}
vec3f cameraKind::up(){
	return (vec3f){0,0,1}.rotate(camera.rot);
}
vec3f cameraKind::right(){
	return (vec3f){0,1,0}.rotate(camera.rot);
}
float cameraKind::eyetrace(float maxdist){
	float dist;
	vec3f start,end;
	vec3f dir = camera.forward();
	start = camera.pos;
	end = camera.pos+dir*maxdist;
	if(!raytrace(start,end,0,&dist)){dist = maxdist;}
	return dist;
}
void cameraKind::reposition(){
	if(mode3D){
		glLoadIdentity();
		 //openGL matrix multiplications are applied to scene in the reverse order
		//to how you call them... this has confusing effects.
		
		//change from OpenGL coordinate system to mine
		//OpenGL: x - right, y - up, z - out of screen
		//Mine: x - into screen, y - left, z - up
		glRotatef(90,0,1,0);
		glRotatef(-90,1,0,0);
		//rotate the world around the camera
		glRotatef(rot.x,1,0,0);
		glRotatef(rot.y,0,1,0);
		glRotatef(rot.z,0,0,1);
		//move the world away from camera
		glTranslatef(-pos.x,-pos.y,-pos.z);
	}else{
		glLoadIdentity();
	}
}
void cameraKind::setPos(vec3f newpos){pos = newpos;}
void cameraKind::setRot(vec3f newrot){rot = newrot;}
void cameraKind::setFov(float newfov){fov = newfov;}
void cameraKind::setScale(float newscale){scale = newscale;}
void cameraKind::go2D(){
	//glDisable(GL_LIGHTING);
	mode3D = false;
	vec2i scr = getScreenSize();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,scr.x,scr.y);
	
	glOrtho(0,scr.x,scr.y,0,0,1);
	glMatrixMode(GL_MODELVIEW);
	reposition();
}
void cameraKind::go3D(){
	//glEnable(GL_LIGHTING);
	mode3D = true;
	vec2i scr = getScreenSize();
	int height = scr.x;
	int width = scr.y;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,scr.x,scr.y);
	
	if(perspective){
		double aspect = ((double)width)/((double)height);
		double znear = 0.1;
		double zfar = 100;
		double left = -znear*tan(fov*d2r/2.0);
		double right = -left;
		double bottom = -znear*tan(fov*d2r/2.0)*aspect;
		double top = -bottom;
		glFrustum(left,right,bottom,top,znear,zfar);
	}else{
		double znear = 0.1;
		double zfar = 100;
		double left = -width*scale/2;
		double right = -left;
		double bottom = -height*scale/2;
		double top = -bottom;
		glOrtho(left,right,bottom,top,znear,zfar);
	}
	glMatrixMode(GL_MODELVIEW);
	reposition();
}
void cameraKind::goPerspective(){
	perspective = true;
	go3D();
}
void cameraKind::goOrtho(){
	perspective = false;
	go3D();
}
void cameraKind::screenshot(){
	printf("ss init\n");
	vec2i scr = getScreenSize();
	int width = scr.x;
	int height = scr.y;
	unsigned char *buff = (unsigned char*)malloc(sizeof(unsigned char)*width*height*3);
	glPixelStorei(GL_PACK_ALIGNMENT,1);
	glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,buff);
	bitmap BMP;
	BMP.width = width;
	BMP.height = height;
	BMP.format = TL_RGB;
	BMP.data.reset(buff);
	//if(BMP.width%2){BMP = BMP.crop(height, width-1);}
	string filename;
	int I = 0,end=0;
	while(!end){
		filename = "screenshots/screenshot";
		filename += itoa(I);
		filename += ".png";
		FILE *f = fopen(filename.c_str(),"rb");
		if(!f){end = 1;}else{I++;}
		fclose(f);
	}
	printf("save started\n");
	BMP.flipVertical().saveAs(filename.c_str());	
	FILE *f = fopen(filename.c_str(),"rb");
	if(!f){printf("\nERROR: couldn't write file [%s]\n",filename.c_str());}
	else{printf("\nScreenshot saved to [%s]\n",filename.c_str());}
	fclose(f);
}

cameraKind camera;