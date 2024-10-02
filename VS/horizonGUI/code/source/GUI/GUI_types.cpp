#include "GUI/GUI_internal.h"

DEFINE_enum_class_bitwise_operators(GUIe_border);
DEFINE_enum_class_bitwise_operators(GUIe_alignment);
string toString(GUIe_border b){
	string S = "[";
	if(b & GUIb::Top){S += "T";}
	if(b & GUIb::Bottom){S += "B";}
	if(b & GUIb::Left){S += "L";}
	if(b & GUIb::Right){S += "R";} 
	if(b & GUIb::Corner_TR){S += "Ctr";}
	if(b & GUIb::Corner_TL){S += "Ctl";}
	if(b & GUIb::Corner_BR){S += "Cbr";}
	if(b & GUIb::Corner_BL){S += "Cbl";}
	if(b == GUIb::None){S = "[NONE";};
	if(S == "["){S = "[ERROR";}
	S += "]";
	return S;
}

eSysCursor toCursor(GUIe_border border){
	switch(border){
		case(GUIb::Top):
		case(GUIb::Bottom):
			return eSysCursor::CRS_SIZENS;
		case(GUIb::Left):
		case(GUIb::Right):
			return eSysCursor::CRS_SIZEWE;
		case(GUIb::Corner_BL):
		case(GUIb::Corner_TR):
			return eSysCursor::CRS_SIZENESW;
		case(GUIb::Corner_BR):
		case(GUIb::Corner_TL):
			return eSysCursor::CRS_SIZENWSE;
		default:
			return eSysCursor::CRS_ARROW;
	}
}
