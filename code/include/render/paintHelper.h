#ifndef PAINTHELPER_GUARD
#define PAINTHELPER_GUARD
#include "vec.h"
#include <string>
using std::string;

void paintArrow2D(vec3 from, vec3 to, float size);
void paintArrow3D(vec3 from, vec3 to, float size);
void paintCube(vec3 from, vec3 to);
void paint3d2dText(vec3 pos, string text);

#endif