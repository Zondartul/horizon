//---
#include "main/main_util.h"
#include "util/global_vars_util.h"
#include <iostream>
using namespace std;
//void openGUI();

void initHorizonUtil() {
	static bool initialized = false;
	if (initialized) { return; }
	initialized = true;

	Gu = new GlobalsUtil();
}
