#ifndef ATLAS_GUARD
#define ATLAS_GUARD
#include <vector>
using std::vector;
#include "vec.h"
#include "bitmap.h"

struct atlas{
	vector<rect> UVs;	
	bitmap BMP;			
};









atlas makeAtlas(vector<bitmap> BMPs, bitmap background, bool sort);



atlas makeAtlasPOT(vector<bitmap> BMPs, bool sort);

#endif