#ifndef IMAGELOADER_GUARD
#define IMAGELOADER_GUARD
#include "util/globals_resource.h"

struct bitmap;
bitmap DLLAPI_RESOURCE *loadImage(const char *filepath);

#endif