#pragma once
#include "util/globals_program.h"

struct DLLAPI_PROGRAM GlobalsProgram {
	GlobalsProgram();
	struct gs_windowKind* gs_window;
	struct sysInputKind* sysInput;
	struct gs_mouseKind* gs_mouse;
};

extern DLLAPI_PROGRAM struct GlobalsProgram* Gp;