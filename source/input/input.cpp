#include "input/input.h"
#include "util/messenger.h"
#include "input/codetostring.h"
#include "windowsx.h" //this one aint mine. wtf?


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
	message msg;
	switch(umsg)
	{
		case(WM_KEYDOWN):
		case(WM_SYSKEYDOWN):
		case(WM_KEYUP):
		case(WM_SYSKEYUP):
		{
			//cout << VKtoString(wParam)<<" ";
			KeyInfo k;
			k.lParam = lParam;
			/*
			cout << "\n";
			cout << "nRepeatCount " << (int)k.Bits.nRepeatCount <<"\n";
			cout << "nScanCode " << (int)k.Bits.nScanCode << "\n";
			cout << "nExtended " << (int)k.Bits.nExtended << "\n";
			cout << "nReserved " << (int)k.Bits.nReserved << "\n";
			cout << "nPrevious " << (int)k.Bits.nPrevious << "\n";
			cout << "nTransition " << (int)k.Bits.nTransition << "\n";
			*/
			string translatedKey = VKtoString(wParam);
			if(k.Bits.nTransition)
			{
				msg.type = "key_up";
				keybuffer[translatedKey] = false;
			}
			else
			if(k.Bits.nPrevious)
			{
				msg.type = "key_still_down";
			}
			else
			{
				msg.type = "key_down";
				keybuffer[translatedKey] = true;
			}
			
			msg.name = translatedKey;
			channel.publish(&msg);
			//printf("[publish: %s]\n", VKtoString(wParam).c_str());
		}
		break;
		
		case(WM_LBUTTONDOWN):{
			msg.type = "lmb_down"; 
			mouse1down = true;
			msg.set(0, (vec2i)getMousePos());
			channel.publish(&msg);
			msg.erase<vec2i>(0);
			
		} break;
		case(WM_LBUTTONUP):{
			msg.type = "lmb_up"; 
			mouse1down = false;
			msg.set(0, (vec2i)getMousePos());
			channel.publish(&msg);
			msg.erase<vec2i>(0);
		} break;
		case(WM_RBUTTONDOWN):{
			msg.type = "rmb_down"; 
			mouse2down = true;
			msg.set(0, (vec2i)getMousePos());
			channel.publish(&msg);
			msg.erase<vec2i>(0);
		} break;
		case(WM_RBUTTONUP):{
			msg.type = "rmb_up"; 
			mouse2down = false;
			msg.set(0, (vec2i)getMousePos());
			channel.publish(&msg);
			msg.erase<vec2i>(0);
		} break;
		case(WM_MOUSEMOVE):
		{
			msg.type = "mouse_move";
			msg.set(0, (vec2i)getMousePos());
			msg.set(1, (vec2i)(getMousePos()-prevMousePos));
			//msg.data.v2i = (getMousePos()-prevMousePos);
			prevMousePos = getMousePos();
			channel.publish(&msg);
			msg.erase<vec2i>(0);
			msg.erase<vec2i>(1);
		} break;
		case(WM_MOUSEWHEEL):
		{
			msg.type = "mouse_wheel"; 
			//msg.data.i = (GET_WHEEL_DELTA_WPARAM(wParam));
			msg.set(0, (int)(GET_WHEEL_DELTA_WPARAM(wParam)));
			channel.publish(&msg);
			msg.erase<int>(0);
		} break;
	}
	//if(msg.type != ""){channel.publish(&msg);}
	
	//different messages for normal and system keys
	//bit for repeated keys
	//extended as in left or right
	//use microsoft's VK_CODE table and make own key name table.
}

inputKind input;
//VIRTUAL KEY TABLE
