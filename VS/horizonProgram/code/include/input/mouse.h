#ifndef MOUSE_GUARD
#define MOUSE_GUARD
#include "math/vec.h"
#include "util/globals_program.h"

void DLLAPI_PROGRAM setMouseRelativeMode(bool mode);
vec2 DLLAPI_PROGRAM getMousePos();
enum eSysCursor{
    CRS_ARROW,
    CRS_IBEAM,
    CRS_WAIT,
    CRS_CROSSHAIR,
    CRS_WAITARROW,
    CRS_SIZENWSE,
    CRS_SIZENESW,
    CRS_SIZEWE,
    CRS_SIZENS,
    CRS_SIZEALL,
    CRS_NO,
    CRS_HAND
};

struct Cursor{
    const uint8_t* data;
    const uint8_t* mask;
    int w;
    int h;
    int hot_x;
    int hot_y;
};

DLLAPI_PROGRAM struct gs_mouseKind{
    std::vector<void*> cursors;
};
void DLLAPI_PROGRAM initMouse();

void DLLAPI_PROGRAM setCursor(eSysCursor new_cursor);

void* DLLAPI_PROGRAM createCursor(Cursor curs);

#endif