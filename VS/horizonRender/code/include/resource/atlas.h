#ifndef ATLAS_GUARD
#define ATLAS_GUARD
#include "math/vec.h"
#include "resource/bitmap.h"
#include <vector>
using std::vector;

struct atlas{
	vector<rect> UVs;	
	bitmap BMP;			
};
atlas makeAtlas(vector<bitmap> BMPs, bitmap background, bool sort);
atlas makeAtlasPOT(vector<bitmap> BMPs, bool sort);
#endif