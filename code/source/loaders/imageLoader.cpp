#include "imageLoader.h"
#include "bitmap.h"
#include "stdio.h"
#include "globals.h"
#define STB_IMAGE_IMPLEMENTATION
//lets support EVERYTHING :D
#include "stb_image.h"

bitmap *loadImage(const char *filename){
	FILE *f = fopen(filename, "rb");
	if(!f){error("ERROR: loadImage: can't open [%s]\n",filename);}
	int width = 0;
	int height = 0;
	int components = 0;
	int requested_components = 4;
	unsigned char *data = stbi_load(filename, &width, &height, &components, requested_components);
	if(!data){error("ERROR: loadImage: opened but can't parse [%s]\n",filename);}

	
	bitmap *BMP = new bitmap();
	BMP->name = filename;
	BMP->width = width;
	BMP->height = height;
	BMP->format = TL_RGBA;
	BMP->setBuffer(data,BMP->numBytes());
	free(data);
	
	return BMP;
}
