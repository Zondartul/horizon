#ifndef MODELLOADER_GUARD
#define MODELLOADER_GUARD
#include <string>
using std::string;
#include <vector>
using std::vector;
#include "vec.h"
#include "textureLoader.h"

class model;
model *loadModel(const char *filepath);

struct gs_modelLoaderKind; //in cpp due to opaque type
gs_modelLoaderKind *new_gs_modelLoaderKind();

#endif