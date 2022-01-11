#ifndef ATLAS_GUARD
#define ATLAS_GUARD
#include <vector>
using std::vector;
#include "vec.h"
#include "bitmap.h"

struct atlas{
	vector<rect> UVs;	//rectangles that define the extents of each sub-image.
	bitmap BMP;			//final super-image
};

//arranges several bitmaps on a bigger one
//inputs:
//	BMPs -	list of bitmaps to arrange
//	background -the bitmap that defines the size of the resulting
//				atlas and makes up it's background.
//	sort -	if true, images will be arranged using FFDT sorting. 
//			if false, preserves original order.
//			order of rectangles in the atlas.UVs is always preserved. 
atlas makeAtlas(vector<bitmap> BMPs, bitmap background, bool sort);

//same as before, but finds the smallest power-of-two square
//to fit all the images instead of using a background bitmap.
atlas makeAtlasPOT(vector<bitmap> BMPs, bool sort);

#endif