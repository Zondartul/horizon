#ifndef TEXTURELOADER_GUARD
#define TEXTURELOADER_GUARD
#include "../util/globals.h"
class texture
{
	public:
	GLuint t;
	int width;
	int height;
	string name;
	texture();
	void free();
	~texture();
};
texture notexture();
extern vector<texture> AllTextures;
texture textureGet(string name);
GLuint LoadTextureRAW(const char *filename, int wrap);
extern bool texture_load_pixelated;
texture GenTextureBMP(string filename);
GLuint LoadtextureBMP(const char *filename);
void FreeTexture(GLuint tex);
#endif