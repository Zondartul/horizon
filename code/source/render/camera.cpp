//system includes
#include <string>
//external includes
#ifndef NO_GLEW
	#include "GL/glew.h"
#endif
#include "glm/gtc/matrix_transform.hpp"
//project includes
#include "camera.h"
#include "math.h"
#include "simplemath.h"
#include "window.h" //for screen size
#include "stdlib.h"
#include "bitmap.h"
#include "stringUtils.h"
#include "globals.h"
#include "mouse.h"
//#include "collision.h"
#include "global_vars.h"
#include "timer.h"
#include "renderLayer.h"
//#include "geometry.h"
using glm::mat4;
using glm::vec3;
using std::string;

cameraKind::cameraKind(){
	pos = {0,0,0};
	rot = {0,0,0};
	fov = 90;
	scale2D = 1;
	scalePersp = 1;
	scaleOrtho = 1;
	perspective = true;
	mode3D = true;
}
vec3 cameraKind::forward(){
	auto& camera = G->gs_camera->g_camera;
	return rotate(vec3{1,0,0},d2r*camera.rot);
}
vec3 cameraKind::up(){
	auto& camera = G->gs_camera->g_camera;
	return rotate(vec3{0,0,1},d2r*camera.rot);
}
vec3 cameraKind::right(){
	auto& camera = G->gs_camera->g_camera;
	return rotate(vec3{0,1,0},d2r*camera.rot);
}
//collisioninfo *cameraKind::eyetrace(bool useCursor){
//	//auto& camera = G->gs_camera->g_camera;
//	//vec3 dv;
//	//if(useCursor){
//	//	vec2 mousepos = getMousePos();
//	//	vec3 mousedv = screenToWorld(vec3(mousepos.x,mousepos.y,1));
//	//	dv = normalizeSafe(mousedv - camera.pos);
//	//}else{
//	//	dv = forward();
//	//}
//	//return  raytrace(pos,dv);
//	return 0;
//}

#include "paint.h"
//
//rmodel *ray_shotgun(int x_steps, int y_steps){
//	auto& camera = G->gs_camera->g_camera;
//	auto& width = G->gs_window->g_width;
//	auto& height = G->gs_window->g_height;
//
//	rmodel *rm = new rmodel();
//	float x = 0;
//	float y = 0;
//	float dx = width/(float)x_steps;
//	float dy = height/(float)y_steps;
//	for(int Ix = 0; Ix < x_steps; Ix++){
//		for(int Iy = 0; Iy < y_steps; Iy++){
//			vec3 screenCoords(dx*Ix,dy*Iy,1.0f);
//			vec3 worldCoords = camera.screenToWorld(screenCoords);
//			vec3 dv = normalizeSafe(worldCoords - camera.pos);
//
//			//collisioninfo *col = raytrace(camera.pos,dv);
//			//if(col){
//			//	vec3 p = col->c_to_c.pos;
//			//	rm->vertices->push_back(p);
//			//}
//		}
//	}
//	rm->finalize();
//	uploadRmodel(rm);
//	return rm;
//}

void cameraKind::reposition(){
	if(mode3D){
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
	}else{
		mView = mat4(1.0f);
	}
}
void cameraKind::setPos(vec3 newpos){pos = newpos; }
void cameraKind::setRot(vec3 newrot){rot = newrot; }
void cameraKind::setFov(float newfov){fov = newfov;}

void cameraKind::go2D(){
	auto& width = G->gs_window->g_width;
	auto& height = G->gs_window->g_height;

	mode3D = false;
	#ifndef NO_GLEW
		setViewport(0, 0, width, height);
	#endif

	vec2 vpixel = vec2(0.f,0.f); //center-of-pixel shift.
	float znear = -1;
	float zfar = 1;
	float left = 0-vpixel.x;
	float right = width*scale2D+vpixel.x;
	float bottom = height*scale2D+vpixel.y;
	float top = 0-vpixel.y;
	mProjection = glm::ortho(left,right,bottom,top,znear,zfar);
	reposition();
}
void cameraKind::go3D(){
	auto& camera = G->gs_camera->g_camera;
	auto& width = G->gs_window->g_width;
	auto& height = G->gs_window->g_height;

	mode3D = true;
	vec2 scr = getScreenSize();
#ifndef NO_GLEW
	setViewport(0, 0, width, height);
#endif

	float znear = 0.1f;
	float zfar = 10000.f;
	if(perspective){
		float aspect = ((float)width)/((float)height);
		float left = -(float)znear*tan(fov*d2r/2.0)*scalePersp;
		float right = -left;
		float bottom = -(float)znear*tan(fov*d2r/2.0)*scalePersp/aspect;
		float top = -bottom;
		mProjection = glm::frustum(left,right,bottom,top,znear,zfar);
	}else{
		float left = -width*scaleOrtho/2;
		float right = -left;
		float bottom = -height*scaleOrtho/2;
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

struct task_screenshot {
	enum { ERROR, SAVE_TO_BMP, SAVE_TO_FILE } mode = task_screenshot::ERROR;
	void* buff = 0;
	bitmap* BMP = 0;
	int width;
	int height;
	task_screenshot(int width, int height, bitmap *BMP):
		width(width), height(height), BMP(BMP), mode(mode)
	{
		if (BMP) {
			this->BMP = BMP;
			mode = SAVE_TO_BMP;
		}
		else {
			BMP = new bitmap();
			this->BMP = BMP;
			BMP->width = width;
			BMP->height = height;
			BMP->format = TL_RGB;
			mode = SAVE_TO_FILE;
		}

		int size = BMP->numBytes();
		buff = (unsigned char*)malloc(size);//(sizeof(unsigned char)*width*height*3);
		printf("init take_screenshot, buff is %p size %d\n", buff, size);
	}
	~task_screenshot() 
	{
		printf("deconstructing take_screenshot...");
		if (mode != SAVE_TO_BMP) {
			printf("deleting BMP...");
			delete BMP;
		}
		if (buff) {
			printf("deleting buff...");
			delete buff;
		}
		printf("\n");
	}
	void run() {
		auto& deleteLayer = G->gs_paint->g_deleteLayer;
		auto& currentLayer = G->gs_paint->g_currentLayer;
		#ifndef NO_GLEW
			setLayer(deleteLayer);
			readPixels(0, 0, width, height, buff);
			printf("readPixels sent for (w %d, h %d, b %p)\n", width, height, buff);
			if (currentLayer) {
				printf("currentLayer: %s\n", toCString(currentLayer));
				currentLayer->print();
			}
			else {
				printf("!!NO CURRENT LAYER!!\n");
			}
		#endif

		auto func_continue = [&](timer* T) {run_continue();};
		int num_ticks = 10;
		simpletimer(func_continue, num_ticks);
	}
	void run_continue() {
		printf("run_continue\n");
		if (mode == 0) {
			printf("save image\n");
			saveImage();
		}
		else {
			printf("save image to file\n");
			saveImageToFile();
		}
		printf("self-destruct\n");
		delete this;
	}
	void saveImage() {
		BMP->setBuffer((unsigned char*)buff, BMP->numBytes());
	}
	void saveImageToFile() {
		saveImage();
		string filename;
		int I = 0, end = 0;
		while (!end) {
			filename = "screenshots/screenshot";
			filename += itoa(I);
			filename += ".png";
			FILE* f = fopen(filename.c_str(), "rb");
			if (!f) { end = 1; }
			else { I++; }
			if (f) { fclose(f); }
		}
		BMP->flipVertical().saveAs(filename.c_str());
		FILE* f = fopen(filename.c_str(), "rb");
		if (!f) { printf("\nERROR: couldn't write file [%s]\n", filename.c_str()); }
		else { printf("Screenshot saved to [%s]\n", filename.c_str()); }
		fclose(f);
	}
};

void cameraKind::screenshot(){
	vec2 scr = getScreenSize();
	int width = (int)scr.x;
	int height = (int)scr.y;
	task_screenshot* task = new task_screenshot(width, height, 0);
	task->run();
}

void cameraKind::screenRead(rect R, bitmap *Bmp){
	if(!Bmp){error("no bmp");}
	int rwidth = (int)R.size.x;
	int rheight = (int)R.size.y;
	task_screenshot* task = new task_screenshot(rwidth, rheight, Bmp);
	task->run();
}

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
	auto& camera = G->gs_camera->g_camera;

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
	error("unreachable code");
	return vec3(0,0,0);
}
vec3 cameraKind::deviceToScreen(vec3 devpos, z_meaning zm){
	auto& camera = G->gs_camera->g_camera;

	float x = devpos.x;
	float y = devpos.y;
	float z = devpos.z;
	vec2 scr = getScreenSize();
	float zfar = 100.f;
	//float zfar_true,zratio;
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
	error("unreachable code");
	return vec3(0,0,0);
}
vec3 cameraKind::getMouseDir(){
	auto& camera = G->gs_camera->g_camera;

	vec2 mouse = getMousePos();
	vec3 wp = screenToWorld({mouse.x,mouse.y,1},Z_IS_ORTHODOX);
	return normalize(wp-camera.pos);
}
#define printval(x) printf(#x ": %f\n", x)
#define zToWorld(z) (znear/(1.f+znear/zfar-z))
#define zToDevice(z) (znear*(1/zfar - 1/z)+1)

vec3 cameraKind::screenToWorld(vec3 scrpos, z_meaning zm){
	vec3 res;
	auto oldperspective = perspective;
	auto oldmode3D = mode3D;
	{
		perspective = true;
		go3D();
		res = deviceToWorld(screenToDevice(scrpos,zm));
	}
	if(!oldmode3D){go2D();}
	perspective = oldperspective;
	return res;
}
vec3 cameraKind::worldToScreen(vec3 worldpos, z_meaning zm){
	//we probably should have a camera always be able to project
	//maybe we need two cameras?
	vec3 res;
	auto oldperspective = perspective;
	auto oldmode3D = mode3D;
	{
		perspective = true;
		go3D();
		res = deviceToScreen(worldToDevice(worldpos),zm);
	}
	if(!oldmode3D){go2D();}
	perspective = oldperspective;
	return res;
}

camprojection cameraKind::getProjection(){
	camprojection cpj;
	cpj.MVP = mProjection*mView;
	cpj.pos = pos;
	return cpj;
}

//cameraKind g_camera;

#include "paint.h"
void go3D(){
	auto& camera = G->gs_camera->g_camera;
	
	camera.go3D();
	setProjection(camera.getProjection());
}

void go2D(){
	auto& camera = G->gs_camera->g_camera;
	
	camera.go2D();
	setProjection(camera.getProjection());
}

