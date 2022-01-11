#include "printw.h"
#include "textureLoader.h"
#include "paint.h"
#include "stdio.h"

vec2f textPos;
font curFont;

void setFont(font fnt){curFont = fnt;}
void setTextPos(vec2f pos){textPos = pos;}
//todo: switch texture only once
//todo: make a model and stuff
void printText2D(const char *text){
	char C = *text;
	int x=textPos.x,y=textPos.y,I=0;
	//texture ot = currentRenderOptions.tex;
	while(C != 0){
		if(curFont.charmap.count(C)){
			glyph G = curFont.charmap[C];
			texture t = G.t;
			
			//if(C == 'i'){
				//printf("print: t.UV: %s\n",t.UV.toString());
			
			setTexture(t);
			renderCmd(RCMD::TRANSPARENCY,b = true);
			//renderCmd(RCMD::TEXTURE_SELECT, t.handle.t);
			//setColor({0,255,0});
			//drawRectOutline(t.rect().moveBy({150,50}));
			//setColor({0,0,255});
			//drawRectOutline(t.rect().moveTo({x,y}));
			//setColor({255,255,255});
			float scale = 1;//currentRenderOptions.textScale;
			drawTexturedRectUV((t.rect().moveTo({G.bearingX,-G.bearingY})*scale).moveBy({x,y}),t.UV);
			x+=G.advance*scale;
			//}
			//printf("%c",C);
		}else{}//printf("_");}
		I++;
		C = text[I];
	}
	textPos = {x,y};
	//printf("\n");
	//setTexture(ot);
}

#include "simplemath.h"
rect preprintText2D(const char *text){
	int maxx=0,maxy=0,minx=0,miny=0;
	char C = *text;
	int x=0,y=0,I=0;
	//texture ot = currentRenderOptions.tex;
	while(C != 0){
		if(curFont.charmap.count(C)){
			glyph G = curFont.charmap[C];
			texture t = G.t;
			
			//setTexture(t);
			float scale = 1;//textScale;
			rect R = (t.rect().moveTo({G.bearingX,-G.bearingY})*scale).moveBy({x,y});
			
			maxx = max(maxx,R.end.x);
			maxy = max(maxy,R.end.y);
			minx = min(minx,R.start.x);
			miny = min(miny,R.start.y);
			
			x+=G.advance*scale;
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
	printText2D(buff);
}
rect preprintw(const char *format, ...){
	va_list ap;
	va_start(ap, format);
	
	char buff[80];
	vsnprintf(buff,79,format,ap);
	return preprintText2D(buff);
}
