#include "vec.h"
#include "math.h"
#include "simplemath.h"
#include "globals.h"
//#include "globals.h"
//#include "stdio.h"
//#include "stdlib.h"

float vec3f::dot(vec3f B){
	return x*B.x+y*B.y+z*B.z;
}
vec3f vec3f::proj(vec3f B){
	return B.norm()*dot(B.norm());
}
vec3f vec3f::cross(vec3f B){
	return	{y*B.z - z*B.y,
			 z*B.x - x*B.z,
			 x*B.y - y*B.x};
}
vec3f vec3f::norm(){
	float L = length();
	if(!L){return *this;}
	return (*this)/length();
}
float vec3f::length(){
	return sqrt(x*x+y*y+z*z);
}
vec3f vec3f::rotate(vec3f axis, float angle){
	float theta = d2r*angle;
	vec3f OP = proj(axis);
	vec3f P = *this - OP;
	vec3f R = P*cos(theta)+P.cross(axis.norm())*sin(theta);
	vec3f F = R+OP;
	// printf("theta: %f\nOP: %s\nP: %s\nR: %s\nF: %s\n",
		// theta,
		// OP.toString().c_str(),
		// P.toString().c_str(),
		// R.toString().c_str(),
		// F.toString().c_str());
	return R+OP;
}
vec3f vec3f::rotate(vec3f angle){
	return rotate({1,0,0},angle.x).rotate({0,1,0},angle.y).rotate({0,0,1},angle.z);
}

bool operator==(vec3f A, vec3f B){return (A.x==B.x)&&(A.y==B.y)&&(A.z==B.z);}
vec3f operator*(vec3f A, float N){
	return {A.x*N,A.y*N,A.z*N};
}
vec3f operator*(float N, vec3f A){
	return A*N;
}
vec3f operator/(vec3f A, float N){
	return {A.x/N,A.y/N,A.z/N};
}
vec3f operator/(float N, vec3f A){
	return A/N;
}
vec3f operator+(vec3f A, vec3f B){
	return {A.x+B.x,A.y+B.y,A.z+B.z};
}
vec3f operator-(vec3f A, vec3f B){
	return {A.x-B.x,A.y-B.y,A.z-B.z};
}

//vec3ub
bool operator==(vec3ub A, vec3ub B){return (A.r==B.r)&&(A.g==B.g)&&(A.b==B.b);}
vec3ub operator*(vec3ub A, float N){
	return {A.r*N,A.g*N,A.b*N};
}
vec3ub operator*(float N, vec3ub A){
	return A*N;
}
vec3ub operator/(vec3ub A, float N){
	return {A.r/N,A.g/N,A.b/N};
}
vec3ub operator/(float N, vec3ub A){
	return A/N;
}
vec3ub operator+(vec3ub A, vec3ub B){
	return {A.r+B.r,A.g+B.g,A.b+B.b};
}
vec3ub operator-(vec3ub A, vec3ub B){
	return {A.r-B.r,A.g-B.g,A.b-B.b};
}

//vec2i

vec2i::operator vec2f(){return {x,y};}

vec2i operator-(vec2i A){return {-A.x,-A.y};}
vec2i clamp(vec2i A, vec2i min, vec2i max){
	A.x = clamp(A.x, min.x, max.x);
	A.y = clamp(A.y, min.y, max.y);
	return A;
}
bool operator==(vec2i A, vec2i B){return (A.x==B.x)&&(A.y==B.y);}

vec2i operator*(vec2i A, float N){
	return {A.x*N,A.y*N};
}
vec2i operator*(float N, vec2i A){
	return A*N;
}
vec2i operator/(vec2i A, float N){
	return {A.x/N,A.y/N};
}
vec2i operator/(float N, vec2i A){
	return A/N;
}
vec2i operator+(vec2i A, vec2i B){
	return {A.x+B.x,A.y+B.y};
}
vec2i operator-(vec2i A, vec2i B){
	return {A.x-B.x,A.y-B.y};
}
//vec2f

vec2f::operator vec2i(){return {x,y};}
vec2f clamp(vec2f A, vec2f min, vec2f max){
	A.x = clamp(A.x, min.x, max.x);
	A.y = clamp(A.y, min.y, max.y);
	return A;
}
bool operator==(vec2f A, vec2f B){return (A.x==B.x)&&(A.y==B.y);}
vec2f operator*(vec2f A, float N){
	return {A.x*N,A.y*N};
}
vec2f operator*(float N, vec2f A){
	return A*N;
}
vec2f operator/(vec2f A, float N){
	return {A.x/N,A.y/N};
}
vec2f operator/(float N, vec2f A){
	return A/N;
}
vec2f operator+(vec2f A, vec2f B){
	return {A.x+B.x,A.y+B.y};
}
vec2f operator-(vec2f A, vec2f B){
	return {A.x-B.x,A.y-B.y};
}
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
rect::rect(vec2i size){
	start = {0,0};
	end = size;
	this->size = size;
}
rect::rect(vec2i start, vec2i end){
	this->start = start;
	this->end = end;
	size = end-start;
}
rect::rect(vec2i start, vec2i end, vec2i size){
	this->start = start;
	this->end = end;
	this->size = size;
}
rect rect::setStart(vec2i start){return {start,end,end-start};}
rect rect::setEnd(vec2i end){return {start,end,end-start};}
rect rect::setSize(vec2i size){return {start,start+size,size};}
rect rect::moveTo(vec2i start){return {start,start+size,size};}
rect rect::moveBy(vec2i relative){return {start+relative,end+relative,size};}
vec2i rect::clamp(vec2i V){return ::clamp(V, start, end);}
rect rect::clamp(rect R){
	R = R.setStart(clamp(R.start)).setEnd(clamp(R.end));
	return R;
}
bool rect::contains(vec2i V){return V == clamp(V);}
vec2i rect::topLeftCorner(){return {start.x,start.y};}
vec2i rect::topRightCorner(){return {end.x,start.y};}
vec2i rect::bottomLeftCorner(){return {start.x,end.y};}
vec2i rect::bottomRightCorner(){return {end.x,end.y};}
vec2i rect::center(){return (topLeftCorner()+bottomRightCorner())/2;}
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

//rectf
rectf::rectf(){
	start = {0,0};
	end = {0,0};
	size = {0,0};
}
rectf::rectf(float width, float height){
	start = {0,0};
	end = {width, height};
	size = {width, height};
}
rectf::rectf(vec2f size){
	start = {0,0};
	end = size;
	this->size = size;
}
rectf::rectf(vec2f start, vec2f end){
	this->start = start;
	this->end = end;
	size = end-start;
}
rectf::rectf(vec2f start, vec2f end, vec2f size){
	this->start = start;
	this->end = end;
	this->size = size;
}
rectf rectf::setStart(vec2f start){return {start,end,end-start};}
rectf rectf::setEnd(vec2f end){return {start,end,end-start};}
rectf rectf::setSize(vec2f size){return {start,start+size,size};}
rectf rectf::moveTo(vec2f start){return {start,start+size,size};}
rectf rectf::moveBy(vec2f relative){return {start+relative,end+relative,size};}
vec2f rectf::clamp(vec2f V){return ::clamp(V, start, end);}
rectf rectf::clamp(rectf R){
	R.setStart(clamp(R.start));
	R.setEnd(clamp(R.end));
	return R;
}
bool rectf::contains(vec2f V){return V == clamp(V);}
/* const char *rectf::toString(){
	char *buff;
	int len = snprintf(0,0,"(%f,%f x %f,%f (%f x %f))",start.x,start.y,end.x,end.y,size.x,size.y)+1;
	printf("len = %d\n",len);
	buff = (char*)malloc(sizeof(char)*len);
	if(!buff){printf("can't malloc\n");exit(0);}
	snprintf(buff,len,"(%f,%f x %f,%f (%d x %f))",start.x,start.y,end.x,end.y,size.x,size.y);
	return buff;
} */

rectf operator*(rectf A, float N){
	return rectf(A.start*N,A.end*N);
}

//AABB
AABB::AABB(){
	start = {0,0};
	end = {0,0};
	size = {0,0};
}
AABB::AABB(int length, int width, int height){
	start = {0,0,0};
	end = {length, width, height};
	size = {length, width, height};
}
AABB::AABB(vec3f size){
	start = {0,0,0};
	end = size;
	this->size = size;
}
AABB::AABB(vec3f start, vec3f end){
	this->start = start;
	this->end = end;
	size = end-start;
}
AABB::AABB(vec3f start, vec3f end, vec3f size){
	this->start = start;
	this->end = end;
	this->size = size;
}
AABB AABB::setStart(vec3f start){return {start,end,end-start};}
AABB AABB::setEnd(vec3f end){return {start,end,end-start};}
AABB AABB::setSize(vec3f size){return {start,start+size,size};}
AABB AABB::moveTo(vec3f start){return {start,start+size,size};}
AABB AABB::moveBy(vec3f relative){return {start+relative,end+relative,size};}

vec3 toVec3(vec3f V){return vec3(V.x,V.y,V.z);}
vec2 toVec2(vec2f V){return vec2(V.x,V.y);}

vec3f toVec3f(vec3 V){return (vec3f){V.x,V.y,V.z};}
vec2f toVec2f(vec2 V){return (vec2f){V.x,V.y};}



