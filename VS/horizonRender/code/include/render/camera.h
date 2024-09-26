#ifndef CAMERA_GUARD
#define CAMERA_GUARD
#include "math/vec.h"
#include "util/globals_render.h"
#include "util/stringUtils.h"

enum DLLAPI_RENDER z_meaning {Z_IS_DISTANCE, Z_IS_PLANE, Z_IS_ORTHODOX};

struct DLLAPI_RENDER camprojection{
	mat4 MVP;
	vec3 pos;
};

struct bitmap;

class DLLAPI_RENDER cameraKind{ 
public:
	glm::mat4 mView;			
	glm::mat4 mProjection;		
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
void DLLAPI_RENDER go3D();
void DLLAPI_RENDER go2D();

struct DLLAPI_RENDER gs_cameraKind {
	cameraKind g_camera;
};

//---- stringUtils:
DLLAPI_RENDER string toString(camprojection);	template<> Result<camprojection,zError> fromString<camprojection>(const string S);

#endif