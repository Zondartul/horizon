#ifndef INPUT_GUARD
#define INPUT_GUARD
//#include "codetostring.h"
#include <windows.h>
#include "../util/messenger.h"

class inputKind
{
	public:
	HWND hwnd;
	PSchannel channel;
	vec2i prevMousePos;
	map<string, bool> keybuffer;
	inputKind();
	vec2i getMousePos();
	char getShifted(char c);
	void keyThing(UINT umsg, WPARAM wParam, LPARAM lParam);
};
#endif
//VIRTUAL KEY TABLE
