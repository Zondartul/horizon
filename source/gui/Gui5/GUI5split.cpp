#include "gui/Gui5/GUI5split.h"
#include "display/paint.h"
#include "input/inputMessage.h"
#include "resource/cursor.h"

int GUI5split::ratioTotal(){
	int total = 0;
	for(int I = 0; I < ratio.size(); I++){
		total += ratio[I];
	}
	return total;
}
GUI5split::GUI5split(){
	vertical = false;
	fill = true;
	borderSize = 1;
	//colors["body"] = (color4i){255,255,255,64};
	colors["bar"] = (color4i){128,128,128,64};//(color4i){255,255,255,64};
	mouseOverBar = 0;
	dragging = false;
	//behaviors.clear();
	cursorSet = false;
}
GUI5split &GUI5split::setRatio(vector<int> newRatio){
	ratio = newRatio;
	invalidate();
	return *this;
}
GUI5split &GUI5split::setVertical(bool newVertical){
	vertical = newVertical;
	invalidate();
	return *this;
}
GUI5split &GUI5split::setBorderSize(int borderSize){
	this->borderSize = borderSize;
	invalidate();
	return *this;
}
void GUI5split::receiveMessageExtra(message *msg){
	if(msg->type == "mouse_move"){
		vec2i newPos = ((message_mouse_move*)msg)->newPos;
		//if(parent){newPos = area.getParent()->toWorld().clamp(newPos);}
		if(dragging){
			int total = ratioTotal();
			rect world = area.toWorld();
			int startx = world.getx();
			for(int I = 0; I < ratio.size()-1; I++){
				int addw = (int)(world.getw()*ratio[I]/(float)total);
				vec2i a = (vec2i){startx, world.gety()};
				vec2i d = (vec2i){startx+addw, world.gety2()};
				vec2i p1 = (vec2i){startx+borderSize+1, world.gety()+2*borderSize+1};
				vec2i p2 = (vec2i){startx+addw-borderSize-1, world.gety2()-2*borderSize-1};
				
				if(I == mouseOverBar-1){
					int oldRatioSum = ratio[I]+ratio[I+1];
					float pixPerUnitOfRatio = (float)world.getw()/(float)total;
					int deltaCursor = (newPos-a).x;
					int ratioNeeded = (float)deltaCursor/pixPerUnitOfRatio;
					//printf("os = %d, dc = %d, rn = %d, ppr = %f\n",oldRatioSum, deltaCursor, ratioNeeded, pixPerUnitOfRatio);
					ratioNeeded = clamp(ratioNeeded,1,oldRatioSum-1);
					ratio[I]   = ratioNeeded;
					ratio[I+1] = oldRatioSum-ratioNeeded;
					invalidate();
				}
				startx = startx+addw;
			}
		}else{
			if(hasAnyMouseover(newPos)){
				int total = ratioTotal();
				rect world = area.toWorld();
				int startx = world.getx();
				setCursor(getCursorHandle("arrow"));
				mouseOverBar = 0;
				for(int I = 0; I < ratio.size()-1; I++){
					int addw = (int)(world.getw()*ratio[I]/(float)total);
					vec2i a = (vec2i){startx, world.gety()};
					vec2i d = (vec2i){startx+addw, world.gety2()};
					vec2i p1 = (vec2i){startx+borderSize+1, world.gety()+2*borderSize+1};
					vec2i p2 = (vec2i){startx+addw-borderSize-1, world.gety2()-2*borderSize-1};
					startx = startx+addw;
					
					rect bar;
					bar.setStart((vec2i){p2.x,a.y}).setEnd((vec2i){p2.x+2*borderSize+2,d.y});
					if(bar.contains(newPos)){
						setCursor(getCursorHandle("sizewe")); //need paint-stack for cursors
						mouseOverBar = I+1;
						cursorSet = true;
					}
				}
			}
			if(!hasAnyMouseover(newPos) && !mouseOverBar && cursorSet){
				setCursor(getCursorHandle("arrow"));
			}
		}
	}
	if(msg->type == "lmb_down"){
		if(mouseOverBar){
			printf("yes\n");
			dragging = true;
		}
	}
	if(msg->type == "lmb_up"){
		printf("no\n");
		dragging = false;
	}
}
// render schematic A:
// a-------------------------+
// |                         |
// |           R3            |
// |     p1-------------+    |
// |     |              |    |
// |     |              |    |
// | R1  |              | R2 |
// |     |              |    |
// |     |              |    |
// |     +--------------p2   |
// |            R4           |
// |                         |
// +-------------------------d

// render schematic B:
// a-----+--------------+----+
// |     |              |    |
// |     |              |    |
// |     p1             |    |
// |     |              |    |
// |     |              |    |
// | R1  |              | R2 |
// |     |              |    |
// |     |              |    |
// |     |              p2   |
// |     |              |    |
// |     |              |    |
// +-----+--------------+----d
void GUI5split::render(){
	int total = ratioTotal();
	rect world = area.toWorld();
	int startx = world.getx();
	for(int I = 0; I < ratio.size(); I++){
		int addw = (int)(world.getw()*ratio[I]/(float)total);
		vec2i a = (vec2i){startx, world.gety()};
		vec2i d = (vec2i){startx+addw, world.gety2()};
		vec2i p1 = (vec2i){startx+borderSize+1, world.gety()+2*borderSize+1};
		vec2i p2 = (vec2i){startx+addw-borderSize-1, world.gety2()-2*borderSize-1};
		startx = startx+addw;
		if(I < ratio.size()-1){
			rect bar;
			bar.setStart((vec2i){p2.x,a.y}).setEnd((vec2i){p2.x+2*borderSize+2,d.y});
			setColorAlpha(colors["bar"]);
			paintRect(bar);
		}
		// setColorAlpha(colors["border"]);
		// paintRectOutline(p1.x,p1.y,p2.x,p2.y);
		// setColorAlpha(colors["body"]);
		// paintRect(a.x,a.y,p1.x,d.y);
		// paintRect(p2.x,a.y,d.x,d.y);
		// paintRect(a.x,a.y,d.x,p1.y);
		// paintRect(a.x,p2.y,d.x,d.y);
		setColorAlpha(colors["border"]);
		if(I != 0)				{paintLine(p1.x,a.y,p1.x,d.y);}
		if(I != ratio.size()-1)	{paintLine(p2.x,a.y,p2.x,d.y);}
	}
	if(children.size()){
		rect R = children[0]->area.toWorld();
		setColorAlpha((color4i){255,0,0,255});
		paintRectOutline(R);
	}
}
void GUI5split::layout(){
	//printf("layout\n");
	client_area = area;
	int total = ratioTotal();
	rect world = area.toWorld();
	int startx = world.getx();
	for(int I = 0; I < ratio.size() && I < children.size(); I++){
		int addw = (int)(world.getw()*ratio[I]/(float)total);
		vec2i a = (vec2i){startx, world.gety()};
		vec2i d = (vec2i){startx+addw, world.gety2()};
		vec2i p1 = (vec2i){startx+borderSize+3, world.gety()+2*borderSize+1};
		vec2i p2 = (vec2i){startx+addw-borderSize-3, world.gety2()-2*borderSize-1};
		startx = startx+addw;
		children[I]->area.setStart(area.fromWorld(p1)).setEnd(area.fromWorld(p2));
		children[I]->setHidden(false);
	}
	for(int I = ratio.size(); I < children.size(); I++){
		children[I]->setHidden(true);
	}
}