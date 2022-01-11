#ifndef FANCYMATH_GUARD
#define FANCYMATH_GUARD
#include "simplemath.h"
#include <vector>
using std::vector;
#include "vec.h"
//packs 2D rectangles inside another 2D rectangle. 
//inputs:
//	sizes - an unsorted list of rectangle sizes.
//  binsize - the dimensions of the big rectangle into which to fit the smaller ones.
//	sort - if true, Uses FFDH strip sort. if false, elements go in the same order as they arrive.
//outputs:
//	results - new positions for all rectangles given.
//	packed - list of indices of only those rectangles that could actually fit here.
//	heightNeeded - given the bin's width, this is the height necessary and sufficient to fit all items.
void pack2D(vector<vec2i> sizes, 
			vec2i binsize, 
			bool sort, 
			vector<vec2i> *results, 
			vector<int> *packed, 
			int *heightNeeded);

//returns the smallest power-of-two square that can
//fit all the rectangles.
vec2i pack2DfindClosestPOT(vector<vec2i> sizes, bool sort);

#endif