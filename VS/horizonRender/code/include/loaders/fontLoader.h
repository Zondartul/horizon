#ifndef FONTLOADER_GUARD
#define FONTLOADER_GUARD
#include "util/globals_render.h"

void DLLAPI_RENDER initFreeType();
struct font;
font DLLAPI_RENDER *loadFont(const char *fontpath, int size);
#endif