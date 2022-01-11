#ifndef PRINTW_GUARD
#define PRINTW_GUARD
#include "vec.h"
#include "fonts.h"
extern vec2f textPos;

void setFont(font f);
void printText2D(const char *text);
rect preprintText2D(const char *text);
void printw(const char *format, ...);
rect preprintw(const char *format, ...);

#endif