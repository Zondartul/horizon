
#pragma warning(push, 0)
#define STB_IMAGE_IMPLEMENTATION
#include "loaders/stb_image.h"
#pragma warning(pop)
#include "loaders/imageLoader.h"
#include "resource/bitmap.h"
#include "util/globals_render.h"
#include <stdexcept>
#include <sstream>
#include <cstdio>
using std::stringstream;

bitmap *loadImage(const char *filename){
	FILE *f = fopen(filename, "rb");
	if (!f){
		//error("ERROR: loadImage: can't open [%s]\n",filename);
		stringstream ss;
		ss << "ERROR: loadImage: can't open [" << filename << "]";
		throw std::runtime_error(ss.str());
	}
	int width = 0;
	int height = 0;
	int components = 0;
	int requested_components = 4;
	unsigned char *data = stbi_load(filename, &width, &height, &components, requested_components);
	if(!data){
		//error("ERROR: loadImage: opened but can't parse [%s]\n",filename);
		stringstream ss;
		ss << "ERROR: loadImage: opened but can't parse [" << filename << "]\n";
		throw std::runtime_error(ss.str());
	}
	bitmap *BMP = new bitmap();
	BMP->name = filename;
	BMP->width = width;
	BMP->height = height;
	BMP->format = TL_RGBA;
	BMP->setBuffer(data,BMP->numBytes());
	free(data);
	return BMP;
}
