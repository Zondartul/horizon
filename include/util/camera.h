#ifndef CAMERA_GUARD
#define CAMERA_GUARD
#include "vec.h"
//#include "globals.h"
#include "glm/glm.hpp"

class cameraKind{ //sets up the view and projection matrix, nothing else
public:
	glm::mat4 mView;			//world to camera
	glm::mat4 mProjection;	//camera to screen
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
	vec3f toWorld(vec2i scrpos);	//screen pos becomes world-space direction vector
	vec2i toScreen(vec3f worldpos);	//world-position becomes screen pos
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