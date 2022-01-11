#include "printw.h"
#include "textureLoader.h"
#include "paint.h"
#include "stdio.h"
#include "fonts.h"
#include "renderLayer.h"
//vec2 textPos;
//font *curFont;
#include "rmodel.h"
bool printFromTop = false;//true;
void drawRectImmediate(rect R){
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
	rcmd_rmodel_upload(rm).execute();
	rcmd_rmodel_render(rm).execute();
	rcmd_rmodel_delete(rm).execute();
}
//void setFont(font *fnt){curFont = fnt;}
//void setTextPos(vec2 pos){textPos = pos;}
//todo: switch texture only once / actually, texture refers to a rectangle on a bitmap, so switch texture != switch bitmap
//todo: make a model and stuff
void printText2D(const char *text, font *F, vec2 &textPos){
	if(!text){return;}
	char C = *text;
	int x=textPos.x,y=textPos.y,I=0;
	//setTexturing(true);
	rcmd_texturing(true).execute();
	//setTransparency(true);
	rcmd_transparency(true).execute();
	float yoffset = 0;
	if(printFromTop){yoffset = F->maxrect.end.y;}
	float scale = 1;
	while(C != 0){
		if(F->charmap.count(C)){
			glyph G = F->charmap[C];
			texture *t = G.t;
			//setTexture(t);
			rcmd_texture_select(t).execute();
			drawRectImmediate((t->rect().moveTo({G.bearingX,-G.bearingY})*scale).moveBy({x,y+yoffset}));
			
			x+=G.advance*scale;
		}else{
			if(C == '\n'){
				y+=F->maxrect.size.y;
				x = textPos.x;
			}
		}
		 I++;
		 C = text[I];
	 }
	 textPos = vec2(x,y);
	
	rcmd_texturing(false).execute();
}

#include "simplemath.h"

rect preprintText2D(const char *text, font *F){
	int maxx,maxy,minx,miny;
	int x=0,y=0,I=0;
	if(!text){return rect();}
	char C = *text;
	//texture ot = currentRenderOptions.tex;
	float scale = 1;//textScale;
	float yoffset = 0;
	if(printFromTop){yoffset = F->maxrect.end.y;}
	while(C != 0){
		if(F->charmap.count(C)){
			glyph G = F->charmap[C];
			texture *t = G.t;
			rect R = (t->rect().moveTo({G.bearingX,-G.bearingY})*scale).moveBy({x,y+yoffset});
			if(I == 0){
				maxx = R.end.x; 
				maxy = R.end.y; 
				minx = R.start.x; 
				miny = R.start.y;
			}else{
				maxx = max(maxx,R.end.x);
				maxy = max(maxy,R.end.y);
				minx = min(minx,R.start.x);
				miny = min(miny,R.start.y);
			}
			x+=G.advance*scale;
		}else{
			if(C == '\n'){
				y+=F->maxrect.size.y;
				x = 0;
			}
		}
		I++;
		C = text[I];
	}
	//setTexture(ot);
	//firstn(10,printf("preprint: %d %d %d %d\n",minx,miny,maxx,maxy));
	//onceon(10,printf("preprint output suppressed\n"));
	return rect({minx,miny},{maxx,maxy});
}

#include "stdarg.h"
void printw(const char *format, ...){
	va_list ap;
	va_start(ap, format);
	
	char buff[80];
	vsnprintf(buff,79,format,ap);
	//printText2D(buff);
	printText(buff);
}
rect preprintw(font *F, const char *format, ...){
	va_list ap;
	va_start(ap, format);
	
	char buff[80];
	vsnprintf(buff,79,format,ap);
	return preprintText2D(buff,F);
}
