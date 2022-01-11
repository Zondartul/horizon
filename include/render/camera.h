#ifndef CAMERA_GUARD
#define CAMERA_GUARD
#include "vec.h"
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
	//float distToZ(float dist);
	vec3 worldToDevice(vec3 worldpos);
	vec3 deviceToWorld(vec3 devpos);
	vec3 deviceToScreen(vec3 devpos, z_meaning zm = Z_IS_DISTANCE);
	vec3 screenToDevice(vec3 scrpos, z_meaning zm = Z_IS_DISTANCE);
	vec3 screenToWorld(vec3 devpos, z_meaning zm = Z_IS_DISTANCE);
	vec3 worldToScreen(vec3 worldpos, z_meaning zm = Z_IS_DISTANCE);
	
	vec3 getMouseDir();
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