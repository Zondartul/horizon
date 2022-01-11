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
#include "mouse.h"

cameraKind::cameraKind(){
	pos = {0,0,0};
	rot = {0,0,0};
	fov = 90;
	scale = 1;
	perspective = true;
	mode3D = true;
}
vec3 cameraKind::forward(){
	return rotate((vec3){1,0,0},d2r*camera.rot);
}
vec3 cameraKind::up(){
	return rotate((vec3){0,0,1},d2r*camera.rot);
}
vec3 cameraKind::right(){
	return rotate((vec3){0,1,0},d2r*camera.rot);
}
float cameraKind::eyetrace(float maxdist){
	return maxdist;
	/*
	float dist;
	vec3 start,end;
	vec3 dir = camera.forward();
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
		mView = glm::translate(mView,-pos);
		//printf("pos = %s, rot = %s\n",toString(pos).c_str(),toString(rot).c_str());
	}else{
		mView = mat4(1.0f);
		//mView = glm::translate(mView,vec3(width/2,height/2,0));
		//glLoadIdentity();
	}
}
void cameraKind::setPos(vec3 newpos){pos = newpos; }//printf("campos: %.3f %.3f %.3f\n",newpos.x,newpos.y,newpos.z);
void cameraKind::setRot(vec3 newrot){rot = newrot; }//printf("camrot: %.3f %.3f %.3f\n",rot.x,rot.y,rot.z);}
void cameraKind::setFov(float newfov){fov = newfov;}
void cameraKind::setScale(float newscale){scale = newscale;}
void cameraKind::go2D(){
	//glDisable(GL_LIGHTING);
	mode3D = false;
	//vec2 scr = getScreenSize();
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
	vec2 scr = getScreenSize();
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
	float znear = 0.1;
	float zfar = 10000;	
	if(perspective){
		float aspect = ((float)width)/((float)height);
		float left = -znear*tan(fov*d2r/2.0);
		float right = -left;
		float bottom = -znear*tan(fov*d2r/2.0)/aspect;
		float top = -bottom;
		mProjection = glm::frustum(left,right,bottom,top,znear,zfar);
	}else{
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
	vec2 scr = getScreenSize();
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
#define swizz3(V,a,b,c) {V.a,V.b,V.c}

vec3 cameraKind::worldToDevice(vec3 worldpos){
	vec4 V4 = vec4(worldpos,1.f);
	V4 = mProjection * mView * V4;
	vec3 V3 = vec3(V4.x,V4.y,V4.z)/V4.w;
	return V3;
}
vec3 cameraKind::deviceToWorld(vec3 devpos){
	vec4 V4 = vec4(devpos,1.f);
	V4 = inverse(mProjection * mView) * V4;
	vec3 V3 = vec3(V4.x,V4.y,V4.z)/V4.w;
	return V3;
}
vec3 cameraKind::screenToDevice(vec3 scrpos, z_meaning zm){
	float x = scrpos.x;
	float y = scrpos.y;
	float z = scrpos.z;
	vec2 scr = (vec2)getScreenSize();
	float zfar = 100.f;
	float zfar_true,zratio;
	vec3 osdw,wtd;
	switch(zm){
		case(Z_IS_ORTHODOX):
			x = x/scr.x;		//x normed
			x = 2.f*x-1.f;		//x shifted
			
			y = scr.y-y-1.f; 	//y flipped
			y = y/scr.y;		//y normed
			y = 2.f*y-1.f;		//y shifted
			
			z = 2.f*z-1.f;		//z shifted
			return vec3(x,y,z);
		break;
		case(Z_IS_DISTANCE):
			osdw = deviceToWorld(screenToDevice({scrpos.x,scrpos.y,1},Z_IS_ORTHODOX));
			wtd = worldToDevice(camera.pos+normalize(osdw-camera.pos)*scrpos.z);
			return wtd;
		break;
		case(Z_IS_PLANE):
			osdw = deviceToWorld(screenToDevice({scrpos.x,scrpos.y,1},Z_IS_ORTHODOX));
			
			zfar_true = length(osdw-camera.pos);
			zratio = zfar_true/zfar;
			wtd = worldToDevice(camera.pos+normalize(osdw-camera.pos)*scrpos.z*zratio);
			return wtd;
		break;
	}
}
vec3 cameraKind::deviceToScreen(vec3 devpos, z_meaning zm){
	float x = devpos.x;
	float y = devpos.y;
	float z = devpos.z;
	vec2 scr = getScreenSize();
	float zfar = 100.f;
	float zfar_true,zratio;
	vec3 dtw,dts;
	switch(zm){
		case(Z_IS_ORTHODOX):
			x = (x+1.f)/2.f; 	//x unshifted
			x = x*scr.x;		//x unnormed

			y = (y+1.f)/2.f;	//y unshifted
			y = y*scr.y;		//y unnormed
			y = scr.y-y-1.f;	//y unflipped
			
			z = (z+1.f)/2.f;	//z unshifted
			return vec3(x,y,z);
		break;
		case(Z_IS_DISTANCE):
			dtw = deviceToWorld(devpos);
			dts = deviceToScreen({devpos.x,devpos.y,1},Z_IS_ORTHODOX);
			dts.z = length(camera.pos-dtw);
			return dts;
		break;
		case(Z_IS_PLANE):
			dtw = deviceToWorld({0,0,devpos.z});
			dts = deviceToScreen(devpos,Z_IS_ORTHODOX);
			dts.z = length(camera.pos-dtw);
			return dts;
		break;
	}
}
vec3 cameraKind::getMouseDir(){
	vec2 mouse = getMousePos();
	vec3 wp = screenToWorld({mouse.x,mouse.y,1},Z_IS_ORTHODOX);
	return normalize(wp-camera.pos);
}
#define printval(x) printf(#x ": %f\n", x)
#define zToWorld(z) (znear/(1.f+znear/zfar-z))
#define zToDevice(z) (znear*(1/zfar - 1/z)+1)

vec3 cameraKind::screenToWorld(vec3 scrpos, z_meaning zm){	
	perspective = true;
	go3D();
	return deviceToWorld(screenToDevice(scrpos,zm));
}
vec3 cameraKind::worldToScreen(vec3 worldpos, z_meaning zm){
	return deviceToScreen(worldToDevice(worldpos),zm);
}

camprojection cameraKind::getProjection(){
	camprojection cpj;
	cpj.MVP = mProjection*mView;
	cpj.pos = pos;
	return cpj;
}

cameraKind camera;



	/*
	float zfar = 100.f;
	float znear = 0.1f;
	//scrpos.z = (scrpos.z-znear)/zfar;			// z.world -> z.device
	vec2 scrpos1 = vec2(scrpos.x,scrpos.y);
	vec2 scr = toVec2((vec2)getScreenSize());
	vec4 viewport= {0,0,scr.x,scr.y};
	//float znear = 0.1;
	mat4 mP = mProjection;
	mat4 mV = mView;
	bool flipy = true;
	bool fixz = true;
	float z = scrpos.z;//zToDevice(scrpos.z);
	
	//#if GLM_DEPTH_CLIP_SPACE == GLM_DEPTH_ZERO_TO_ONE
	//printf("Z from 0 to 1\n")
	//#else
	//printf("Z from -1 to 1\n");
	//#endif
	//printf("zToDevice: %f\n",z);
	//printf("zToWorld: %f\n",zToWorld(z));
	
	//vec3 WTD = worldToDevice(scrpos);
	//printf("WTD: %s, len: %f\n",toString(WTD).c_str(),length(WTD));
	
	//vec3 STD = screenToDevice(scrpos);
	//printf("STD: %s, len: %f\n",toString(STD).c_str(),length(STD));
	
	//vec3 DTW = deviceToWorld(STD);
	//printf("DTW: %s, len: %f\n",toString(DTW).c_str(),length(DTW));
	
	//z = (z-znear)/(zfar-znear);			// z.world -> z.device
	if(flipy){scrpos1.y = scr.y-scrpos1.y-1;}	// z.screen -> z.screen(o-dox)
	
	
	
    //mat4 invVP = glm::inverse(mP * mV);
	
	vec3 p0 = toVec3(pos);
	vec3 pr = glm::unProject(vec3(scrpos1.x,scrpos1.y,z),mV,mP,viewport);
	vec3 dir = pr-p0;
	//it appears that z becomes znear/(1-z)
	//reverse should be 
	
	if(fixz){
		//vec3 p1 = glm::unProject(vec3(scrpos1.x,scrpos1.y,0),mV,mP,viewport);
		//vec3 p2 = glm::unProject(vec3(scrpos1.x,scrpos1.y,1),mV,mP,viewport);
		//float zfar_true = length(p2-p0);
		//float znear_true = length(p1-p0);
		//printf("pr-p0: %f\n",length(pr-p0));
		//printf("zfar_true: %f\n znear_true: %f\n",zfar_true,znear_true);
		//printval(znear_true);
		//printval(zfar_true);
		//float zpred = znear/(1.f+znear/zfar-scrpos.z);
		//printf("predicted len: %f\n",zpred);
		//float zw = znear*(1/zfar - 1/zpred)+1;
		//printf("zw: %f, z real: %f\n",zw, dir.z);
		printf("dir1: %s, len: %f\n",toString(dir).c_str(),length(dir));
		//dir = normalize(p2-p0);
		//vec3 dd = normalize(dir);
		//float len = length(dir);
		//dir = dd*(len/znear_true);
		//printf("dir2: %s, len: %f\n",toString(dir).c_str(),length(dir));
		
		float newlen = length(dir);
		//float newlen = z*zfar_true+znear_true;
		printf("scrpos.z: %f, newlen: %f\n",scrpos.z,newlen);
		//pr = p0+normalize(dir)*newlen;//scrpos.z*zfar_true;
	}
	printf("\n");
	return pr;
	*/