#ifndef PRINTW_GUARD
#define PRINTW_GUARD
#include "vec.h"
struct font;

void printText2D(const char *text,font *f, vec2 &textPos);
rect preprintText2D(const char *text,font *f, vec2 &textPos);
void printw(const char *format, ...);
rect preprintw(font *F, const char *format, ...);

#endif