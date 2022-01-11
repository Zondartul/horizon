#ifndef PAINT_GUARD
#define PAINT_GUARD
#include <gl/gl.h>
//#include <gl/glu.h>
#include "../util/globals.h"
// lol
void paintRect(int X1, int Y1, int X2, int Y2);
void paintTexturedRect(int X1, int Y1, int X2, int Y2, GLuint tex);
void paintCircle(int X1, int Y1, int Radius);
void paintCircleOutline(int X1, int Y1, int Radius);
void paintRectOutline(int X1, int Y1, int X2, int Y2);
void setColor(color3i color);
void setAlpha(byte alpha);
extern color3i paintColor;
extern byte paintAlpha;
#endif