#ifndef CAMERA_GUARD
#define CAMERA_GUARD
#include "../math/quaternions.h"

void camInit();
struct frustum{
	vec A,B,C,D,E,F,G,H;
	static int triangles[12][3];
	vec getTri(int tri, int vert);
	frustum();
};

class cameraKind{
	public:
	vec pos;
	quat angle;
	bool goingForward;
	bool goingBackward;
	bool goingUp;
	bool goingDown;
	bool goingLeft;
	bool goingRight;
	double fov; //horizontal fov, degrees
	double aspect;
	double znear;
	double zfar;
	frustum curFrustum;
	cameraKind();
	void onMove();
	void mouseAim();
	void tick();
	double getFrustumRight();
	double getFrustumTop();
	void setFrustum();
	//converts  between continous screen space (right, down, into-screen) and world (x, y, z) coordinates
	vec worldtoscreen(vec worldpos);
	vec screentoworld(vec screenpos);
	//converts between discrete pixel coordinates (x, y) and a normalized world-space (x, y, z) direction
	vec screentoworld_simple(vec2i screenpos);
	vec2i worldtoscreen_simple(vec worldpos);
	//returns world direction vector corresponding to cursor
	vec cursorDir();
	frustum getFrustum();
	void getNewFrustum();
	void go2D();
	void go3D();
	bool screencoordsvisible(vec pos);
	bool worldcoordsvisible(vec pos);
};
extern cameraKind camera;



void camGoForward(void* arg);
void camStopForward(void* arg);
void camGoBackward(void* arg);
void camStopBackward(void* arg);
void camGoUp(void* arg);
void camStopUp(void* arg);
void camGoDown(void* arg);
void camStopDown(void* arg);
void camGoRight(void* arg);
void camStopRight(void* arg);
void camGoLeft(void* arg);
void camStopLeft(void* arg);
void camReset(void* arg);
void camInit();
#endif