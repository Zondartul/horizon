#ifndef FONTLOADER_GUARD
#define FONTLOADER_GUARD


void initFreeType();
struct font;
font *loadFont(const char *fontpath, int size);

#endif