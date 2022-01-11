#include "camera.h"
#include "math.h"
#include "GL/gl.h"
//#include "SDL2/SDL.h"
#include "simplemath.h"
//#include "selfaware.h"
#include "window.h" //for screen size
#include "stdlib.h"
#include "bitmap.h"
#include <string>
using std::string;
#include "stringUtils.h"
#include "globals.h"
#include "glm/gtc/matrix_transform.hpp"
using glm::mat4;
using glm::vec3;

//#include "physics.h"

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
	return maxdist;
	/*
	float dist;
	vec3f start,end;
	vec3f dir = camera.forward();
	start = camera.pos;
	end = camera.pos+dir*maxdist;
	if(!raytrace(start,end,0,&dist)){dist = maxdist;}
	return dist;
	*/
}
void cameraKind::reposition(){
	if(mode3D){
		/*
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
		*/
		mView = mat4(1.0f);
		//change from OpenGL coordinate system to mine
		//OpenGL: x - right, y - up, z - out of screen
		//Mine: x - into screen, y - left, z - up
		mView = glm::rotate(mView,90.0f*(float)d2r,vec3(0,1,0));
		mView = glm::rotate(mView,-90.0f*(float)d2r,vec3(1,0,0));
		//rotate the world around the camera
		mView = glm::rotate(mView,rot.x*(float)d2r,vec3(1,0,0));
		mView = glm::rotate(mView,rot.y*(float)d2r,vec3(0,1,0));
		mView = glm::rotate(mView,rot.z*(float)d2r,vec3(0,0,1));
		//move the world away from camera
		mView = glm::translate(mView,-toVec3(pos));
	}else{
		mView = mat4(1.0f);
		//mView = glm::translate(mView,vec3(width/2,height/2,0));
		//glLoadIdentity();
	}
}
void cameraKind::setPos(vec3f newpos){pos = newpos; }//printf("campos: %.3f %.3f %.3f\n",newpos.x,newpos.y,newpos.z);
void cameraKind::setRot(vec3f newrot){rot = newrot; }//printf("camrot: %.3f %.3f %.3f\n",rot.x,rot.y,rot.z);}
void cameraKind::setFov(float newfov){fov = newfov;}
void cameraKind::setScale(float newscale){scale = newscale;}
void cameraKind::go2D(){
	//glDisable(GL_LIGHTING);
	mode3D = false;
	//vec2i scr = getScreenSize();
	//int height = scr.x;
	//int width = scr.y;
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glViewport(0,0,scr.x,scr.y); //put this into renderqueue as well?
	glViewport(0,0,width,height);
	scale = 1.0f;//0.01f;
	float znear = -1;
	float zfar = 1;
	float left = 0;//-width*scale/2;
	float right = width*scale;//-left;
	float bottom = height*scale;//-height*scale/2;
	float top = 0;//-bottom;
	mProjection = glm::ortho(left,right,bottom,top,znear,zfar);
	//glOrtho(0,scr.x,scr.y,0,0,1);
	//glMatrixMode(GL_MODELVIEW);
	reposition();
}
void cameraKind::go3D(){
	//glEnable(GL_LIGHTING);
	mode3D = true;
	vec2i scr = getScreenSize();
	//int height = scr.x;
	//int width = scr.y;
	/*
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
	*/
	//glViewport(0,0,scr.x,scr.y);
	glViewport(0,0,width,height);
	if(perspective){
		float aspect = ((float)width)/((float)height);
		float znear = 0.1;
		float zfar = 100;
		float left = -znear*tan(fov*d2r/2.0);
		float right = -left;
		float bottom = -znear*tan(fov*d2r/2.0)/aspect;
		float top = -bottom;
		mProjection = glm::frustum(left,right,bottom,top,znear,zfar);
	}else{
		float znear = 0.1;
		float zfar = 100;
		float left = -width*scale/2;
		float right = -left;
		float bottom = -height*scale/2;
		float top = -bottom;
		mProjection = glm::ortho(left,right,bottom,top,znear,zfar);
	}
	
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

	// glRotatef(rot.x,1,0,0);
	// glRotatef(rot.y,0,1,0);
	// glRotatef(rot.z,0,0,1);
vec3f cameraKind::toWorld(vec2i scrpos){
	vec2i scr = getScreenSize();
	double width,height,aspect,znear,zfar,left,right,bottom,top;
	width = scr.x;
	height = scr.y;
	if(perspective){
		aspect = ((double)width)/((double)height);
		znear = 0.1;
		zfar = 100;
		left = -znear*tan(fov*d2r/2.0);
		right = -left;
		bottom = -znear*tan(fov*d2r/2.0)*aspect;
		top = -bottom;
	}else{
		znear = 0.1;
		zfar = 100;
		left = -width*scale/2;
		right = -left;
		bottom = -height*scale/2;
		top = -bottom;
	}
	vec3f W = {left+((float)scrpos.x)/scr.x,top+((float)scrpos.y)/scr.y,znear};
	W = W.rotate((vec3f){1,0,0},rot.x).rotate((vec3f){0,1,0},rot.y).rotate((vec3f){0,0,1},rot.z).norm();
	return W;
}
vec2i cameraKind::toScreen(vec3f worldpos){
	vec2i scr = getScreenSize();
	worldpos = worldpos - pos;
	worldpos = worldpos.rotate((vec3f){1,0,0},-rot.x).rotate((vec3f){0,1,0},-rot.y).rotate((vec3f){0,0,1},-rot.z).norm();
	worldpos = worldpos * 0.1;
	return (vec2i){worldpos.x+scr.x/2,worldpos.y+scr.y/2};
}

cameraKind camera;