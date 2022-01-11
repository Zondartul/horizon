#ifndef INPUT_GUARD
#define INPUT_GUARD
//#include "codetostring.h"
#include <windows.h>
#include "../util/messenger.h"

//singleton class intended to receive all window input
//and send it over a MessageChannel (messenger.h)
//message types posted:
//	type:			name:	data:					description:
//	key_down		<key>							a key was pressed on the keyboard.
//	key_still_down	<key>							OS has sent a repeat signal for a key that is still pressed.
//	key_up			<key>							a key was released.
//	lmb_down				0:(vec2i)newPos			left mouse button was pressed.
//	lmb_up					0:(vec2i)newPos			left mouse button was released.
//	rmb_down				0:(vec2i)newPos			right mouse button was pressed.
//	rmb_up					0:(vec2i)newPos			right mouse button was released.
//	mouse_move				0:(vec2i)newPos			mouse was moved.
//							1:(vec2i)deltaPos
//	mouse_wheel				0:(int)deltaWheel		mouse wheel was turned.

class inputKind
{
	public:
	HWND hwnd;						//handle of the current window (for getting mouse pos and such)
	messageChannel channel;			//channel where all events will be posted
	vec2i prevMousePos;
	map<string, bool> keybuffer;	//a keyboard buffer - shows which keys are still down
	inputKind();					//constructor
	vec2i getMousePos();			//returns window-relative mouse position
	bool mouse1down;
	bool mouse2down;
	bool mouse3down;
	char getShifted(char c);		//returns the character that results from holding shift while pressing the given key.
	void keyThing(UINT umsg, WPARAM wParam, LPARAM lParam);	//OS messages are fed into this function.
};
extern inputKind input;	//singleton declaration (created in input.cpp)
#endif
//VIRTUAL KEY TABLE
