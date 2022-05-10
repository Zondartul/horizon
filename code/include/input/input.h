#ifndef INPUT_GUARD
#define INPUT_GUARD
#include "event.h"
void initInput();
struct gs_inputKind {
	eventChannel* g_inputChannel;
};
#endif