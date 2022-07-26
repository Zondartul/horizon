#ifndef PRINTW_GUARD
#define PRINTW_GUARD
#include "math/vec.h"
#include "util/globals_render.h"
//#include "fonts.h"
//#include "renderLow.h"
//extern vec2 textPos;
//struct font;
//void setFont(font *f);
struct font;

void DLLAPI_RENDER printText2D(const char *text,font *f, vec2 &textPos);
rect DLLAPI_RENDER preprintText2D(const char *text,font *f, vec2 &textPos);
void DLLAPI_RENDER printw(const char *format, ...);
rect DLLAPI_RENDER preprintw(font *F, const char *format, ...);

struct DLLAPI_RENDER gs_printwKind {
	bool g_printFromTop = false;
};

#endif