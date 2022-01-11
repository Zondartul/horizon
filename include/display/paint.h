#ifndef PAINT_GUARD
#define PAINT_GUARD
//#include <gl/gl.h>
//#include <gl/glu.h>
#include "util/globals.h"
#include "resource/textureloader.h"
// lol
void paintLine(int X1, int Y1, int X2, int Y2);
void paintLine(vec2i P1, vec2i P2);
void paintRect(int X1, int Y1, int X2, int Y2);
void paintRect(rect A);
void paintTexturedRect(int X1, int Y1, int X2, int Y2, texture *tex);
void paintRect(rect A, texture *tex);
void paintCircle(int X1, int Y1, int Radius);
void paintCircleOutline(int X1, int Y1, int Radius);
void paintRectOutline(int X1, int Y1, int X2, int Y2);
void paintRectOutline(rect);
void setColor(color3i color);
void setAlpha(byte alpha);
extern color3i paintColor;
extern byte paintAlpha;
#endif