#ifndef PAINT_GUARD
#define PAINT_GUARD
#include "vec.h"
#include "textureLoader.h"
#include "fonts.h"
#include "modelLoader.h"

#define renderContext for(int rendercontext=0,pushRenderOptions();rendercontext<1;rendercontext++,popRenderOptions())

struct renderOptions{
	vec3ub color1;
	vec3ub color2;
	vec3ub color3;
	vec3ub color4;
	unsigned char alpha;
	float thickness;
	rect scissor;
	bool scissorEnabled;
	int textMaxX;
	int textMaxY;
	vec2i textPos;
	bool infiniteLine;
	bool infinitePlane;
	texture tex;
	font f;
	model m;
	float textScale;
};
extern renderOptions currentRenderOptions;
void pushRenderOptions();
void popRenderOptions();

void initRenderOptions();
void setColor(vec3ub color);
void setColor(vec3ub color1, vec3ub color2);
void setColor(vec3ub color1, vec3ub color2, vec3ub color3);
void setColor(vec3ub color1, vec3ub color2, vec3ub color3, vec3ub color4);
void setAlpha(unsigned char alpha);
void setThickness(float thickness);
void setScissor(rect scissor);
void cropScissor(rect scissor);
void clearScissor();
void setTextMaxX(int textMaxX);
void setTextMaxY(int textMaxY);
void setTextPos(vec2i textPos);
void setInfiniteLine(bool infiniteLine);
void setInfinitePlane(bool infinitePlane);
void setTexture(texture t);
void setFont(font f);
void setModel(model m);
void setTextScale(float scale);

void clearScreen();
void drawPoint2D(vec2i pos);
void drawLine2D(vec2i start, vec2i end);
void drawRect(rect R);
void drawRectOutline(rect R);
void drawTexturedRect(rect R);
void drawTexturedRectUV(rect R, rectf UV);

void drawPoint3D(vec3f pos);
void drawLine3D(vec3f start, vec3f end);
void drawTriangle3D(vec3f A, vec3f B, vec3f C);
void drawModel3D(vec3f pos, vec3f rot, vec3f scale);

void printText2D(const char *text);
rect preprintText2D(const char *text);

void printw(const char *format, ...);
//returns the area taken up by the printed string.
rect preprintw(const char *format, ...);

#endif