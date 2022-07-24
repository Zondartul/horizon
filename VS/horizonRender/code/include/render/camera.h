#ifndef CAMERA_GUARD
#define CAMERA_GUARD
#include "math/vec.h"
//#include "globals.h"
//#include "glm/glm.hpp"

// Camera is responsible for projection, transforms, and coordinate systems.
// Also responsible for squares being actually square, correct chirality, and FOV.
// NOT responsible for color correction or lighting.

/*
	Coordinate systems:
		World (vec3)
			|
			|<ModelView matrix>
			v
		camera-relative
			|
			| vec4 = {vec3,w=1}
			| <Projection matrix, different w for persp/ortho>
			v
		4D Clip coordinates
			|
			| perspective division (vec3 = vec4.xyz / w)
			v
		Device
			|
			| glViewport transform
			v
		Screen
	
	Coords:		World(orthodox)	World(game)		Camera		Device			Screen(orthodox)	Screen(game)	
	X range		-inf...inf		-inf...inf					-1.....1		 0..width			 0..width
	Y range		-inf...inf		-inf...inf		same as		-1.....1		 0..height			 0....height
	Z range		-inf...inf		-inf...inf		world,		-1.....1		 0..1				-inf..inf
	X meaning	 left..right	 in....out		+rotate		 left..right	 from 				 from
	Y meaning	 down..up		 left..right	+translate	 down..up		 bottom-left corner	 top-left corner
	Z meaning	 out...in		 down..up					 out...in		 zNear..zFar		 out..in
*/

enum z_meaning {Z_IS_DISTANCE, Z_IS_PLANE, Z_IS_ORTHODOX};
//when z is distance, screen (100,100,1) is 1 unit away from camera.
//when z is plane, screen (100,100,1) is on a plane that is 1 unit away from camera
//when z is orthodox, screen (100,100,1) is on a zFar plane.
//when world point is outside view frustum (znear..zfar and fov) results may be invalid.

struct camprojection{
	mat4 MVP;
	vec3 pos;
};

struct collisioninfo;
struct bitmap;

class cameraKind{ //sets up the view and projection matrix, nothing else
public:
	glm::mat4 mView;			//world to camera
	glm::mat4 mProjection;		//camera to screen
	vec3 pos;
	vec3 rot;
	float fov;
	float scalePersp;
	float scaleOrtho;
	float scale2D;
	bool mode3D;
	bool perspective;
	cameraKind();
	vec3 forward();
	vec3 up();
	vec3 right();
	vec3 worldToDevice(vec3 worldpos);
	vec3 deviceToWorld(vec3 devpos);
	vec3 deviceToScreen(vec3 devpos, z_meaning zm = Z_IS_DISTANCE);
	vec3 screenToDevice(vec3 scrpos, z_meaning zm = Z_IS_DISTANCE);
	vec3 screenToWorld(vec3 devpos, z_meaning zm = Z_IS_DISTANCE);
	vec3 worldToScreen(vec3 worldpos, z_meaning zm = Z_IS_DISTANCE);
	
	vec3 getMouseDir();
	collisioninfo *eyetrace(bool useCursor = true);
	void reposition();
	void setPos(vec3 newpos);
	void setRot(vec3 newrot);
	void setFov(float newfov);
	void go2D();
	void go3D();
	void goPerspective();
	void goOrtho();
	void screenshot();
	void screenRead(rect R, bitmap *Bmp);
	camprojection getProjection();
};
extern cameraKind g_camera;

class rmodel;
rmodel *ray_shotgun(int x_steps, int y_steps);

void go3D();
void go2D();

#endif