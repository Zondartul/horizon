#ifndef PAINT_GUARD
#define PAINT_GUARD
//#include <gl/gl.h>
//#include <gl/glu.h>
#include "util/globals.h"
#include "resource/textureloader.h"
// lol
void paintInit(); //apply default settings
//
void paintLine(int X1, int Y1, int X2, int Y2);
void paintLine(vec2i P1, vec2i P2);
void paintLine(); //uses current settings
void paintRect(int X1, int Y1, int X2, int Y2);
void paintRect(vec2i P1, vec2i P2);
void paintRect(rect A);
void paintRect();
void paintTexturedRect(int X1, int Y1, int X2, int Y2, texture *tex);
void paintTexturedRect(rect A, texture *tex);
void paintTexturedRect();
void paintCircle(int X1, int Y1, int Radius);
void paintCircleOutline(int X1, int Y1, int Radius);
void paintCircle();
void paintRectOutline(int X1, int Y1, int X2, int Y2);
void paintRectOutline(rect);
void paintRectOutline();
void paintTriangle(int X1, int Y1, int X2, int Y2, int X3, int Y3);
void paintTriangle(vec2i P1, vec2i P2, vec2i P3);
void paintTriangle();
void setColor(color3i color);
void setAlpha(byte alpha);
void setColorAlpha(color4i coloralpha);
void setScissor(rect scissor);
extern color3i paintColor;
extern byte paintAlpha;
extern vec2i paintP1, paintP2, paintP3;
extern texture *paintTex;
extern rect paintScissor;
extern bool paintScissorEnabled;
//an object of this class specifies temporary settings for the paint system
//these settings are reverted automatically when the object is destroyed.
//When object is first created, it takes on the current settings.
class tempPaintSettings{
	protected:
	color3i color;
	byte alpha;
	vec2i P1;
	vec2i P2;
	vec2i P3;
	texture *tex;
	rect scissor;
	bool scissorEnabled;
	public:
	tempPaintSettings();
	~tempPaintSettings();
	tempPaintSettings &setColor(color3i color);
	tempPaintSettings &setAlpha(byte alpha);
	tempPaintSettings &setP1(int X, int Y);
	tempPaintSettings &setP1(vec2i P);
	tempPaintSettings &setP2(int X, int Y);
	tempPaintSettings &setP2(vec2i P);
	tempPaintSettings &setP3(int X, int Y);
	tempPaintSettings &setP3(vec2i P);
	tempPaintSettings &setRect(rect R);
	tempPaintSettings &setTexture(texture *tex);
	tempPaintSettings &setScissor(rect scissor);
	tempPaintSettings &crop(rect scissor);
	tempPaintSettings &setScissorEnabled(bool scissorEnabled);
	color3i getColor();
	byte getAlpha();
	vec2i getP1();
	vec2i getP2();
	vec2i getP3();
	texture *getTexture();
	rect getScissor();
	bool getScissorEnabled();
};
#endif
















