#include "util/global_vars_program.h"
#include "program/window.h"
#include "input/input.h"
struct GlobalsProgram* Gp = 0;

GlobalsProgram::GlobalsProgram() {
	gs_window = new gs_windowKind();
	sysInput = new sysInputKind();
}