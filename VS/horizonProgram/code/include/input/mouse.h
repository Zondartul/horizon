#ifndef MOUSE_GUARD
#define MOUSE_GUARD
#include "math/vec.h"
#include "util/globals_program.h"

void DLLAPI_PROGRAM setMouseRelativeMode(bool mode);
vec2 DLLAPI_PROGRAM getMousePos();

#endif