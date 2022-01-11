#include "GUIlabel.h"



//GUIlabel
GUIlabel::GUIlabel(){
	textColor = defaulttextColor;
	textfont = defaulttextfont;
	alignment_horizontal = ALIGN_CENTER;
	alignment_vertical = ALIGN_CENTER;
	text = defaulttext;
	const_height = false;
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
GUIbase *GUIlabel::sizeToContents(){
	rect size = preprintw(textfont,"%s",text.c_str());
	area = area.setSize(size.size);
	//printf("GUIlabel:new area = %s\n",toString(area).c_str());
	//popRenderOptions();
	return this;
}
void GUIlabel::render(){
	if(GUIoptions.push){pushRenderOptions();}
	setColor(textColor);
	setFont(textfont);
	//vec2 pos = area.start;
	//pos.y += area.size.y/2;
	rect tRect = preprintw(textfont,"%s",text.c_str());
	//setTextPos(thisToWorld(-tRect.start));
	vec2 tp = getTextCentering(worldArea(),tRect,alignment_vertical,alignment_horizontal,const_height,textfont);//-tRect.start;
	//setColor({0,0,255});
	//drawRectOutline(worldArea());
	//setColor({0,255,0});
	//drawRectOutline(tRect.moveBy(tp));
	setTextPos(vec2(tp.x,tp.y));
	printw("%s",text.c_str());
	if(GUIoptions.push){popRenderOptions();}
}




//area is arbitrary world rect in which the alignment happens
//rect text should be the rect returned by preprintw
vec2 getTextCentering(rect area, rect text,alignmentKind alignment_vertical, alignmentKind alignment_horizontal, bool const_height, font *F){
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
		case(ALIGN_LEFT):	x = axs-txs; break;
		case(ALIGN_CENTER):	x = axc-txc; break;
		case(ALIGN_RIGHT):	x = axe-txe; break;
	}

	switch(alignment_vertical){
		case(ALIGN_TOP):	y = ays-tys; break;
		case(ALIGN_CENTER):	y = ayc-tyc; break;
		case(ALIGN_BOTTOM):	y = aye-tye; break;
	}
	return (vec2){x,y};
	//return area.center()-text.center();//-text.topLeftCorner();
}

