#ifndef GUILABEL_GUARD
#define GUILABEL_GUARD
#include "util/globals_gui.h"
#include "render/render_gui.h"

class DLLAPI_GUI GUIlabel:public virtual GUIbase{
	public:
	GUIlabel();
	GUIlabel(string text);
	renderableText* rT;
	vec3 textColor;
	font *textfont;
	string text;
	GUIe_alignment alignment_horizontal;
	GUIe_alignment alignment_vertical;
	bool const_height;	
	vec2 textOffset; 
	GUIlabel *setTextColor(vec3 color);
	GUIlabel *setTextFont(font *f);
	GUIlabel *setText(string newtext);
	rect getTextRect(string text);
	rect getTextRect();
	virtual GUIbase *sizeToContents();
	virtual GUIpropertyTable getDefaultPropertyTable();
	virtual string getProperty(string key);
	virtual void setProperty(string key, string val);
	virtual string getType(); 
};

vec2 DLLAPI_GUI getTextCentering(rect area, rect text,
		GUIe_alignment alignment_vertical, 
		GUIe_alignment alignment_horizontal, 
		bool const_height, font *F);

#endif
