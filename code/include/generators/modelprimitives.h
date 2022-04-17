#ifndef MODELPRIMITIVES_GUARD
#define MODELPRIMITIVES_GUARD
#include "vec.h"

struct e_model;

e_model *generateBox(vec3 scale = vec3(1,1,1),float texscale = 1.0f);
e_model *generateCyllinder(float r, float h, int numsides = 16, float texscale = 1.0f);
e_model *generateCone(float r, float h, int numsides = 16, float texscale = 1.0f);
e_model *generateSphere(float r, int numsides = 16, int numslices = 8, float texscale = 1.0f);
e_model *generateSheet(int numx, int numy, vec3 scale = vec3(1,1,1), float texscale = 1.0f);
e_model *generatePlane(vec3 normal, float offset, vec2 size = vec2(1,1), float texscale = 1.0f);

#endif