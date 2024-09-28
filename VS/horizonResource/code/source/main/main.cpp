//#include "pch.h"
#include "main/main_resource.h"
#include "main/main_program.h"
#include "loaders/fontLoader.h" //to initFreeType
#include "resource/resource.h"
//#include "util/global_vars_resource.h"

void initHorizonResource() {
	static bool initialized = false;
	if (initialized) { return; }
	initialized = true;

	initHorizonProgram();
	initFreeType();
	loadAssets();
}
