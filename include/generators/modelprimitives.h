#ifndef MODELPRIMITIVES_GUARD
#define MODELPRIMITIVES_GUARD
#include "vec.h"
//struct rmodel;

//rmodel *generateBox(vec3 scale = vec3(1,1,1),float texscale = 1.0f);
struct e_model;

e_model *generateBox(vec3 scale = vec3(1,1,1),float texscale = 1.0f);
e_model *generateCyllinder(float r, float h, int numsides = 16);
e_model *generateCone(float r, float h, int numsides = 16);
e_model *generateSphere(float r, int numsides = 16, int numslices = 8);

#endif