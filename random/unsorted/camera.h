#ifndef CAMERA_GUARD
#define CAMERA_GUARD
#include "vec.h"
//#include "globals.h"

class cameraKind{ //sets up the view and projection matrix, nothing else
public:
	vec3f pos;
	vec3f rot;
	float fov;
	float scale;
	bool mode3D;
	bool perspective;
	cameraKind();
	vec3f forward();
	vec3f up();
	vec3f right();
	float eyetrace(float maxdist);
	void reposition();
	void setPos(vec3f newpos);
	void setRot(vec3f newrot);
	void setFov(float newfov);
	void setScale(float newscale);
	void go2D();
	void go3D();
	void goPerspective();
	void goOrtho();
	void screenshot();
};
extern cameraKind camera;

#endif