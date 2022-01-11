#include "gui/Gui5/GUI5scrollbar.h"
#include "gui/Gui5/GUI5button.h"
#include "gui/Gui5/GUI5frame.h"
#include "display/paint.h"

void scrollBtnUp(void *arg){GUI5scrollbar *scroll = (GUI5scrollbar *)arg; scroll->children[4]->area.moveBy({0,-scroll->children[4]->area.geth()/2}); scroll->layout();}
void scrollBtnDown(void *arg){GUI5scrollbar *scroll = (GUI5scrollbar *)arg; scroll->children[4]->area.moveBy({0,scroll->children[4]->area.geth()/2}); scroll->layout();}
void scrollBtnLeft(void *arg){GUI5scrollbar *scroll = (GUI5scrollbar *)arg; scroll->children[5]->area.moveBy({-scroll->children[5]->area.getw()/2,0}); scroll->layout();}
void scrollBtnRight(void *arg){GUI5scrollbar *scroll = (GUI5scrollbar *)arg; scroll->children[5]->area.moveBy({scroll->children[5]->area.getw()/2,0}); scroll->layout();}

GUI5scrollbar::GUI5scrollbar(){
	printf("constructing scrollbar\n");
	//area.setSize({100,100});
	client_area.setSize({1,1});
	vertical = true;
	horizontal = true;
	fill = true;
	min = (vec2i){0,0};
	max = (vec2i){1,1};
	//offsetRect.setStart({0,0}).setEnd({10,10}).setParent(&client_area);
	prevOffset = (vec2i){0,0};
	addElement((*(new GUI5button())).setFunc(&scrollBtnUp).setArg((void*)this).setText("^").setSize({20,20}).setClient(false));
	addElement((*(new GUI5button())).setFunc(&scrollBtnDown).setArg((void*)this).setText("v").setSize({20,20}).setClient(false));
	addElement((*(new GUI5button())).setFunc(&scrollBtnLeft).setArg((void*)this).setText("<").setSize({20,20}).setClient(false));
	addElement((*(new GUI5button())).setFunc(&scrollBtnRight).setArg((void*)this).setText(">").setSize({20,20}).setClient(false));
	addElement((*(new GUI5frame())).setClient(false));
	addElement((*(new GUI5frame())).setClient(false));
	printf("done constructing scrollbar\n");
}
GUI5scrollbar &GUI5scrollbar::setVertical(bool v){vertical = v; invalidate(); return *this;}
GUI5scrollbar &GUI5scrollbar::setHorizontal(bool h){horizontal = h; invalidate(); return *this;}
GUI5scrollbar &GUI5scrollbar::setVerticalLimits(int start, int end){min.y = start; max.y = end; invalidate(); return *this;}
GUI5scrollbar &GUI5scrollbar::setHorizontalLimits(int start, int end){min.x = start; max.x = end; invalidate(); return *this;}
GUI5scrollbar &GUI5scrollbar::setVerticalPos(int pos){invalidate(); return *this;} //todo
GUI5scrollbar &GUI5scrollbar::setHorizontalPos(int pos){invalidate(); return *this;} //todo
GUI5scrollbar &GUI5scrollbar::setAutoHide(bool hide){autoHide = hide; return *this;}
// RENDER DIAGRAM
//
//					                a1--b1
//					                | t1|
//					                c1--d1
//					                |   |
//					                |	|
//					                a3--b3
//					                |///|
//					                |///|
//					                |///|
//					                |///|
//					                c3--d3
//					                |   |
//					                |	|
//					                a2--b2
//					                | t2|
//					                c2--d2
//e1--f1----e3----------f3----e2--f2
//| t3|     |///////////|     | t4|
//g1--h1----g3----------h3----g2--h2
//


void GUI5scrollbar::render(){
/* 	vec2i a1,b1,c1,d1,e1,f1,g1,t1;
	vec2i a2,b2,c2,d2,e2,f2,g2,t2;
	vec2i a3,b3,c3,d3,e3,f3,g3,t3;
	
	b1 = (vec2i){area.getw(), 0};
	a1 = b1 + (vec2i){-20,0};
	c1 = b1 + (vec2i){-20,20};
	d1 = b1 + (vec2i){0,20};
	t1 = (a1+b1+c1+d1)/4;
	
	b2 = (vec2i){area.getw(), area.geth()-40};
	a2 = b2 + (vec2i){-20,0};
	c2 = b2 + (vec2i){-20,20};
	d2 = b2 + (vec2i){0,20};
	t2 = (a2+b2+c2+d2)/4;
	
	g1 = (vec2i){0, area.geth()};
	e1 = g1 + (vec2i){0,-20};
	f1 = g1 + (vec2i){20,-20};
	h1 = g1 + (vec2i){20,0};
	t3 = (e1+f1+g1+h1)/4;
	
	g2 = (vec2i){0, area.geth()};
	e2 = g2 + (vec2i){0,-20};
	f2 = g2 + (vec2i){20,-20};
	h2 = g2 + (vec2i){20,0};
	t4 = (e2+f2+g2+h2)/4; */
}
void GUI5scrollbar::debugrender(){
	if(children.size() < 6){return;}
	tempPaintSettings t;
	t.setScissorEnabled(false);
	t.setColor({255,0,0});
	t.setRect(children[4]->area.toWorld());
	paintRectOutline();
	t.setRect(children[5]->area.toWorld());
	paintRectOutline();
	t.setP1(area.toWorld().getCenter());
	t.setP2(children[4]->area.toWorld().getCenter());
	paintLine();
	t.setP2(children[5]->area.toWorld().getCenter());
	paintLine();
	
	//compute the size of the slidey
	vec2i slideyTrackSize;
	slideyTrackSize.x = area.getw()-60;
	slideyTrackSize.y = area.geth()-60;
	vec2i availAreaSize = max-min;
	vec2i slideySize;
	slideySize.y = slideyTrackSize.y*clamp((float)area.geth()/availAreaSize.y,0,1);
	slideySize.x = slideyTrackSize.x*clamp((float)area.getw()/availAreaSize.x,0,1);
	t.setP1({0,0});
	t.setColor({0,128,0});
	t.setP2(slideyTrackSize);
	paintRectOutline();
	t.setColor({0,255,0});
	t.setP2(slideySize);
	paintRectOutline();
	//constrain the position of the slideys
	rect YslideyZone;
	YslideyZone.setStart({area.getw()-20,20}).setEnd({area.getw(),area.geth()-40});
	t.setColor({0,0,255});
	t.setRect(YslideyZone);
	paintRectOutline();
	
	rect XslideyZone;
	XslideyZone.setStart({20,area.geth()-20}).setEnd({area.getw()-40, area.geth()});
	t.setColor({0,0,255});
	t.setRect(XslideyZone);
	paintRectOutline();
	
	//compute overal scroll offset from slidey positions
	//(center of slidey - center of track) / size of track
	vec2i offset;
	offset.y = (children[4]->area.getCenter().y - YslideyZone.getCenter().y)/YslideyZone.geth();
	offset.x = (children[5]->area.getCenter().x - XslideyZone.getCenter().x)/XslideyZone.getw();
	t.setColor({0,0,255});
	t.setP1(area.toWorld().getCenter());
	t.setP2(area.toWorld().getCenter()+offset);
	paintLine();
}
void GUI5scrollbar::layout(){
	vec2i availAreaSize = max-min;
	bool noYtravel;
	bool noXtravel;
	if(autoHide){
		//-----------------------------------------
		//                            |      |
		//                            |      |
		//                            |      |
		//                            |      |
		//          !h !v             |!h !v | h !v
		//                            |      |
		//                            |      |
		//                            |      |
		//----------------------------20-----------
		//          !h !v             | h  v | h v
		//---------------------------------area----
		//          !h  v             | h  v | h v
		//                            |      |
		horizontal = availAreaSize.x > area.getw() || (availAreaSize.x > area.getw()-20 && availAreaSize.y > area.geth()-20);
		vertical = availAreaSize.y > area.geth() || (availAreaSize.y > area.geth()-20 && availAreaSize.x > area.getw()-20);
		noYtravel = !vertical;
		noXtravel = !horizontal;
	}else{
		noYtravel = area.geth()-20*horizontal >= availAreaSize.y;
		noXtravel = area.getw()-20*vertical >= availAreaSize.x;
	}
	client_area.setStart({0,0});
	client_area.setSize({area.getw()-20*vertical, area.geth()-20*horizontal});
	
	//have all the elements been created yet?
	if(children.size() < 6){return;}
	children[0]->area.moveStart({area.getw()-20,0});
	children[1]->area.moveStart({area.getw()-20,area.geth()-20-20*horizontal});
	children[2]->area.moveStart({0,area.geth()-20});
	children[3]->area.moveStart({area.getw()-20-20*vertical,area.geth()-20});
	
	children[0]->setHidden(!vertical);
	children[1]->setHidden(!vertical);
	children[2]->setHidden(!horizontal);
	children[3]->setHidden(!horizontal);
	children[4]->setHidden(!vertical);
	children[5]->setHidden(!horizontal);
	//compute the size of the slidey
	vec2i slideyTrackSize;
	slideyTrackSize.x = area.getw()-40-20*vertical;
	slideyTrackSize.y = area.geth()-40-20*horizontal;
	vec2i slideySize;
	slideySize.y = slideyTrackSize.y*clamp((float)area.geth()/(float)availAreaSize.y,0,1);
	slideySize.x = slideyTrackSize.x*clamp((float)area.getw()/(float)availAreaSize.x,0,1);
	//printf("slideySize: %d, %d\n", slideySize.x, slideySize.y);
	//constrain the position of the slideys
	rect YslideyZone;
	YslideyZone.setStart({area.getw()-20,20}).setEnd({area.getw(),area.geth()-20-20*horizontal});
	children[4]->area.setSize({20, slideySize.y});
	children[4]->area.moveEnd(YslideyZone.clamp(children[4]->area.getEnd()));
	children[4]->area.moveStart(YslideyZone.clamp(children[4]->area.getStart()));
	
	rect XslideyZone;
	XslideyZone.setStart({20,area.geth()-20}).setEnd({area.getw()-20-20*vertical, area.geth()});
	children[5]->area.setSize({slideySize.x, 20});
	children[5]->area.moveEnd(XslideyZone.clamp(children[5]->area.getEnd()));
	children[5]->area.moveStart(XslideyZone.clamp(children[5]->area.getStart()));
	//compute overal scroll offset from slidey positions
	//scroll ratio = scroll offset / scroll travel
	//offset = max size * scroll ratio
	//or something
	vec2i offset;
	if(noYtravel){offset.y = 0;}
	else{offset.y = availAreaSize.y*(0.5+(float)(children[4]->area.getCenter().y - YslideyZone.getCenter().y)/(float)(YslideyZone.geth()-children[4]->area.geth()));}
	
	if(noXtravel){offset.x = 0;}
	else{offset.x = availAreaSize.x*(0.5+(float)(children[5]->area.getCenter().x - XslideyZone.getCenter().x)/(float)(XslideyZone.getw()-children[5]->area.getw()));}
	//do actual layout now
	//offsetRect.setStart(offset);
	for(int I = 0; I < children.size(); I++){
		if(children[I]->isClient){
			//children[I]->area.setParent(&offsetRect);
			children[I]->area.moveBy(-(offset-prevOffset));
		}
	}
	prevOffset = offset;
}
void GUI5scrollbar::receiveMessageExtra(message *msg){}








