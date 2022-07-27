#include "loaders/textureLoader.h"
#include "loaders/imageLoader.h"
#include "util/globals_resource.h"
#include <cstdio>

texture *loadTexture(const char *filename){
	bitmap *bmp = loadImage(filename);
	if(!bmp){return 0;}
	texture *t = new texture();
	t->bmp = bmp;
	t->UV = rect({0,0},{1,1});
	return t;
}
