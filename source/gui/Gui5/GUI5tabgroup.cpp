#include "gui/Gui5/GUI5tabgroup.h"
#include "display/paint.h"
#include "resource/fonts.h"
#include "input/inputMessage.h"

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
	tabs[n].title = title;
	return *this;
}

void GUI5tabgroup::render(){
	//draw tabs
	for(int I = 0; I < children.size(); I++){
		tab &t = tabs[I];
		//setColor({0,0,0});
		//setAlpha(255);
		setColorAlpha(colors["border"]);
		paintLine(t.p1,t.p2);
		paintLine(t.p2,t.p3);
		paintLine(t.p3,t.p4);
		printw(t.p5.x,t.p5.y,-1,-1,tabs[I].title.c_str());
		//setColor({128,128,128});
		//setAlpha(64);
		setColorAlpha(colors["body"]);
		paintTriangle(t.p1,t.p2,t.p3);
		paintTriangle(t.p1,t.p3,t.p4);
	}
	//highlight the selected tab
	rect world = area.toWorld();
	vec2i pstart = world.getStart()+(vec2i){0,20};
	vec2i pend = world.getStart()+(vec2i){world.getw(),20};
	//setColor({0,0,0});
	//setAlpha(255);
	setColorAlpha(colors["border"]);
	paintLine(pstart, tabs[currentTab].p1);
	paintLine(tabs[currentTab].p4, pend);
	//paint an outline just because
	paintLine(tabs[children.size()-1].p4, world.getBottomRight());
	paintLine(world.getBottomRight(), world.getBottomLeft());
	paintLine(world.getBottomLeft(), tabs[0].p1);
	//setColor({128,128,128});
	//setAlpha(64);
	setColorAlpha(colors["body"]);
	paintRect(tabs[0].p1,world.getEnd());
}

void GUI5tabgroup::receiveMessageExtra(message *msg){
	if(msg->type == "lmb_down" && hasAnyMouseover(((message_mouse*)msg)->newPos)){
		vec2i pos = ((message_mouse*)msg)->newPos;
		for(int I = 0; I < children.size(); I++){
			rect tabrect;
			tabrect.setStart(tabs[I].p2);
			tabrect.setEnd(tabs[I].p3+(vec2i){0,20});
			if(tabrect.contains(pos)){
				currentTab = I;
				msg->handled = true;
				//hide every element except the one in the selected tab
				for(int I = 0; I < children.size(); I++){
					children[I]->setHidden(true);
				}
				children[currentTab]->setHidden(false);
			}
		}
	}
}

void GUI5tabgroup::layout(){
	if(parent){
		area.setStart({0,0});
		area.setSize(parent->client_area.getSize());
		client_area = area;
		client_area.setStart({0,20});
	}
	//check if current tab is still valid
	if(currentTab > children.size()-1){currentTab = children.size()-1;}
	//divide the space evenly into tabs
	int N = children.size();
	rect world = area.toWorld();
	int maxw = world.getw();
	for(int I = 0; I < N; I++){
		tab &t = tabs[I];
		t.p1 = world.getStart()+(vec2i){(maxw*I)/N,20};
		t.p2 = t.p1+(vec2i){10,-20};
		t.p4 = t.p1+(vec2i){maxw/N,0};
		t.p3 = t.p4+(vec2i){-10,-20};
		//printf("I: %d, N: %d\np1:(%d,%d), p2:(%d,%d)\np3:(%d,%d), p4:(%d,%d)\n\n",I,N,t.p1.x,t.p1.y,t.p2.x,t.p2.y,t.p3.x,t.p3.y,t.p4.x,t.p4.y);
		rect trect;
		preprintw(&trect,0,-1,-1,tabs[I].title.c_str());
		t.p5 = ((t.p1+t.p2+t.p3+t.p4)/4)-trect.getSize()/2;
	}
	//hide every element except the one in the selected tab
	for(int I = 0; I < N; I++){
		children[I]->setHidden(true);
	}
	children[currentTab]->setHidden(false);
}