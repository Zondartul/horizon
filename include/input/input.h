#ifndef INPUT_GUARD
#define INPUT_GUARD
//#include "codetostring.h"
#include <windows.h>
#include "../util/messenger.h"

class inputKind
{
	public:
	HWND hwnd;
	MessageChannel channel;
	vec2i prevMousePos;
	map<string, bool> keybuffer;
	inputKind();
	vec2i getMousePos();
	bool mouse1down;
	bool mouse2down;
	bool mouse3down;
	char getShifted(char c);
	void keyThing(UINT umsg, WPARAM wParam, LPARAM lParam);
};
extern inputKind input;
#endif
//VIRTUAL KEY TABLE
