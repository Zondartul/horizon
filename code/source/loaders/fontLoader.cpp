#include "fontLoader.h"

#include "fonts.h"
#include "globals.h"
#include "atlas.h"
#include "stringUtils.h"
//#include "selfaware.h"
#include "file.h"
#include <string>
using std::string;
#include <ft2build.h>
#include FT_FREETYPE_H
FT_Library  freetype;
FT_Face		face;

#include "texture.h"
#include "renderLow.h" //to submit rendercommand (upload texture)
#include "bitmap.h"
#include "simplemath.h"
// map<string, font> fontCache;
// font getFont(const char *fontname, int size){
	// string name = string(fontname)+itoa(size);
	// if(!fontCache.count(name)){
		////string fontpath = string("resource/")+fontname+".ttf";
		// string path = locateResource("font",fontname);
		// fontCache[name] = generateFont(path.c_str(), size);
	// }
	// return fontCache[name];
// }
#include "paint.h"

void initFreeType(){
	if(FT_Init_FreeType(&freetype) != 0)
	{printf("FreeType: could not initialize!\n");exit(0);}
	printf("-------- FreeType init done -------\n");
}

void loadFont_FT(const char *fontpath, int size){
	//const char *fontpath = "resource/calibri.ttf";
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
	/* error = FT_Set_Char_Size(
		face,
		0,		//width in typographic points/64
		16*64,
		300,	//device resolution to convert from points to pixels
		300); */
	//alternative:
	err = FT_Set_Pixel_Sizes(
		face,
		0,	//width in pixels (0 means same)
		size);//height in pixels 
	
	// for(int i = 0; i < 255; i++){
		// generateCharTexture(i);
	// }
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
	//unsigned char *newbuffer = new unsigned char[BMP.numBytes()]; //(unsigned char *)malloc(sizeof(unsigned char)*BMP.numBytes());
	//vector<unsigned char> *newbuff = new vector<unsigned char>();
	//newbuff->resize(BMP.numBytes());
	//memcpy(newbuff->data(), face->glyph->bitmap.buffer, BMP.numBytes());
	//taking ownership!
	//BMP.data.reset(newbuff);
	BMP.setBuffer(face->glyph->bitmap.buffer, BMP.numBytes());
	//free(face->glyph->bitmap.buffer); // wait, is this really ours?
    //nope, turns out we do not own it.
	bitmap BMP2 = BMP.changeFormat(TL_RGBA);
	BMP2.forEachPixel(replaceColor);
	return BMP2;
	// int width = face->glyph->bitmap.width;
	// int height = face->glyph->bitmap.rows;
	// unsigned char *data1 = face->glyph->bitmap.buffer;
	// unsigned char *data2 = (unsigned char*)malloc(sizeof(unsigned char)*width*height*4);
	// for(int y = 0; y < height; y++){
		// for(int x = 0; x < width; x++){
			// int I = width*y+x;
			// unsigned char alpha = data1[I];
			// I = I*4;
			// data2[I] = 255*(bool)alpha;
			// data2[I+1] = 255*(bool)alpha;
			// data2[I+2] = 255*(bool)alpha;
			// data2[I+3] = alpha;
		// }
	// }
	/* unsigned char *data3 = (unsigned char*)malloc(sizeof(unsigned char)*64*64*4);
	for(int y = 0; y < 64; y++){
		for(int x = 0; x < 64; x++){
			int I1 = (64*y+x)*4;
			if((y < height) && (x < width)){
				int I2 = (width*y+x)*4;
				data3[I1] = data2[I2];
				data3[I1+1] = data2[I2+1];
				data3[I1+2] = data2[I2+2];
				data3[I1+3] = data2[I2+3];
			}else{
				data3[I1] = 0;
				data3[I1+1] = 0;
				data3[I1+2] = 0;
				data3[I1+3] = 0;
			}
		}
	} */
	//loadTextureFromMemory("font_calibri",height,width,data2);
	//loadTextureFromMemory("font_calibri",64,64,data3);
}
//#include "globals.h"
font *loadFont(const char *fontpath, int size){
	loadFont_FT(fontpath, size);
	vector<bitmap> BMPs;
	for(int I = 0; I < 255; I++){
		if(isprint(I)){
			bitmap BMP = generateGlyphBitmap(I);
			//printf("char %c size: %dx%d\n", I,BMP.height,BMP.width);
			BMPs.push_back(BMP);
		}
	}
	atlas A = makeAtlasPOT(BMPs,true);
	bitmap *BMP = new bitmap();
	*BMP = A.BMP.clone();
	//dbgAtlas = A;
	//textureHandle h = uploadImage(A.BMP); //HMMMMMMMMM
	//uploadImage(A.BMP);
	//makeTexture("font",h);
	//dbgAtlasTextureName = "font";
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
			//t.handle = h; nah, use the bmp handle
			int hscale = BMP->height;
			int wscale = BMP->width;
			rect AUV = A.UVs[J++];
			t->UV = rect(vec2(1.0f*AUV.start.x/wscale,1.0f*AUV.start.y/hscale),vec2(1.0f*AUV.end.x/wscale,1.0f*AUV.end.y/hscale));
			//t->UV = t->UV.setSize(-t->UV.size);
			//printf("t->UV: %s\n",toString(t->UV).c_str());
			//rqueue->push_back(new rcmd_texture_upload(t));
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
			//if(G.bearingY > maxy){maxy = G.bearingY;}
			
			F->charmap[I] = G;
			
			int ysize = AUV.size.y;
			int xsize = AUV.size.x;
			
			//if(hscale > 100){printf("glyph %d [%c] has ysize of %d\n",I,(char)I,ysize);}
			//if(wscale > 100){printf("glyph %d [%c] has xsize of %d\n",I,(char)I,xsize);}
			
			if(!first){
				first = true;
				start.x = G.bearingX; 
				end.x = start.x+xsize;
				start.y = G.bearingY;
				end.y = start.y+ysize;
			}
			start.x = min(start.x,G.bearingX);
			start.y = min(start.y,G.bearingY);
			end.x = max(end.x,start.x+xsize);
			end.y = max(end.y,start.y+ysize);
			//if(start.x > maxrect.start.x){start.x = maxrect.start.x;}
			//if(end.x < maxrect.end.x){end.x = maxrect.end.x;}
			//if(start.y > maxrect.start.y){start.y = maxrect.start.y;}
			//if(end.y < maxrect.start.x){start.x = maxrect.start.x;}
			
			//if(I == 0 || G.bearingY +hscale > maxrect.end.y){maxrect.setEnd({maxrect.end.x,G.bearingY+hscale});}
			//if(I == 0 || G.bearingX +wscale > maxrect.end.x){maxrect.setEnd({G.bearingX+wscale,maxrect.end.y});}
			//if(I == 0 || G.bearingY < maxrect.start.y){maxrect.setStart({maxrect.start.x,G.bearingY});}
			//if(I == 0 || G.bearingX < maxrect.start.x){maxrect.setStart({G.bearingX,maxrect.start.y});}
			//printf("genfont: char %c, bx=%d, by=%d, ad=%d\n",I,G.bearingX,G.bearingY,G.advance);
		}
	}
	//F->ysize = maxy;
	F->maxrect = rect(start,end);
	return F;
}

void uploadFont(font *f){
	
}
