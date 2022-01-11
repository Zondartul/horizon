#ifndef GUIBUTTON_GUARD
#define GUIBUTTON_GUARD
#include "GUIframe.h"
#include "GUIimage.h"
#include "GUIlabel.h"

class GUIbutton:public virtual GUIframe, public virtual GUIimage, public virtual GUIlabel{
	public:
	GUIbutton();
	//vec3 textColor;
	vec3 hoverColor;
	vec3 pressedColor;
	//bool mouseover;
	bool pressed;
	float lastPressTime;
	//font *textfont;
	//string text;
	//GUIbutton *setTextColor(vec3 color);
	//GUIbutton *setHoverColor(vec3 color);
	//GUIbutton *setPressedColor(vec3 color);
	//GUIbutton *setTextFont(font *newfont);
	//GUIbutton *setText(string newtext);
	//texture *image;
	//GUIbutton *setImage(texture *newimage);
	function<void()> F;
	GUIbutton *setFunction(function<void()> f);
	virtual GUIbase *sizeToContents();
	virtual void render();
	virtual void onEvent(eventKind event);
};

#endif // GUIBUTTON_GUARD
