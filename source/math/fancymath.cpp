#include "fancymath.h"
#include <algorithm>

void pack2D(vector<vec2> sizes, vec2 binsize, bool sort, vector<vec2> *results, vector<int> *packed, int *heightNeeded){
	vector<int> sorted;
	results->clear();
	packed->clear();
	*heightNeeded = 0;
	for(int I = 0; I < sizes.size(); I++){
		sorted.push_back(I);
		results->push_back({0,0});
	}
	if(sort){
		std::sort(sorted.begin(),sorted.end(),
			[&](int A, int B)->bool{
				if(sizes[A].y > sizes[B].y){return true;}
				if(sizes[A].y == sizes[B].y){
					if(sizes[A].x > sizes[B].y){return true;}
				}
				return false;
			}
		);
	}
	int x=0,y=0,I=0,lineHeight=0,widthleft=0,selected=0,overflow=0;
	pack2D_oneLine:
		pack2D_oneRect:
			lineHeight = max(lineHeight,sizes[sorted[0]].y);
			if(*heightNeeded+lineHeight > binsize.y){overflow = 1;}
			if(sorted.size() == 0){goto pack2D_done;}
			widthleft = binsize.x-x;
			if(widthleft < sizes[sorted[0]].x){
				if(!sort){goto pack2D_nextLine;}
				//find the biggest one that will still fit
				for(I = 0; I < sorted.size(); I++){
					if(sizes[sorted[I]].x < widthleft){
						selected = sorted[I];
						sorted.erase(sorted.begin()+I);
						if(!overflow){packed->push_back(selected);}
						goto pack2D_plopDown;
					}
				}
				goto pack2D_nextLine;
			}
			selected = sorted[0];
			sorted.erase(sorted.begin());
			if(!overflow){packed->push_back(selected);}
			pack2D_plopDown:
				(*results)[selected].x = x;
				(*results)[selected].y = y;
				x = x+sizes[selected].x;
				goto pack2D_oneRect;
		pack2D_nextLine:
			x = 0;
			y = y+lineHeight;
			*heightNeeded = *heightNeeded + lineHeight;
			lineHeight = 0;
			goto pack2D_oneLine;
	pack2D_done:
		*heightNeeded = *heightNeeded + lineHeight;
		printf("2D packing complete: %d/%d items packed\n"
			"width = %d, height = %d / %d needed\n",
			packed->size(),sizes.size(),binsize.x,binsize.y,*heightNeeded);
		return;
}

vec2 pack2DfindClosestPOT(vector<vec2> sizes, bool sort){
	//obviously, the POT can't have a smaller area
	//than all the rectangles put together.
	int area = 0;
	for(int I = 0; I < sizes.size(); I++){
		area += sizes[I].x*sizes[I].y;
	}
	int side = ceil(sqrt(area));
	int n = log2ceil(side);
	
	vector<vec2> results;
	vector<int> packed;
	int heightNeeded;
	do{
		side = exp2(n);
		pack2D(sizes,{side,side},sort,&results,&packed,&heightNeeded);
		n++;
	}while(packed.size() < sizes.size());
	return {side,side};
}
