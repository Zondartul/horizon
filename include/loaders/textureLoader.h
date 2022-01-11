#ifndef TEXTURELOADER_GUARD
#define TEXTURELOADER_GUARD
#include "texture.h"
/*
struct textureHandle{	//corresponds to a loaded OpenGL texture
	textureHandle();
	bitmap BMP;			//contains image parameters and a pointer to actual data.
	uint32_t t;	//OpenGL texture handle
};

//all textures are owned by the texture manager
struct texture{
	texture();
	//important stuff
	string name;			//name of the texture
	textureHandle handle;	//texture handle tells our program that OpenGL has texture data
	//various fancy drawing properties
	rectf UV;				//An texture need not be the entire image, but it can be some sub-image.
	int w();
	int h();
	vec2i size();
	rect rect();
};
*/


//enum pixelType{TL_UNSIGNED_BYTE, TL_UNSIGNED_SHORT_5_6_5, TL_UNSIGNED_SHORT_4_4_4_4, TL_UNSIGNED_SHORT_5_5_5_1};

//returns a texture by the given name.
//if no such texture exists, attempts to 
//load a texture by that filename.
//texture *getTexture(string name);
//texture makeTexture(string name, textureHandle handle);
//bitmap *loadImage(const char *filename);
texture *loadTexture(const char *filename);
//textureHandle uploadImage(bitmap BMP);

#endif