#ifndef TERRAIN_GUARD
#define TERRAIN_GUARD
#include "modelLoader.h"
#include <string>
using std::string;

model generateTerrain(string name,string heightmapname, string texturename, vec3f scale,vec3f tpos);

#endif