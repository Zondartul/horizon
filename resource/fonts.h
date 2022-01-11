#ifndef FONTS_GUARD
#define FONTS_GUARD
#include <gl/gl.h>
#include <string>
using std::string;
// fonts.h creates letter textures from fonts

struct glyphkind
{
	int bearingX;
	int bearingY;
	int sizeX;
	int sizeY;
	int advance;
	GLuint texture;
};

typedef glyphkind* font;

void setFont(font F);
font GenerateFont(const char* filepath, int size, bool aa);
int printw(int x, int y, int xlim, int ylim, string format, ...);
void fontFree(font F);
int printwrich(int x, int y, int xlim, int ylim, int* defcolor, string format, ...);
extern int FF_version_major;
extern int FF_version_minor;
extern int FF_version_patch;
#endif