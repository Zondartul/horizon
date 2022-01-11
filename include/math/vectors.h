#ifndef VECTORS_GUARD
#define VECTORS_GUARD
#include "../util/globals.h"
//vectors.h

struct vec
{
	double x;
	double y;
	double z;
	vec(double x1,double y1,double z1);
	vec();
	bool operator == (vec B) const;
	vec operator + (vec B) const;
	vec operator - (vec B) const;
	vec operator - ();
	vec operator * (double b) const;
	vec operator * (vec B) const;
	vec operator / (double b) const;
	bool operator < (const vec B) const;
	double dot(vec B) const;
	vec cross(vec B) const;
	double length() const;
	vec norm() const;
	double scalarProjection(vec B)const;
	vec vectorProjection(vec B)const;
	double angle(vec B)const;
	double sgnAngle(vec B, vec ref) const;
	string toString();
};

vec operator * (double b, vec a);

// returns true: result is set to point of intersection of ray and triangle
// returns false: result is not modified. No intersection exists.
bool ray_triangle_intersection(vec start, vec dir, vec A, vec B, vec C, vec &result);
#endif