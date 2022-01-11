#ifndef GUILABEL_GUARD
#define GUILABEL_GUARD
#include "GUIbase.h"

enum alignmentKind{ALIGN_NONE,ALIGN_LEFT,ALIGN_RIGHT,ALIGN_TOP,ALIGN_BOTTOM,ALIGN_CENTER};

class GUIlabel:public virtual GUIbase{
	public:
	GUIlabel();
	vec3 textColor;
	font *textfont;
	string text;
	alignmentKind alignment_horizontal;
	alignmentKind alignment_vertical;
	bool const_height;	//1 - text height depends on font, 0 - text height depends on actual text
	GUIlabel *setTextColor(vec3 color);
	GUIlabel *setTextFont(font *f);
	GUIlabel *setText(string newtext);
	virtual GUIbase *sizeToContents();
	virtual void render();
};

vec2 getTextCentering(rect area, rect text,alignmentKind alignment_vertical, alignmentKind alignment_horizontal, bool const_height, font *F);

#endif
