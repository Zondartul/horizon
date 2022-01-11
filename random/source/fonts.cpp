#include "fonts.h"
#include "globals.h"
#include "atlas.h"
#include "stringUtils.h"
#include "selfaware.h"
#include <string>
using std::string;
#include <ft2build.h>
#include FT_FREETYPE_H
FT_Library  freetype;
FT_Face		face;

map<string, font> fontCache;
font getFont(const char *fontname, int size){
	string name = string(fontname)+itoa(size);
	if(!fontCache.count(name)){
		//string fontpath = string("resource/")+fontname+".ttf";
		string path = locateResource("font",fontname);
		fontCache[name] = generateFont(path.c_str(), size);
	}
	return fontCache[name];
}

void initFreeType(){
	if(FT_Init_FreeType(&freetype) != 0)
	{printf("FreeType: could not initialize!\n");exit(0);}	
}

void loadFont(const char *fontpath, int size){
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
	unsigned char *newbuffer = (unsigned char *)malloc(sizeof(unsigned char)*BMP.numBytes());
	memcpy(newbuffer, face->glyph->bitmap.buffer, BMP.numBytes());
	//taking ownership!
	BMP.data.reset(newbuffer);
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
font generateFont(const char *fontpath, int size){
	loadFont(fontpath, size);
	vector<bitmap> BMPs;
	for(int I = 0; I < 255; I++){
		if(isprint(I)){
			bitmap BMP = generateGlyphBitmap(I);
			BMPs.push_back(BMP);
		}
	}
	atlas A = makeAtlasPOT(BMPs,true);
	//dbgAtlas = A;
	textureHandle h = uploadImage(A.BMP);
	//makeTexture("font",h);
	//dbgAtlasTextureName = "font";
	font F;
	int J = 0;
	for(int I = 0; I < 255; I++){
		if(isprint(I)){
			texture t;
			t.handle = h;
			int hscale = h.BMP.height;
			int wscale = h.BMP.width;
			rect AUV = A.UVs[J++];
			t.UV = rectf({1.0f*AUV.start.x/wscale,1.0f*AUV.start.y/hscale},{1.0f*AUV.end.x/wscale,1.0f*AUV.end.y/hscale});
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
			//printf("genfont: char %c, bx=%d, by=%d, ad=%d\n",I,G.bearingX,G.bearingY,G.advance);
			F.charmap[I] = G;
		}
	}
	return F;
}







