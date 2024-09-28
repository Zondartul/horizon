#include "main/main_program.h"
#include "main/main_util.h"
#include "util/global_vars_program.h"
#include "program/window.h"

void initHorizonProgram() {
	static bool initialized = false;
	if (initialized) { return; }
	initialized = true;

	initHorizonUtil();

	Gp = new GlobalsProgram();
	window_init(512, 512);
}
