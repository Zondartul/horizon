#ifndef QUATERNION_GUARD
#define QUATERNION_GUARD
#include "../math/vectors.h"
extern double deg2rad;
extern double rad2deg;

struct quat
{
	double w;
	vec v;
	
	quat operator -();		
	quat operator *(double N);
	quat operator *(quat B);
	static quat zero();
	double abs();
	quat conj();
	quat norm();
	static quat fromAngle(double p, double y, double r);
	static quat fromAngleAxis(double w, vec v);
	static quat fromAngleAxis(double w, double x, double y, double z);
	quat inv();
	quat qmod();
	vec getRotationAxis();
	double getRotationAngle();
	quat addRotation(double ang, vec vect);
	vec corotateVector(vec vect);
	vec rotateVector(vec vect);
	static quat from2vecs(vec X1, vec Y1);
	static quat from4vecs(vec X1, vec Y1, vec X2, vec Y2);
	
	quat operator +(quat B);
	quat operator -(quat B);
	double getX();
	double getY();
	double getZ();
	vec getAxis();
	double getAngle();
	vec forward();
	vec right();
	vec up();
	vec localX();
	vec localY();
	vec localZ();
	string toString();
};
#endif