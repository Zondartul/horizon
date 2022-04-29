#include <cstdarg>
#include "printw.h"
#include "textureLoader.h"
#include "paint.h"
#include "stdio.h"
#include "fonts.h"
#include "renderLayer.h"
#include "rmodel.h"
#include "simplemath.h"
#include "global_vars.h"

void drawRectImmediate(rect R){
	auto& currentLayer = G->gs_paint->g_currentLayer;

	rmodel *rm = new rmodel();
	vec3 A = vec3(R.start.x,R.start.y,0.0f);
	vec3 B = vec3(R.start.x,R.end.y,0.0f);
	vec3 C = vec3(R.end.x,R.start.y,0.0f);
	vec3 D = vec3(R.end.x,R.end.y,0.0f);
	vec2 UVA = vec2(0,0);//*0.1f;
	vec2 UVB = vec2(0,1);//*0.1f;
	vec2 UVC = vec2(1,0);//*0.1f;
	vec2 UVD = vec2(1,1);//*0.1f;
	rm->vertices->push_back(A);
	rm->vertices->push_back(B);
	rm->vertices->push_back(C);
	rm->vertices->push_back(B);
	rm->vertices->push_back(D);
	rm->vertices->push_back(C);

	rm->uvs->push_back(UVA);
	rm->uvs->push_back(UVB);
	rm->uvs->push_back(UVC);
	rm->uvs->push_back(UVB);
	rm->uvs->push_back(UVD);
	rm->uvs->push_back(UVC);


	rm->colors->push_back(vec3(1,0,0));
	rm->colors->push_back(vec3(1,1,1));
	rm->colors->push_back(vec3(1,1,1));
	rm->colors->push_back(vec3(1,1,1));
	rm->colors->push_back(vec3(1,0,0));
	rm->colors->push_back(vec3(1,1,1));

	rm->finalize();
    renderLayer *oldL = currentLayer;
    renderLayer *L = new renderLayer("drawRectImmediate",false,true);
    setLayer(L);
    uploadRmodel(rm);
    drawRmodel(rm);
    deleteRmodel(rm);
    L->render();
    L->clear();
    delete L;
    setLayer(oldL);
}

void printText2D(const char *text, font *F, vec2 &textPos){
	auto& currentLayer = G->gs_paint->g_currentLayer;
	auto& printFromTop = G->gs_printw->g_printFromTop;

	renderLayer *oldL = currentLayer;
    renderLayer *L = new renderLayer("printText2D",false,true);
    setLayer(L);
    if(!text){return;}
	char C = *text;
	int x=(int)textPos.x,y=(int)textPos.y,I=0;
	setTexturing(true);

    setTransparency(true);
	float yoffset = 0;
	if(printFromTop){yoffset = F->maxrect.end.y;}
	float scale = 1;
	while(C != 0){
		if(F->charmap.count(C)){
			glyph G = F->charmap[C];
			texture *t = G.t;
			setTexture(t);
			drawRect((t->getRect().moveTo({G.bearingX,-G.bearingY})*scale).moveBy({x,y+yoffset}));


			x+=(int)(G.advance*scale);
		}else{
			if(C == '\n'){
				y+=(int)(F->maxrect.size.y);
				x = (int)(textPos.x);
			}
		}
		 I++;
		 C = text[I];
	 }
	 textPos = vec2(x,y);

    setTexturing(false);
    L->render();
    L->clear();
    delete L;
    setLayer(oldL);
}

rect preprintText2D(const char *text, font *F){
	auto& printFromTop = G->gs_printw->g_printFromTop;

	int maxx=0,maxy=0,minx=0,miny=0;
	int x=0,y=0,I=0;
	if(!text){return rect();}
	char C = *text;
	float scale = 1;
	float yoffset = 0;
	if(printFromTop){yoffset = F->maxrect.end.y;}
	while(C != 0){
		if(F->charmap.count(C)){
			glyph G = F->charmap[C];
			texture *t = G.t;
			rect R = (t->getRect().moveTo({G.bearingX,-G.bearingY})*scale).moveBy({x,y+yoffset});
			if(I == 0){
				maxx = (int)R.end.x;
				maxy = (int)R.end.y;
				minx = (int)R.start.x;
				miny = (int)R.start.y;
			}else{
				maxx = (int)max(maxx,(int)R.end.x);
				maxy = (int)max(maxy,(int)R.end.y);
				minx = (int)min(minx,(int)R.start.x);
				miny = (int)min(miny,(int)R.start.y);
			}
			x+=(int)(G.advance*scale);
		}else{
			if(C == '\n'){
				y+=(int)F->maxrect.size.y;
				x = 0;
			}
		}
		I++;
		C = text[I];
	}
	return rect({minx,miny},{maxx,maxy});
}

//only up to 1k characters are supported
//because it's really hard to calculate the length of
//a printf'ed string without printing it.
//MS' version of vsnprintf returns -1 instead of
//needed num characters when supplied with 0 buffer.
//2021: we really should get rid of all printf-style output and switch to streams
//they can be customized too
void printw(const char *format, ...){
	va_list ap;
	va_start(ap, format);

	char buff[1000];
	vsnprintf(buff,1000,format,ap);
	printText(buff);
}
rect preprintw(font *F, const char *format, ...){
	va_list ap;
	va_start(ap, format);

	char buff[1000];
	vsnprintf(buff,1000,format,ap);
	return preprintText2D(buff,F);
}
