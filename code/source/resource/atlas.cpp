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
	for(int I = 0; I < BMPs.size(); I++){
		vec2 size = vec2(BMPs[I].width,BMPs[I].height);
		sizes.push_back(size);
		//if(isprint(I)){printf("size %c: %s\n",I,toString(size).c_str());}
	}
	binsize = {background.width,background.height};
	pack2D(sizes,binsize,sort,&results,&packed,&heightNeeded);
	for(int I = 0; I < results.size(); I++){
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
	for(int I = 0; I < BMPs.size(); I++){
		//sizes.push_back({BMPs[I].width,BMPs[I].height});	
		vec2 size = vec2(BMPs[I].width,BMPs[I].height);
		sizes.push_back(size);
		//if(isprint(I)){printf("size %c: %s\n",I,toString(size).c_str());}
	}
	binsize = pack2DfindClosestPOT(sizes, sort);	
	bitmap background = blankBitmap(binsize.y,binsize.x,TL_RGBA);
	pack2D(sizes,binsize,sort,&results,&packed,&heightNeeded);
	for(int I = 0; I < results.size(); I++){
		background.insert(BMPs[I],results[I]);
		rect R = rect(sizes[I]).moveTo(results[I]);
		rects.push_back(R);
		//if(isprint(I)){printf("rect %c: %s\n",I,toString(R).c_str());}
	}
	return {rects, background};
}

/*
switch(){
	case(TL_ALPHA):
	break;
	case(TL_RGB):
	break;
	case(TL_RGBA):
	break;
	case(TL_LUMINANCE):
	break;
	case(TL_LUMINANCE_ALPHA):
	break;
}
*/


	/* vector<rect> rects;
	for(int I = 0; I < BMPs.size(); I++){
		rects.push_back(rect(BMPs[I].width,BMPs[I].height));
	}
	pack2D(rects, rect(background.width,background.height));
	for(int I = 0; I < BMPs.size(); I++){
		background.insert(BMPs[I],(*UVs_out)[I].start);
	} */
	/* printf("sorting: unpacked.size() = %d\n",unpacked.size());
	for(int I = 0; I < unpacked.size(); I++){
		printf("%d ",unpacked[I]);
	}
	printf("end unpacked\n"); */
/* 
//clusterfuck warning
atlas makeAtlas(vector<bitmap> BMPs, bitmap background, bool sort){
	vector<int> unpacked;
	vector<rect> UVs;
	for(int I = 0; I < BMPs.size(); I++){
		unpacked.push_back(I);
		UVs.push_back(rect());
	}
	if(sort){
		std::sort(unpacked.begin(),unpacked.end(),
			[&](int A, int B)->bool{
				//printf("sort %d, %d, &BMPs = %p\n",A,B,&BMPs);
				//if((A < 0) || (A >= BMPs.size())){exit(1/0);}
				//if((B < 0) || (B >= BMPs.size())){exit(1/0);}
				if(BMPs[A].height > BMPs[B].height){return true;}
				if(BMPs[A].height == BMPs[B].height){
					if(BMPs[A].width > BMPs[B].width){return true;}
				}
				return false;
			}
		);
	}
	int x=0,y=0;
	int I = 0;
	bitmap nextBMP = BMPs[unpacked[I]];
	int lineHeight = nextBMP.height;
	while((I < BMPs.size()) && (lineHeight < background.height)){
		while((I < BMPs.size()) && (x+BMPs[unpacked[I]].width < background.width)){	
			nextBMP = BMPs[unpacked[I]];
			lineHeight = max(lineHeight,nextBMP.height);
			background.insert(nextBMP,{x,y});
			UVs[unpacked[I]] = rect(nextBMP.width,nextBMP.height).moveTo({x,y});
			x = x+nextBMP.width;
			I++;
		}
		x=0;
		y+=lineHeight;
		lineHeight = BMPs[I].height;
	}
	
	return {UVs,background};
} */


/* 
//returns number of boxes packed
vector<rect> stripPack2D(vector<rect> boxes, int width, int *height_out){
	//1) sort boxes tallest-shortest, then widest-thinnest.
	vector<int> unpacked;
	for(int I = 0; I < boxes.size(); I++){
		unpacked.push_back(I);
	}
	std::sort(unpacked.begin(),unpacked.end(),
		[&](int A, int B){
			if(boxes[A].size.y > boxes[B].size.y){return true;}
			if(boxes[A].size.x > boxes[B].size.y){return true;}
			return false;
		}
	);
	//2) take the biggest box and plop it down.
	int boxI = unpacked[0];
	unpacked.pop_front();
	
}
*/