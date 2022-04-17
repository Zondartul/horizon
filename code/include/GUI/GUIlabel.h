#ifndef GUILABEL_GUARD
#define GUILABEL_GUARD

class GUIlabel:public virtual GUIbase{
	public:
	GUIlabel();
	GUIlabel(string text);
	//properties
	vec3 textColor;
	font *textfont;
	string text;
	GUIe_alignment alignment_horizontal;
	GUIe_alignment alignment_vertical;
	bool const_height;	//1 - text height depends on font, 0 - text height depends on actual text
	vec2 textOffset; //text will be printed at an offset
	
	GUIlabel *setTextColor(vec3 color);
	GUIlabel *setTextFont(font *f);
	GUIlabel *setText(string newtext);
	rect getTextRect(string text);
	rect getTextRect();
	virtual GUIbase *sizeToContents();
	
	//property table
	virtual GUIpropertyTable getDefaultPropertyTable();
	virtual string getProperty(string key);
	virtual void setProperty(string key, string val);

	virtual string getType(); //returns the most-derived-class of the widget.
};

vec2 getTextCentering(rect area, rect text,
		GUIe_alignment alignment_vertical, 
		GUIe_alignment alignment_horizontal, 
		bool const_height, font *F);

#endif
