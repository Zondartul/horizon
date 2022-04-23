#include "textureLoader.h"
#include "stdio.h"
#include "globals.h"
#include "imageLoader.h"

//should get a specific texture loader eventually.

texture *loadTexture(const char *filename){
	bitmap *bmp = loadImage(filename);
	if(!bmp){return 0;}
	texture *t = new texture();
	t->bmp = bmp;
	t->UV = rect({0,0},{1,1});
	return t;
}
