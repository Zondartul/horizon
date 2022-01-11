#ifndef INPUT_GUARD
#define INPUT_GUARD
//#include "codetostring.h"
//#include <windows.h> how dare thee
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
//! singleton class intended to receive all window input and send it over a MessageChannel (messenger.h)
class inputKind
{
	public:
	//HWND hwnd;						//handle of the current window (for getting mouse pos and such)
	messageChannel channel;			//!< channel where all events will be posted
	vec2i prevMousePos;				//!< (internal) mouse position at previous mouse move event.
	map<string, bool> keybuffer;	//!< a keyboard buffer - shows which keys are still down
	inputKind();					//!< constructor
	vec2i getMousePos();			//!< returns window-relative mouse position
	bool mouse1down;				/*!< keeps track of mouse button 1*/
	bool mouse2down;				/*!< keeps track of mouse button 2*/
	bool mouse3down;				/*!< keeps track of mouse button 3*/
	char getShifted(char c);		/*!< figures out the character you pressed by combining shift and another keyboard key.
										\param c the character whose keyboard key that was pressed.
										\return the alternative character that corresponds to the same keyboard key.
									*/
	//void keyThing(UINT umsg, WPARAM wParam, LPARAM lParam);	//OS messages are fed into this function.
	void keyThing(SDL_Event event); //!< SDL messages are fed into this function.
									//!\param event an event fired by the SDL library. Only input-related events will have any effect.
};
extern inputKind input;	//!<singleton declaration (created in input.cpp)
#endif
//VIRTUAL KEY TABLE
