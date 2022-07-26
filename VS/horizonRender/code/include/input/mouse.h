#ifndef MOUSE_GUARD
#define MOUSE_GUARD
#include "math/vec.h"
#include "util/globals_render.h"

void DLLAPI_RENDER setMouseRelativeMode(bool mode);
vec2 DLLAPI_RENDER getMousePos();
#endif