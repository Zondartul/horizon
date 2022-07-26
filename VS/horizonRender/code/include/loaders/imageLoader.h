#ifndef IMAGELOADER_GUARD
#define IMAGELOADER_GUARD
#include "util/globals_render.h"

struct bitmap;
bitmap DLLAPI_RENDER *loadImage(const char *filepath);

#endif