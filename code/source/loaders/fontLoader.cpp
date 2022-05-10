#include "fontLoader.h"

#include "fonts.h"
#include "globals.h"
#include "atlas.h"
#include "stringUtils.h"
#include "file.h"
#include <string>
using std::string;

#pragma warning(push, 0)
#include <ft2build.h>
#include FT_FREETYPE_H
#pragma warning(pop)

FT_Library  freetype;
FT_Face		face;

#include "texture.h"
#include "renderLow.h" 
#include "bitmap.h"
#include "simplemath.h"
#include "paint.h"

void initFreeType(){
	if(FT_Init_FreeType(&freetype) != 0)
	{printf("FreeType: could not initialize!\n");exit(0);}
	printf("-------- FreeType init done -------\n");
}

void loadFont_FT(const char *fontpath, int size){
	FILE *f = fopen(fontpath, "rb");
	if(!f){error("can't open file [%s]\n",fontpath);}
	fclose(f);
	int err = FT_New_Face(freetype,
							fontpath,
							0,
							&face);
	if(err == FT_Err_Unknown_File_Format){
		error("FreeType: unsupported file format for [%s]\n", fontpath);
	}else if(err){
		error("FreeType: broken font [%s]\n", fontpath);
	}
	printf("font opened: [%s]\n",fontpath);

	
	err = FT_Set_Pixel_Sizes(
		face,
		0,	
		size);

	if(err){error("FreeType: some error (fixed size?)\n");}
}

void replaceColor(pixel *P){
	if(P->A){
		P->R = 255;
		P->G = 255;
		P->B = 255;
	}
}

bitmap generateGlyphBitmap(char C){
	int charcode = C;
	int glyph_index = FT_Get_Char_Index(face, charcode);
	int error = 0;
	error = FT_Load_Glyph(
		face,
		glyph_index,
		FT_LOAD_DEFAULT);
	if(error){printf("FreeType: some error\n");exit(0);}
	error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
	if(error){printf("FreeType: some error\n");exit(0);}
	
	bitmap BMP;
	BMP.width = face->glyph->bitmap.width;
	BMP.height = face->glyph->bitmap.rows;
	BMP.format = TL_ALPHA;
	if(BMP.numBytes() == 0){BMP.format = TL_RGBA; return BMP;}

	BMP.setBuffer(face->glyph->bitmap.buffer, BMP.numBytes());
	
    
	bitmap BMP2 = BMP.changeFormat(TL_RGBA);
	BMP2.forEachPixel(replaceColor);
	return BMP2;
}
font *loadFont(const char *fontpath, int size){
	loadFont_FT(fontpath, size);
	vector<bitmap> BMPs;
	for(int I = 0; I < 255; I++){
		if(isprint(I)){
			bitmap BMP = generateGlyphBitmap(I);
			BMPs.push_back(BMP);
		}
	}
	atlas A = makeAtlasPOT(BMPs,true);
	bitmap *BMP = new bitmap();
	*BMP = A.BMP.clone();
	font *F = new font();
	int maxy=0;
	int J = 0;
	vec2 start = {0,0};
	vec2 end = {0,0};
	bool first = 0;
	for(int I = 0; I < 255; I++){
		if(isprint(I)){
			texture *t = new texture();
			t->name = string()+"glyph "+(char)I;
			t->bmp = BMP;
			int hscale = BMP->height;
			int wscale = BMP->width;
			rect AUV = A.UVs[J++];
			t->UV = rect(vec2(1.0f*AUV.start.x/wscale,1.0f*AUV.start.y/hscale),vec2(1.0f*AUV.end.x/wscale,1.0f*AUV.end.y/hscale));
			uploadTexture(t);
			glyph G;
			G.t = t;
			
			int glyph_index = FT_Get_Char_Index(face, I);
			int error = 0;
			error = FT_Load_Glyph(
				face,
				glyph_index,
				FT_LOAD_DEFAULT);
			
			G.bearingX = face->glyph->metrics.horiBearingX/64;
			G.bearingY = face->glyph->metrics.horiBearingY/64;
			G.advance = face->glyph->metrics.horiAdvance/64;
			
			F->charmap[I] = G;
			
			int ysize = (int)AUV.size.y;
			int xsize = (int)AUV.size.x;
			
			if(!first){
				first = true;
				start.x = (float)G.bearingX;
				end.x = (float)start.x+xsize;
				start.y = (float)G.bearingY;
				end.y = (float)start.y+ysize;
			}
			start.x = min(start.x, (float)G.bearingX);
			start.y = min(start.y, (float)G.bearingY);
			end.x = max(end.x,start.x+xsize);
			end.y = max(end.y,start.y+ysize);
		}
	}
	F->maxrect = rect(start,end);
	return F;
}

void uploadFont(font *f){
	
}
