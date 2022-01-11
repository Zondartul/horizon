#include "GUI_internal.h"



//GUIlabel
GUIlabel::GUIlabel(){
	textColor = defaulttextColor;
	textfont = defaulttextfont;
	alignment_horizontal = GUIa::Center;
	alignment_vertical = GUIa::Center;
	text = defaulttext;
	const_height = false;
	textOffset = vec2(0,0);
}
GUIlabel::GUIlabel(string text):GUIlabel(){
	setText(text);
}
GUIlabel *GUIlabel::setTextColor(vec3 color){
	textColor = color;
	return this;
}
GUIlabel *GUIlabel::setTextFont(font *f){
	textfont = f;
	return this;
}
GUIlabel *GUIlabel::setText(string newtext){
	text = newtext;
	return this;
}

rect GUIlabel::getTextRect(string text){
	rect size = preprintw(textfont,"%s",text.c_str());
	return size;
}

rect GUIlabel::getTextRect(){
	return getTextRect(text);
}

GUIbase *GUIlabel::sizeToContents(){
	rect size = getTextRect();
	area = area.setSize(size.size);
	//printf("GUIlabel:new area = %s\n",toString(area).c_str());
	//popRenderOptions();
	return this;
}

/*
void GUIlabel::render(){
	if(GUIoptions.push){pushRenderOptions();}
	setColor(textColor);
	setFont(textfont);
	//vec2 pos = area.start;
	//pos.y += area.size.y/2;
	rect tRect = getTextRect();//preprintw(textfont,"%s",text.c_str());
	//setTextPos(thisToWorld(-tRect.start));
	vec2 tp = getTextCentering(worldArea(),tRect,alignment_vertical,alignment_horizontal,const_height,textfont);//-tRect.start;
	//setColor({0,0,255});
	//drawRectOutline(worldArea());
	//setColor({0,255,0});
	//drawRectOutline(tRect.moveBy(tp));
	setTextPos(vec2(tp.x,tp.y)+textOffset);
	printw("%s",text.c_str());
	if(GUIoptions.push){popRenderOptions();}
}
*/



//area is arbitrary world rect in which the alignment happens
//rect text should be the rect returned by preprintw
vec2 getTextCentering(rect area, rect text,
	GUIe_alignment alignment_vertical, 
	GUIe_alignment alignment_horizontal, 
	bool const_height, font *F){
	rect R;
	int yborder = 2; //if it was 0, letters would be printed over the actual border-border.
	if(const_height){
		//printf("===========\ntext: %s\nmaxrect: %s\n",toString(text).c_str(),toString(F->maxrect).c_str());
		//calculation fucks up when start position changes. Maybe preprint doesn't include vertical bearing.
		//bool cond = text.size.y < F->maxrect.end.y;
		//if(cond){text = text.setEnd(text.start+(vec2){text.size.x,F->maxrect.end.y});}
		//if(cond){text = text.setSize({text.size.x,F->maxrect.end.y});}
		//printf("cond: %d\ntext2: %s\n",cond,toString(text).c_str());
		text = text.setEnd({text.size.x,F->maxrect.size.y+yborder});
	}
	float axs = area.start.x;
	float axc = area.center().x;
	float axe = area.end.x;
	float txs = text.start.x;
	float txc = text.center().x;
	float txe = text.end.x;

	float ays = area.start.y;
	float ayc = area.center().y;
	float aye = area.end.y;
	float tys = text.start.y;
	float tyc = text.center().y;
	float tye = text.end.y;


	float x,y;

	switch(alignment_horizontal){
		case(GUIa::Left):	x = axs-txs; break;
		case(GUIa::Center):	x = axc-txc; break;
		case(GUIa::Right):	x = axe-txe; break;
	}

	switch(alignment_vertical){
		case(GUIa::Top):	y = ays-tys; break;
		case(GUIa::Center):	y = ayc-tyc; break;
		case(GUIa::Bottom):	y = aye-tye; break;
	}
	return vec2{x,y};
	//return area.center()-text.center();//-text.topLeftCorner();
}

//property table
GUIpropertyTable GUIlabel::getDefaultPropertyTable(){
	GUIpropertyTable table = GUIbase::getDefaultPropertyTable();
	table.table["textColor"] = toString(vec3(defaulttextColor));
	table.table["textfont"] = toString(defaulttextfont);
	table.table["text"] = toString(defaulttext);
	table.table["alignment_horizontal"] = toString(GUIa::Center);
	table.table["alignment_vertical"] = toString(GUIa::Center);
	table.table["const_height"] = toString((bool)false);
	
	return table;
}
string GUIlabel::getProperty(string key){
	if(key == "textColor"){return toString(textColor);}
	else if(key == "textfont"){return toString(textfont);}
	else if(key == "text"){return toString(text);}
	else if(key == "alignment_horizontal"){return toString(alignment_horizontal);}
	else if(key == "alignment_vertical"){return toString(alignment_vertical);}
	else if(key == "const_hegiht"){return toString(const_height);}
	else return GUIbase::getProperty(key);
}
void GUIlabel::setProperty(string key, string val){
	//printf("%s::setProperty(%s)=[%s]\n",getType().c_str(),key.c_str(),val.c_str());

	if(key == "textColor"){setTextColor(fromString<vec3>(val));}
	else if(key == "textfont"){setTextFont(fromString<font*>(val));}
	else if(key == "text"){setText(val);}
	else if(key == "alignment_horizontal"){alignment_horizontal = fromString<GUIe_alignment>(val);}
	else if(key == "alignment_vertical"){alignment_vertical = fromString<GUIe_alignment>(val);}
	else if(key == "const_height"){const_height = fromString<bool>(val);}
	else GUIbase::setProperty(key,val);
}
	

string GUIlabel::getType(){return "GUIlabel";}
	