#ifndef MODELLOADER_GUARD
#define MODELLOADER_GUARD
#include "math/vec.h"
#include "loaders/textureLoader.h"
#include "util/globals_resource.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

class model;
model DLLAPI_RESOURCE *loadModel(const char *filepath);

#endif