#include "game/camera.h"
#include "math/vectors.h"
#include "math/quaternions.h"
#include "gui/newconsole.h"
#include "gui/valscreen.h"
void camInit();

vec frustum::getTri(int tri, int vert){ // this is really dumb. why can't we have a normal convex hull generator?
	switch(triangles[tri][vert]){
		case 1: return A; break;
		case 2: return B; break;
		case 3: return C; break;
		case 4: return D; break;
		case 5: return E; break;
		case 6: return F; break;
		case 7: return G; break;
		case 8: return H; break;
	}
}
frustum::frustum(){
	A=B=C=D=E=F=G=H=vec();
	triangles[0][0] = 1;
	triangles[0][1] = 3;
	triangles[0][2] = 2;
	triangles[1][0] = 3;
	triangles[1][1] = 1;
	triangles[1][2] = 4;
	triangles[2][0] = 1;
	triangles[2][1] = 5;
	triangles[2][2] = 4;
	triangles[3][0] = 4;
	triangles[3][1] = 5;
	triangles[3][2] = 8;
	triangles[4][0] = 5;
	triangles[4][1] = 6;
	triangles[4][2] = 8;
	triangles[5][0] = 8;
	triangles[5][1] = 6;
	triangles[5][2] = 7;
	triangles[6][0] = 6;
	triangles[6][1] = 2;
	triangles[6][2] = 7;
	triangles[7][0] = 7;
	triangles[7][1] = 2;
	triangles[7][2] = 3;
	triangles[8][0] = 1;
	triangles[8][1] = 2;
	triangles[8][2] = 6;
	triangles[9][0] = 6;
	triangles[9][1] = 5;
	triangles[9][2] = 1;
	triangles[10][0] = 8;
	triangles[10][1] = 7;
	triangles[10][2] = 4;
	triangles[11][0] = 4;
	triangles[11][1] = 7;
	triangles[11][2] = 3;
}
int frustum::triangles[12][3];


cameraKind::cameraKind(){
	printf("cameraKind constructor start\n");
	pos = {0,0,0};
	angle = quat::zero();
	
	fov = 90;
	//aspectRatio = 4.0/3.0;
	znear = 0.1;
	zfar = 100;
	
	printf("cameraKind constructor end\n");
}
void cameraKind::onMove(){
	//cout << "dist: "<< pos.length() <<", origin to screen: "<< worldtoscreen({0,0,0}).toString() << "\n";
	printvals(toString(this), string("dist: ")+pos.length()+",\n origin to screen: "+worldtoscreen({0,0,0}).toString());
}
void cameraKind::mouseAim(){
	vec deltaMouse;
	deltaMouse.x = mousePos.x/width - 0.5;
	deltaMouse.y = mousePos.y/height - 0.5;
	if(deltaMouse.x or deltaMouse.y){
		angle = angle.addRotation(-90.0*deltaMouse.x, {0,0,1});
		angle = angle.addRotation(-90.0*deltaMouse.y, angle.right());
		onMove();
	}
}
void cameraKind::tick(){
	if(goingForward)
		pos = pos+angle.forward()*convars["camSpeed"];
	if(goingBackward)
		pos = pos-angle.forward()*convars["camSpeed"];
	if(goingUp)
		pos = pos+angle.up()*convars["camSpeed"];
	if(goingDown)
		pos = pos-angle.up()*convars["camSpeed"];
	if(goingRight)
		pos = pos+angle.right()*convars["camSpeed"];
	if(goingLeft)
		pos = pos-angle.right()*convars["camSpeed"];
	if(goingForward or goingBackward or goingUp or goingDown or goingRight or goingLeft){
		onMove();
	}
	getNewFrustum();
}
double cameraKind::getFrustumRight(){return tan(d2r(fov)/2.0)*znear;}
double cameraKind::getFrustumTop(){return tan(d2r(fov*aspect)/2.0)*znear;}
void cameraKind::setFrustum(){
	double top;
	double bottom;
	double left;
	double right;
	aspect = height/width;
	//aspect = (float)width/(float)height;
	
	right = getFrustumRight();
	left = -right;
	top = getFrustumTop();
	bottom = -top;

	glFrustum(left,right,bottom,top,znear,zfar);
}
vec cameraKind::worldtoscreen(vec worldpos){ //algorithmical reversal of screentoworld
	vec lwpos = angle.corotateVector(worldpos-pos);
	//cout << "wp = "<<lwpos.toString() <<", dot = "<< lwpos.dot({0,1,0}) << "\n";
	double depth = lwpos.dot({0,1,0});
	vec lspos = lwpos*(znear/lwpos.y);
	double right = getFrustumRight();
	double top = getFrustumTop();
	double x = lspos.x/(2*right);
	double y = lspos.z/(2*top);
	x = (x+0.5)*width;
	y = (0.5-y)*height;
	return {x,y,depth};
}
vec cameraKind::screentoworld(vec screenpos){
	double x = screenpos.x;
	double y = screenpos.y;
	double depth = screenpos.z;
	aspect = height/width;
	x = (x/width)-0.5; // x as percentage of screen away from center
	y = 0.5-(y/height);
	vec lspos;		//local screen pos
	double right = getFrustumRight(); // length from center to right edge of the nearer frustum side.
	double top = getFrustumTop(); // same for top edge
	lspos.x = x*2*right; //assuming symmetrical screen (center is actually center)
	lspos.z = y*2*top;
	lspos.y = znear;
	
	vec lwpos = lspos.norm()*depth; //local world pos (triangles are similar)
	vec wwpos = pos+angle.rotateVector(lwpos); //world world pos
	
	return wwpos;
}
frustum cameraKind::getFrustum(){
	return curFrustum;
}
void cameraKind::getNewFrustum(){
	frustum viewfrustum; // A - near top left, then CW.
	//printf("H = %f, W = %f\n", height, width);
	//printf("znear = %f, cos1 = %f, cos2 = %f\n", znear, cos(d2r(fov/2.0)), cos(d2r(fov/2.0*aspect)));
	double top = getFrustumTop();
	double right = getFrustumRight();
	double lnear = sqrt(znear*znear+right*right+top*top); // distance to any of 4 vertices of near plane
	double lfar = lnear * zfar / znear;
	//printf("lnear = %f, lfar = %f\n", lnear, lfar);
	viewfrustum.A = screentoworld({0,		0,		lnear});
	viewfrustum.B = screentoworld({width,	0,		lnear});
	viewfrustum.C = screentoworld({width,	height, lnear});
	viewfrustum.D = screentoworld({0, 		height, lnear});
	viewfrustum.E = screentoworld({0,		0,		lfar});
	viewfrustum.F = screentoworld({width,	0,		lfar});
	viewfrustum.G = screentoworld({width,	height,	lfar});
	viewfrustum.H = screentoworld({0,		height,	lfar});
	curFrustum = viewfrustum;
}
void cameraKind::go2D()
{
	glMatrixMode(GL_PROJECTION);
	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}
void cameraKind::go3D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setFrustum();
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
bool cameraKind::screencoordsvisible(vec pos){
	return (pos.z >= 0) and (pos.x >= 0) and (pos.x <= width) and (pos.y >= 0) and (pos.y <= height);
}
bool cameraKind::worldcoordsvisible(vec pos){
	return screencoordsvisible(worldtoscreen(pos));
}
cameraKind camera;

void camGoForward(void* arg){camera.goingForward = true;}
void camStopForward(void* arg){camera.goingForward = false;}
void camGoBackward(void* arg){camera.goingBackward = true;}
void camStopBackward(void* arg){camera.goingBackward = false;}
void camGoUp(void* arg){camera.goingUp = true;}
void camStopUp(void* arg){camera.goingUp = false;}
void camGoDown(void* arg){camera.goingDown = true;}
void camStopDown(void* arg){camera.goingDown = false;}
void camGoRight(void* arg){camera.goingRight = true;}
void camStopRight(void* arg){camera.goingRight = false;}
void camGoLeft(void* arg){camera.goingLeft = true;}
void camStopLeft(void* arg){camera.goingLeft = false;}
void camReset(void* arg){
	cout << "camera pos = "+camera.pos.toString()+", angle = "+camera.angle.toString()+"\n";
	camera.pos = {0,0,0};
	camera.angle = {0,{0,0,1}};
};

void camInit(){
	printf("camInit start\n");
	convars["camSpeed"] = 0.1;
	confuncs["+camforward"] = camGoForward;
	confuncs["-camforward"] = camStopForward;
	confuncs["+cambackward"] = camGoBackward;
	confuncs["-cambackward"] = camStopBackward;
	confuncs["+camup"] = camGoUp;
	confuncs["-camup"] = camStopUp;
	confuncs["+camdown"] = camGoDown;
	confuncs["-camdown"] = camStopDown;
	confuncs["+camright"] = camGoRight;
	confuncs["-camright"] = camStopRight;
	confuncs["+camleft"] = camGoLeft;
	confuncs["-camleft"] = camStopLeft;
	confuncs["camreset"] = camReset;
	printf("camInit end\n");
}