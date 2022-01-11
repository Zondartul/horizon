#include "input/input.h"
#include "util/messenger.h"
#include "input/codetostring.h"
#include "windowsx.h" //this one aint mine. wtf?
#include "input/inputMessage.h"

inputKind::inputKind(){
	prevMousePos = getMousePos();
}
vec2i inputKind::getMousePos()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(hwnd, &p);
	vec2i pos = {p.x,p.y};
	return pos;
}
/*void keyDown(int kb)
{
	cout << "keyDown: [" << kb << "] - " << decode(kb) << "\n";
}
void keyUp(int kb)
{
	//cout << "keyUp: [" << kb << "]\n";
}*/
char inputKind::getShifted(char c){
	string initial = "`1234567890-=qwertyuiop[]\\asdfghjkl;\'zxcvbnm,./";
	string shifted = "~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>\?";
	for(int I = 0; I < initial.length(); I++){
		if(initial[I] == c)
			return shifted[I];
	}
	return c;
}
void inputKind::keyThing(UINT umsg, WPARAM wParam, LPARAM lParam)
{
	//char newmessage[100];
	//TranslateWMessage(umsg, newmessage);
	//cout << message << " ["<<wParam<<"]["<<lParam<<"]["<<umsg<<"]\n";
	message_key msg_key;
	message_mouse msg_mouse;
	message_mouse_move msg_mouse_move;
	message_mouse_wheel msg_mouse_wheel;
	switch(umsg)
	{
		case(WM_KEYDOWN):
		case(WM_SYSKEYDOWN):
		case(WM_KEYUP):
		case(WM_SYSKEYUP):
		{
			KeyInfo k;
			k.lParam = lParam;
			string translatedKey = VKtoString(wParam);
			if(k.Bits.nTransition)
			{
				msg_key.type = "key_up";
				keybuffer[translatedKey] = false;
			}
			else
			if(k.Bits.nPrevious)
			{
				msg_key.type = "key_still_down";
			}
			else
			{
				msg_key.type = "key_down";
				keybuffer[translatedKey] = true;
			}
			
			msg_key.key = translatedKey;
			channel.publish(&msg_key);
			//printf("[publish: %s]\n", VKtoString(wParam).c_str());
		}
		break;
		
		case(WM_LBUTTONDOWN):{
			msg_mouse.type = "lmb_down"; 
			mouse1down = true;
			msg_mouse.newPos = getMousePos();
			channel.publish(&msg_mouse);
			
		} break;
		case(WM_LBUTTONUP):{
			msg_mouse.type = "lmb_up"; 
			mouse1down = false;
			msg_mouse.newPos = getMousePos();
			channel.publish(&msg_mouse);
		} break;
		case(WM_RBUTTONDOWN):{
			msg_mouse.type = "rmb_down"; 
			mouse2down = true;
			msg_mouse.newPos = getMousePos();
			channel.publish(&msg_mouse);
		} break;
		case(WM_RBUTTONUP):{
			msg_mouse.type = "rmb_up"; 
			mouse2down = false;
			msg_mouse.newPos = getMousePos();
			channel.publish(&msg_mouse);
		} break;
		case(WM_MOUSEMOVE):
		{
			msg_mouse_move.type = "mouse_move";
			msg_mouse_move.newPos = getMousePos();
			msg_mouse_move.deltaPos = getMousePos()-prevMousePos;
			prevMousePos = getMousePos();
			channel.publish(&msg_mouse_move);
		} break;
		case(WM_MOUSEWHEEL):
		{
			msg_mouse_wheel.type = "mouse_wheel";
			msg_mouse_wheel.deltaWheel = (int)(GET_WHEEL_DELTA_WPARAM(wParam));
			channel.publish(&msg_mouse_wheel);
		} break;
	}
	//if(msg.type != ""){channel.publish(&msg);}
	
	//different messages for normal and system keys
	//bit for repeated keys
	//extended as in left or right
	//use microsoft's VK_CODE table and make own key name table.
}

inputKind input;
//VIRTUAL KEY TABLE... exists somewhere.
