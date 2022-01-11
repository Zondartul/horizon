#ifndef PAINT_GUARD
#define PAINT_GUARD
// "paint" is a stateless wrapper to apply
// immediate draw commands and draw settings to GPU.
#include "vec.h"

void clearScreen();
void setViewport(rect viewport);

#endif