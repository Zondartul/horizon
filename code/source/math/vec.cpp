#define oldNormalize
#include "vec.h"
#include "math.h"
#include "simplemath.h"
#include "globals.h"
#include "stringUtils.h"
//#include "globals.h"
//#include "stdio.h"
//#include "stdlib.h"

string toString(quat q){
    return fstring("(%f*,%s)",angle(q)*r2d,toCString(axis(q)));
}

quat toQuat(vec3 angleVec){
    float L = length(angleVec);
    if(L){
        return glm::angleAxis(length(angleVec),normalize(angleVec));
    }else{
        return glm::angleAxis(0.f,vec3(0,0,1));
    }
}

quat slerpAngle(quat Q1, quat Q2, float ang){
    //find diff such that diff * q1 == q2
    //diff = q2 * inverse(q1)
    //inverse(q1) = conf(q1)/abs(q1)
    //for rotations, abs(q1) = 1
    quat diff = Q2*glm::conjugate(Q1);
    float angD = angle(diff);
    angD = fmodf(angD+180.f,360.f)-180.f; //to go the short way around
    angD = clamp(angD,-180.f,180.f);
    quat diffClamped = glm::angleAxis(angD,axis(diff));
    //printf("slerpAngle:\nQ1 = %s\nQ2 = %s\ndiff = %s\ndiffClamped = %s\n",toCString(Q1),toCString(Q2),toCString(diff),toCString(diffClamped));
    return diffClamped*Q1;
}

vec3 rotate(vec3 V, vec3 angle){
	//float r2d = 1.f/d2r;
	V = rotate(V,-angle.x,{1,0,0});
	V = rotate(V,-angle.y,{0,1,0});
	V = rotate(V,-angle.z,{0,0,1});
	return V;
}

vec3 clamp(vec3 A, vec3 min, vec3 max){
	A.x = clamp(A.x, min.x, max.x);
	A.y = clamp(A.y, min.y, max.y);
	A.z = clamp(A.z, min.z, max.z);
	return A;
}

vec3 operator/(vec3 A, float N){return vec3(A.x/N,A.y/N,A.z/N);}
vec3 operator*(vec3 A, float N){return vec3(A.x*N,A.y*N,A.z*N);}
vec2 clamp(vec2 A, vec2 min, vec2 max){
	A.x = clamp(A.x, min.x, max.x);
	A.y = clamp(A.y, min.y, max.y);
	return A;
}

vec2 operator/(vec2 A, float N){return vec2(A.x/N,A.y/N);}
vec2 operator*(vec2 A, float N){return vec2(A.x*N,A.y*N);}


vec3 normalizeSafe(vec3 V){if(length(V)){return normalize(V);}else{return vec3(0,0,0);}}
vec2 normalizeSafe(vec2 V){if(length(V)){return normalize(V);}else{return vec2(0,0);}}


vec2 toVec2(vec3 A){return vec2(A.x,A.y);}
vec3 toVec3(vec2 A,float z){return vec3(A.x,A.y,z);}

vec3 setX(vec3 A, float x){return vec3(  x,A.y,A.z);}
vec3 setY(vec3 A, float y){return vec3(A.x,  y,A.z);}
vec3 setZ(vec3 A, float z){return vec3(A.x,A.y,  z);}

float cross(vec2 A, vec2 B){return A.x*B.y-A.y*B.x;}

/*
float vec3::dot(vec3 B){
	return x*B.x+y*B.y+z*B.z;
}
vec3 vec3::proj(vec3 B){
	return B.norm()*dot(B.norm());
}
vec3 vec3::cross(vec3 B){
	return	{y*B.z - z*B.y,
			 z*B.x - x*B.z,
			 x*B.y - y*B.x};
}
vec3 vec3::norm(){
	float L = length();
	if(!L){return *this;}
	return (*this)/length();
}
float vec3::length(){
	return sqrt(x*x+y*y+z*z);
}
vec3 vec3::rotate(vec3 axis, float angle){
	float theta = d2r*angle;
	vec3 OP = proj(axis);
	vec3 P = *this - OP;
	vec3 R = P*cos(theta)+P.cross(axis.norm())*sin(theta);
	vec3 F = R+OP;
	// printf("theta: %f\nOP: %s\nP: %s\nR: %s\nF: %s\n",
		// theta,
		// OP.toString().c_str(),
		// P.toString().c_str(),
		// R.toString().c_str(),
		// F.toString().c_str());
	return R+OP;
}
vec3 vec3::rotate(vec3 angle){
	return rotate({1,0,0},angle.x).rotate({0,1,0},angle.y).rotate({0,0,1},angle.z);
}

bool operator==(vec3 A, vec3 B){return (A.x==B.x)&&(A.y==B.y)&&(A.z==B.z);}
vec3 operator*(vec3 A, float N){
	return {A.x*N,A.y*N,A.z*N};
}
vec3 operator*(float N, vec3 A){
	return A*N;
}
vec3 operator/(vec3 A, float N){
	return {A.x/N,A.y/N,A.z/N};
}
vec3 operator/(float N, vec3 A){
	return A/N;
}
vec3 operator+(vec3 A, vec3 B){
	return {A.x+B.x,A.y+B.y,A.z+B.z};
}
vec3 operator-(vec3 A, vec3 B){
	return {A.x-B.x,A.y-B.y,A.z-B.z};
}

//vec3
bool operator==(vec3 A, vec3 B){return (A.r==B.r)&&(A.g==B.g)&&(A.b==B.b);}
vec3 operator*(vec3 A, float N){
	return {A.r*N,A.g*N,A.b*N};
}
vec3 operator*(float N, vec3 A){
	return A*N;
}
vec3 operator/(vec3 A, float N){
	return {A.r/N,A.g/N,A.b/N};
}
vec3 operator/(float N, vec3 A){
	return A/N;
}
vec3 operator+(vec3 A, vec3 B){
	return {A.r+B.r,A.g+B.g,A.b+B.b};
}
vec3 operator-(vec3 A, vec3 B){
	return {A.r-B.r,A.g-B.g,A.b-B.b};
}

//vec2

vec2::operator vec2(){return {x,y};}

vec2 operator-(vec2 A){return {-A.x,-A.y};}
vec2 clamp(vec2 A, vec2 min, vec2 max){
	A.x = clamp(A.x, min.x, max.x);
	A.y = clamp(A.y, min.y, max.y);
	return A;
}
bool operator==(vec2 A, vec2 B){return (A.x==B.x)&&(A.y==B.y);}

vec2 operator*(vec2 A, float N){
	return {A.x*N,A.y*N};
}
vec2 operator*(float N, vec2 A){
	return A*N;
}
vec2 operator/(vec2 A, float N){
	return {A.x/N,A.y/N};
}
vec2 operator/(float N, vec2 A){
	return A/N;
}
vec2 operator+(vec2 A, vec2 B){
	return {A.x+B.x,A.y+B.y};
}
vec2 operator-(vec2 A, vec2 B){
	return {A.x-B.x,A.y-B.y};
}
//vec2

vec2::operator vec2(){return {x,y};}
vec2 clamp(vec2 A, vec2 min, vec2 max){
	A.x = clamp(A.x, min.x, max.x);
	A.y = clamp(A.y, min.y, max.y);
	return A;
}
bool operator==(vec2 A, vec2 B){return (A.x==B.x)&&(A.y==B.y);}
vec2 operator*(vec2 A, float N){
	return {A.x*N,A.y*N};
}
vec2 operator*(float N, vec2 A){
	return A*N;
}
vec2 operator/(vec2 A, float N){
	return {A.x/N,A.y/N};
}
vec2 operator/(float N, vec2 A){
	return A/N;
}
vec2 operator+(vec2 A, vec2 B){
	return {A.x+B.x,A.y+B.y};
}
vec2 operator-(vec2 A, vec2 B){
	return {A.x-B.x,A.y-B.y};
}
*/
//rect
rect::rect(){
	start = {0,0};
	end = {0,0};
	size = {0,0};
}
rect::rect(int width, int height){
	start = {0,0};
	end = {width, height};
	size = {width, height};
}
rect::rect(vec2 size){
	start = {0,0};
	end = size;
	this->size = size;
}
rect::rect(vec2 start, vec2 end){
	//vec2 newstart;
	//vec2 newend;
	//newstart.x = min(start.x,end.x);
	//newstart.y = min(start.y,end.y);
	//newend.x = max(start.x,end.x);
	//newend.y = max(start.y,end.y);
	this->start = start;
	this->end = end;
	this->size = end-start;//start-end;
}
rect::rect(vec2 start, vec2 end, vec2 size){
	//vec2 newstart;
	//vec2 newend;
	//newstart.x = min(start.x,end.x);
	//newstart.y = min(start.y,end.y);
	//newend.x = max(start.x,end.x);
	//newend.y = max(start.y,end.y);
	this->start = start;
	this->end = end;
	this->size = size;//newstart-newend;
}
rect rect::setStart(vec2 start){return {start,end,end-start};}
rect rect::setEnd(vec2 end){return {start,end,end-start};}
rect rect::setSize(vec2 size){return {start,start+size,size};}
rect rect::moveTo(vec2 start){return {start,start+size,size};}
rect rect::moveBy(vec2 relative){return {start+relative,end+relative,size};}
rect rect::repair(){
	vec2 newstart;
	vec2 newend;
	newstart.x = min(start.x,end.x);
	newstart.y = min(start.y,end.y);
	newend.x = max(start.x,end.x);
	newend.y = max(start.y,end.y);
	return {newstart,newend,newstart-newend};
}
vec2 rect::clamp(vec2 V){return ::clamp(V, start, end-vec2(1,1));}
rect rect::clamp(rect R){
	R = R.setStart(clamp(R.start)).setEnd(clamp(R.end));
	return R;
}
//a 0 to 32 rect contains pixels 0..31.
bool rect::contains(vec2 V){return (V.x >= start.x) && (V.x < end.x) && (V.y >= start.y) && (V.y < end.y);}//V == clamp(V);}
vec2 rect::topLeftCorner(){return {start.x,start.y};}
vec2 rect::topRightCorner(){return {end.x,start.y};}
vec2 rect::bottomLeftCorner(){return {start.x,end.y};}
vec2 rect::bottomRightCorner(){return {end.x,end.y};}
vec2 rect::center(){return (topLeftCorner()+bottomRightCorner())/2;}
/*
const char *rect::toString(){
	char *buff;
	int len = snprintf(0,0,"(%d,%d x %d,%d (%d x %d))",start.x,start.y,end.x,end.y,size.x,size.y)+1;
	printf("len = %d\n",len);
	buff = (char*)malloc(sizeof(char)*len);
	if(!buff){printf("can't malloc\n");exit(0);}
	snprintf(buff,len,"(%d,%d x %d,%d (%d x %d))",start.x,start.y,end.x,end.y,size.x,size.y);
	return buff;
} */

rect operator*(rect A, float N){
	return rect(A.start*N,A.end*N);
}
bool operator==(rect A, rect B){
	return (A.start == B.start)&&(A.end == B.end)&&(A.size == B.size);
}

/*
//rectf
rect::rect(){
	start = {0,0};
	end = {0,0};
	size = {0,0};
}
rect::rect(float width, float height){
	start = {0,0};
	end = {width, height};
	size = {width, height};
}
rect::rect(vec2 size){
	start = {0,0};
	end = size;
	this->size = size;
}
rect::rect(vec2 start, vec2 end){
	this->start = start;
	this->end = end;
	size = end-start;
}
rect::rect(vec2 start, vec2 end, vec2 size){
	this->start = start;
	this->end = end;
	this->size = size;
}
rect rect::setStart(vec2 start){return {start,end,end-start};}
rect rect::setEnd(vec2 end){return {start,end,end-start};}
rect rect::setSize(vec2 size){return {start,start+size,size};}
rect rect::moveTo(vec2 start){return {start,start+size,size};}
rect rect::moveBy(vec2 relative){return {start+relative,end+relative,size};}
vec2 rect::clamp(vec2 V){return ::clamp(V, start, end);}
rect rect::clamp(rect R){
	R.setStart(clamp(R.start));
	R.setEnd(clamp(R.end));
	return R;
}
bool rect::contains(vec2 V){return V == clamp(V);}

rect operator*(rect A, float N){
	return rect(A.start*N,A.end*N);
}
*/
//AABB
AABB::AABB(){
	start = vec3(0,0,0);
	end = vec3(0,0,0);
	size = vec3(0,0,0);
}
AABB::AABB(int length, int width, int height){
	start = vec3(0,0,0);
	end = vec3(length, width, height);
	size = vec3(length, width, height);
}
AABB::AABB(vec3 size){
	start = vec3(0,0,0);
	end = size;
	this->size = size;
}
AABB::AABB(vec3 start, vec3 end){
	this->start = start;
	this->end = end;
	size = end-start;
}
AABB::AABB(vec3 start, vec3 end, vec3 size){
	this->start = start;
	this->end = end;
	this->size = size;
}
AABB AABB::setStart(vec3 start){return {start,end,end-start};}
AABB AABB::setEnd(vec3 end){return {start,end,end-start};}
AABB AABB::setSize(vec3 size){return {start,start+size,size};}
AABB AABB::moveTo(vec3 start){return {start,start+size,size};}
AABB AABB::moveBy(vec3 relative){return {start+relative,end+relative,size};}
bool AABB::contains(vec3 point){return ((point.x >= start.x) && (point.x <= end.x) && (point.y >= start.y) && (point.y <= end.y) && (point.z >= start.z) && (point.z <= end.z));}

vec3 toVec3(vec3 V){return vec3(V.x,V.y,V.z);}
vec2 toVec2(vec2 V){return vec2(V.x,V.y);}

vec3 tovec3(vec3 V){return vec3{V.x,V.y,V.z};}
vec2 tovec2(vec2 V){return vec2{V.x,V.y};}

vec3 toVec3Angle(quat q){return axis(q)*angle(q);}

vec3 toVec3Angle(vec3 v){
    quat q_LR = glm::angleAxis((float)atan2(v.y,v.x),vec3(0,0,1));
    quat q_UD = glm::angleAxis((float)atan2(v.z,sqrt(v.x*v.x+v.y*v.y)), vec3(0,-1,0));
    return toVec3Angle(q_LR*q_UD);
}

//builds 3x3 rotation matrices for rotating a vec3 by ang RADIANS around basic axes.
mat3 xrotr(float ang){
	return mat3(1, 0, 0,
				0, cos(ang), -sin(ang),
				0, sin(ang),  cos(ang));
}
mat3 yrotr(float ang){
	return mat3(cos(ang), 0, sin(ang),
					   0, 1, 0,
			   -sin(ang), 0, cos(ang));
}
mat3 zrotr(float ang){
	return mat3(cos(ang), -sin(ang), 0,
				sin(ang),  cos(ang), 0,
				0, 0, 1);
}

