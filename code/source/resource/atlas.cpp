#include "atlas.h"
#include "fancymath.h"
#include "globals.h"
#include "stringUtils.h"

atlas makeAtlas(vector<bitmap> BMPs, bitmap background, bool sort){
	vector<vec2> sizes;
	vec2 binsize;
	vector<vec2> results;
	vector<int> packed;
	int heightNeeded;
	vector<rect> rects;
	for(unsigned int I = 0; I < BMPs.size(); I++){
		vec2 size = vec2(BMPs[I].width,BMPs[I].height);
		sizes.push_back(size);
	}
	binsize = {background.width,background.height};
	pack2D(sizes,binsize,sort,&results,&packed,&heightNeeded);
	for(unsigned int I = 0; I < results.size(); I++){
		background.insert(BMPs[I],results[I]);
		rects.push_back(rect(sizes[I]).moveTo(results[I]));
	}
	return {rects, background};
}

atlas makeAtlasPOT(vector<bitmap> BMPs, bool sort){
	vector<vec2> sizes;
	vec2 binsize;
	vector<vec2> results;
	vector<int> packed;
	int heightNeeded;
	vector<rect> rects;
	for(unsigned int I = 0; I < BMPs.size(); I++){
		vec2 size = vec2(BMPs[I].width,BMPs[I].height);
		sizes.push_back(size);
	}
	binsize = pack2DfindClosestPOT(sizes, sort);	
	bitmap background = blankBitmap(binsize.y,binsize.x,TL_RGBA);
	pack2D(sizes,binsize,sort,&results,&packed,&heightNeeded);
	for(unsigned int I = 0; I < results.size(); I++){
		background.insert(BMPs[I],results[I]);
		rect R = rect(sizes[I]).moveTo(results[I]);
		rects.push_back(R);
	}
	return {rects, background};
}
