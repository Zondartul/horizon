#ifndef ATLAS_GUARD
#define ATLAS_GUARD
#include "math/vec.h"
#include "resource/bitmap.h"
#include "util/globals_resource.h"
#include <vector>
using std::vector;

struct DLLAPI_RESOURCE atlas{
	vector<rect> UVs;	
	bitmap BMP;			
};
atlas DLLAPI_RESOURCE makeAtlas(vector<bitmap> BMPs, bitmap background, bool sort);
atlas DLLAPI_RESOURCE makeAtlasPOT(vector<bitmap> BMPs, bool sort);
#endif