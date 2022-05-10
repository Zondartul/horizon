#ifndef GUI_TYPES_GUARD
#define GUI_TYPES_GUARD
#include "util.h"
#include "vec.h"
struct font;
struct texture;
struct GUI_border_rects{
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
struct GUI_border_size{
	float top = 3;
	float bottom = 3;
	float left = 3;
	float right = 3;
	float corner = 5;
};
typedef GUI_border_size GUIbs;
enum class GUIe_border{None = 0, 
					Top = 1, Bottom = 2, Left = 4, Right = 8, 
					Corner_TR = 16, Corner_TL = 32, Corner_BR = 64,
					Corner_BL = 128};
typedef GUIe_border GUIb;
DECLARE_enum_class_bitwise_operators(GUIb);
string toString(GUIe_border b);
enum class GUIe_alignment{None,Left,Right,Top,Bottom,Center};
typedef GUIe_alignment GUIa;
DECLARE_enum_class_bitwise_operators(GUIe_alignment);
enum class GUIe_fileop{None,Open,Save,Folder};
typedef GUIe_fileop GUIf;
#endif