#ifndef MODELLOADER_GUARD
#define MODELLOADER_GUARD
#include <string>
using std::string;
#include <vector>
using std::vector;
#include "vec.h"
#include "textureLoader.h"


// #include <map>
// using std::map;
// extern map<string,model> modelCache;

// model getModel(string name);
class model;
model *loadModel(const char *filepath);

#endif