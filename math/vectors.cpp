//vectors.h
#include "vectors.h"
#include "../util/globals.h"


vec::vec(double x1,double y1,double z1){x=x1;y=y1;z=z1;}
vec::vec(){}
bool vec::operator == (vec B) const{return (x==B.x)&&(y==B.y)&&(z==B.z);}
vec vec::operator + (vec B) const{return {x+B.x, y+B.y, z+B.z};}
vec vec::operator - (vec B) const{return {x-B.x, y-B.y, z-B.z};}
vec vec::operator - (){return {-x,-y,-z};}
vec vec::operator * (double b) const{return {x*b,y*b,z*b};}
vec vec::operator * (vec B) const{return {x*B.x,y*B.y,z*B.z};}
vec vec::operator / (double b) const{return {x/b,y/b,z/b};}
bool vec::operator < (const vec B) const{return length() < B.length();}
double vec::dot(vec B) const{return {x*B.x+y*B.y+z*B.z};}
vec vec::cross(vec B) const{return {y*B.z-z*B.y, z*B.x-x*B.z, x*B.y-y*B.x};}
double vec::length() const{return sqrt(pow(x,2)+pow(y,2)+pow(z,2));}
vec vec::norm() const{return *this/length();}
double vec::scalarProjection(vec B)const{return dot(B)/B.length();}
vec vec::vectorProjection(vec B)const{return B.norm()*scalarProjection(B);}
double vec::angle(vec B)const{return acos(dot(B)/(length()*B.length()))*180/M_PI;}
double vec::sgnAngle(vec B, vec ref) const// sign only possible when you know axis of rotation
{return sgn(ref.dot(cross(B)))*acos(dot(B)/(length()*B.length()))*180/M_PI;}
string vec::toString(){
	string str = "";
	return str+"vec:["+x+","+y+","+z+"]";
}

vec operator * (double b, vec a){return a*b;}

// returns true: result is set to point of intersection of ray and triangle
// returns false: result is not modified. No intersection exists.
bool ray_triangle_intersection(vec start, vec dir, vec A, vec B, vec C, vec &result){
	vec N = (B-A).cross(C-A);
	double d = -(N.x*A.x+N.y*A.y+N.z*A.z);
	double dist1 = -(start.dot(N)+d)/(dir.dot(N));
	vec hit1 = start+dir*dist1;
	//inside-outside triangle test
	if(  !((((B-A).cross(hit1-A)).dot(N)>=0)
		&&(((C-B).cross(hit1-B)).dot(N)>=0)
		&&(((A-C).cross(hit1-C)).dot(N)>=0)))
		{return false;}
		result = hit1;
		return true;
}