#include "gui/Gui5/GUI5constraint.h"

GUI5constraint::GUI5constraint(){
	self = 0;
	dependencyFlags = 0;
}
void GUI5constraint::invalidate(){}

//sizes the element according to contents
GUI5constraintSizeToContents::GUI5constraintSizeToContents(GUI5base *self){
	this->self = self;
	dependencyFlags = getsChildrenSize | getsChildrenPos | setsOwnSize;
}
void GUI5constraintSizeToContents::invalidate(){
	//1) calculate the bounding box of all children
	int minx = 0;
	int miny = 0;
	int maxx = minx;
	int maxy = miny;
	vector<GUI5base*> &children = self->children;
	for(int I = 0; I < children.size(); I++){
		GUI5base &child = *(children[I]);
		maxx = max(maxx, child.area.getx2());
		maxy = max(maxy, child.area.gety2());
	}
	//2) set own size to the size of that bounding box
	self->area.setx2(maxx).sety2(maxy);
}

//sizes the element according to parent size
GUI5constraintPercentParentSize::GUI5constraintPercentParentSize(GUI5base *self){
	this->self = self;
	dependencyFlags = getsParentSize | setsOwnSize;
}
void GUI5constraintPercentParentSize::invalidate(){
	int newx = self->parent->area.getw() * ratiox + addx;
	int newy = self->parent->area.geth() * ratioy + addy;
	self->area.setx2(newx).sety2(newy);
}

//lines up children in horizontal rows
GUI5constraintHorizontalList::GUI5constraintHorizontalList(GUI5base *self){
	this->self = self;
	dependencyFlags = getsChildrenSize | getsOwnSize | setsChildrenPos;
}
void GUI5constraintHorizontalList::invalidate(){
	//vertical size may be different for each row:
	//attempt placing a row to calculate size, then place it for real.
	int cury = 0;
	int rowstart = 0;
	vector<GUI5base*> &children = self->children;
	while(rowstart < children.size()){
		//1) calculate the vertical size of a row by first placing all the children
		//on a single row unaligned.
		int maxy = 0;
		int curx = 0;
		for(int I = rowstart; I < children.size(); I++){
			GUI5base &child = *(children[I]);
			maxy = max(maxy, child.area.geth());
			child.area.setx(curx).sety(cury);
			curx = curx+child.area.getw();
			//a row will always contain at least one element.
			//if wrap is on, stop at the edge.
			if(wrap && (curx > self->area.getw())){break;}
		}
		//2)place the row for real.
		curx = 0;
		for(int I = rowstart; I < children.size(); I++){
			GUI5base &child = *(children[I]);
			int newy = 0;
			switch(verticalAlignment){
				case 0://top
					newy = cury;
				break;
				case 1://center
					newy = cury+(maxy-child.area.geth())/2;
				break;
				case 2://bottom
					newy = cury+(maxy-child.area.geth());
				break;
			}
			child.area.setx(curx).sety(newy);
			curx = curx+child.area.getw();
			rowstart = I;
			//a row will always contain at least one element.
			//if wrap is on, stop at the edge.
			if(wrap && (curx > self->area.getw())){break;}
		}
	//if the row ended but there are still elements to place, repeat the procedure.
	}
}

//lines up children in vertical columns
GUI5constraintVerticalList::GUI5constraintVerticalList(GUI5base *self){
	this->self = self;
	dependencyFlags = getsChildrenSize | setsChildrenPos;
}
void GUI5constraintVerticalList::invalidate(){
	//horizontal size may be different for each column:
	//attempt placing a column to calculate size, then place it for real.
	int curx = 0;
	int columnstart = 0;
	vector<GUI5base*> &children = self->children;
	while(columnstart < children.size()){
		//1) calculate the horizontal size of a column by first placing all the children
		//on a single column unaligned.
		int maxx = 0;
		int cury = 0;
		for(int I = columnstart; I < children.size(); I++){
			GUI5base &child = *(children[I]);
			maxx = max(maxx, child.area.getw());
			child.area.sety(cury).setx(curx);
			cury = cury+child.area.geth();
			//a column will always contain at least one element.
			//if wrap is on, stop at the edge.
			if(wrap && (cury > self->area.geth())){break;}
		}
		//2)place the column for real.
		cury = 0;
		for(int I = columnstart; I < children.size(); I++){
			GUI5base &child = *(children[I]);
			int newx = 0;
			switch(horizontalAlignment){
				case 0://left
					newx = curx;
				break;
				case 1://center
					newx = curx+(maxx-child.area.getw())/2;
				break;
				case 2://right
					newx = curx+(maxx-child.area.getw());
				break;
			}
			child.area.sety(cury).setx(newx);
			cury = cury+child.area.getw();
			columnstart = I;
			//a column will always contain at least one element.
			//if wrap is on, stop at the edge.
			if(wrap && (cury > self->area.geth())){break;}
		}
	//if the column ended but there are still elements to place, repeat the procedure.
	}
}

//aligns the child top-middle-bottom and left-middle-right.
GUI5constraintAlignment::GUI5constraintAlignment(GUI5base *self){
	this->self = self;
	dependencyFlags = getsOwnSize | getsChildrenSize | setsChildrenPos;
}
void GUI5constraintAlignment::invalidate(){
	GUI5base &child = *(self->children[0]);
	int newx = 0;
	int newy = 0;
	switch(horizontalAlignment){
		case 0:	//left
			newx = 0;
		break;
		case 1:	//center
			newx = (self->area.getw()-child.area.getw())/2;
		break;
		case 2:	//right
			newx = (self->area.getw()-child.area.getw());
		break;
	}
	switch(verticalAlignment){
		case 0:	//top
			newy = 0;
		break;
		case 1:	//center
			newy = (self->area.geth()-child.area.geth())/2;
		break;
		case 2:	//bottom
			newy = (self->area.geth()-child.area.geth());
		break;
	}
	child.area.setx(newx).sety(newy);
}
//todo: get these things to use client area instead.










