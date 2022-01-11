#ifndef TEXTURELOADER_GUARD
#define TEXTURELOADER_GUARD
#include "../util/globals.h"

//a texture descriptor (body not included)
class texture
{
	public:
	GLuint t;	//OpenGL texture ID number
	int width; 	//texture dimensions in pixels
	int height;
	string name;//texture name
	texture();
	void free();//deallocate texture body
	~texture();
};
//represents the lack of a texture
texture notexture(); 

//the storage of all texture headers (bodies are handled by OpenGL)
extern map<string,texture> AllTextures;

//load the texture into the texture storage and return it
texture LoadTexture(const char *filename);

//load texture if it doesn't exist and return a pointer to it's descriptor
texture *getTextureHandle(string name);

//look up loaded texture by name
texture textureGet(string name);

//load the texture as a header-less BMP
texture LoadTextureRAW(const char *filename);

//load the texture as a BMP
texture LoadTextureBMP(const char *filename);

//load the texture as a PNG
texture LoadTexturePNG(const char *filename);

//new textures should wrap-around
extern bool texture_load_wrap;

//new textures should be pixelated
extern bool texture_load_pixelated;


//free the texture data
void FreeTexture(GLuint tex);
#endif