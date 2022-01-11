#include "gui/Gui5/GUI5tabgroup.h"
#include "display/paint.h"
#include "resource/fonts.h"

GUI5tabgroup::GUI5tabgroup(){
	currentTab = 0;
}

// fancy render schematic
//       p2--------p3
//      /  (p5)Tab  \
// A--p1             p4---------B 
// |                            |
// | ~actual element goes here~ |
// |                            |
// D----------------------------C

GUI5tabgroup &GUI5tabgroup::setTitle(int n, string title){
	titles[n] = title;
	return *this;
}

void GUI5tabgroup::render(){
	//divide the space evenly into tabs
	int N = children.size();
	rect world = area.toWorld();
	int maxx = world.getx2();
	for(int I = 0; I < N; I++){
		vec2i p1 = world.getStart()+(vec2i){I/N,20};
		vec2i p2 = p1+(vec2i){10,-20};
		vec2i p4 = p1+(vec2i){maxx*(I+1)/N,0};
		vec2i p3 = p4+(vec2i){-10,-20};
		setColor({0,0,0});
		setAlpha(255);
		paintLine(p1,p2);
		paintLine(p2,p3);
		paintLine(p3,p4);
		rect trect;
		preprintw(&trect,0,-1,-1,titles[I].c_str());
		vec2i p5 = ((p1+p2+p3+p4)/4)-trect.getSize()/2;
		printw(p5.x,p5.y,-1,-1,titles[I].c_str());
	}
}

void GUI5tabgroup::layout(){
	if(parent){
		area.setStart({0,0});
		area.setSize(parent->client_area.getSize());
		client_area = area;
		client_area.setStart({0,20});
	}
}