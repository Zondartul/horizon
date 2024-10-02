#ifndef GUI_TYPES_GUARD
#define GUI_TYPES_GUARD
#include "util/util.h"
#include "math/vec.h"
#include "util/globals_gui.h"
#include "input/mouse.h"

struct font;
struct texture;

struct DLLAPI_GUI GUI_border_rects{
	rect Rtop;
	rect Rbottom;
	rect Rleft;
	rect Rright;
	rect Rctl; 
	rect Rctr; 
	rect Rcbl; 
	rect Rcbr; 
};

typedef GUI_border_rects GUIbr;

struct DLLAPI_GUI GUI_border_size{
	float top = 3;
	float bottom = 3;
	float left = 3;
	float right = 3;
	float corner = 5;
};

typedef GUI_border_size GUIbs;

enum class DLLAPI_GUI GUIe_border{None = 0,
					Top = 1, Bottom = 2, Left = 4, Right = 8, 
					Corner_TR = 16, Corner_TL = 32, Corner_BR = 64,
					Corner_BL = 128};
typedef GUIe_border GUIb;

eSysCursor DLLAPI_GUI toCursor(GUIe_border border);

DECLARE_enum_class_bitwise_operators(DLLAPI_GUI, GUIb);

string DLLAPI_GUI toString(GUIe_border b);

enum class DLLAPI_GUI GUIe_alignment{None,Left,Right,Top,Bottom,Center};

typedef GUIe_alignment GUIa;

DECLARE_enum_class_bitwise_operators(DLLAPI_GUI, GUIe_alignment);

enum class DLLAPI_GUI GUIe_fileop{None,Open,Save,Folder};

typedef GUIe_fileop GUIf;
#endif