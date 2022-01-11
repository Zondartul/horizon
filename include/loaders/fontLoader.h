#ifndef FONTLOADER_GUARD
#define FONTLOADER_GUARD
//will do the "fontGenerator" later.
//font *getFont(const char *fontpath, int size);

void initFreeType();
struct font;
font *loadFont(const char *fontpath, int size);

#endif