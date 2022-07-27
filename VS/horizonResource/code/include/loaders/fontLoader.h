#ifndef FONTLOADER_GUARD
#define FONTLOADER_GUARD
#include "util/globals_resource.h"

void DLLAPI_RESOURCE initFreeType();
struct font;
font DLLAPI_RESOURCE *loadFont(const char *fontpath, int size);
#endif