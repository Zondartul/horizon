#include "GUI.h"
#include "paint.h"
#include "simplemath.h"
#include "globals.h"
//#include "selfaware.h"
#include "window.h"
#include "stringUtils.h"
#include "globals.h"
#include "resource.h"
#include "texture.h"
#include "mouse.h"
#include "renderLayer.h"
#include "frameprinter.h"
#include <cmath>
using std::isnan;

#define defaultarea rect(100,100)
#define defaultclientarea rect(100,100)
#define defaultbgcolor (vec3){128,196,255}
#define defaultborderColor (vec3){100,128,196}
#define defaulttextColor (vec3){0,0,0}
#define defaulttextfont getFont("calibri 18")
#define defaulttext "text"
#define defaultimage 0
//#define defaultimage getTexture("resource/party.jpg")
extern renderLayer *layerGUI;
//utility functions for GUI
GUIsetFavoriteRenderOptions(){
	setLayer(layerGUI);
	layerGUI->clear();
	setColoring(false);
	setTexturing(false);
	setColor(defaultbgcolor);
	setAlpha(255);
	//firstn(50,printf("setAlpha\n"));
	//clearScissor();
	setScissoring(false);
	//setThickness(1);
	//setTextMaxX(-1);
	//setTextMaxY(-1);
	setTextPos({0,0});
	//setInfiniteLine(false);
	//setInfinitePlane(false);
	setFont(defaulttextfont);
	//setTextScale(1);
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

//GUIbase
GUIbase::GUIbase(){
	parent = NULL;
	isClient = true;
	hidden = false;
	suppressInvalidate = false;
	area = defaultarea;
	clientArea = defaultclientarea;
}

GUIbase::~GUIbase(){
	if(parent){parent->removeChild(this);}
}

GUIbase *GUIbase::addChild(GUIbase *child){
	child->parent = this;
	children.push_back(child); //somehow, adding them to the front screws everything up
	channel.addListenerFront(child);
	invalidateTree();
	//invalidate();
	//child->invalidate();
	return this;
}

void GUIbase_debugChildren(GUIbase *b){
	if(!b){error("'this' is null");}
	printf("printing %p's %d children:\n",b,b->children.size());
	if(!b->children.size()){printf("%p has no children\n");return;}
	int i = 0;
	for(auto I = b->children.begin(); I != b->children.end(); I++){
		GUIbase *b2 = *I;
		string type = typeid(*b2).name();
		printf("%d: %p (%s)\n",i,*I,type.c_str());
		i++;
	}
}

GUIbase *GUIbase::removeChild(GUIbase *child){
	//printf("removeChild called\n");
	//GUIbase_debugChildren(this);
	bool found = false;
	int i = 0;
	for(auto I = children.begin(); I != children.end();){ //no I++ here
		if(*I == child){
			//printf("erased %p, %d\n",*I,i++);
			channel.removeListener(child);
			I = children.erase(I);
			found = true;
		}else{
			I++;
		}
	}
	if(!found){printf("couldn't erase %p: not found\n",child);}
	invalidateTree();
	//printf("invalidated\n");
}

GUIbase *GUIbase::setSize(vec2 newSize){
	area = area.setSize(newSize);
	invalidate();
	return this;
}

GUIbase *GUIbase::moveTo(vec2 newstart){
	area = area.moveTo(newstart);
	//printf("moveTo: new area = %s\n",toString(area).c_str());
	invalidate();
	return this;
}

GUIbase *GUIbase::setClientArea(rect newclientarea){
	clientArea = newclientarea;
	return this;
}

GUIbase *GUIbase::setHidden(bool newHidden){
	hidden = newHidden;
	return this;
}

GUIbase *GUIbase::sizeToContents(){
	//printf("sizeToContents()\n");
	//printf("stc area1: %s\n",toString(area).c_str());
	//printf("stc cli1:  %s\n",toString(clientArea).c_str());
	float maxx = 0;
	float maxy = 0;
	for(int I = 0; I < children.size(); I++){
		//printf("ch 1: %s\n",toString(children[I]->area).c_str());
		maxx = max(maxx, children[I]->area.end.x);
		maxy = max(maxy, children[I]->area.end.y);
		//printf("maxx = %f\n",maxx);
		//printf("maxy = %f\n",maxy);
	}
	vec2 newsize = {maxx,maxy};
	vec2 diff = newsize - clientArea.size;
	clientArea = clientArea.setSize(newsize);
	area = area.setSize(area.size+diff);
	//printf("stc area2: %s\n",toString(area).c_str());
	//printf("stc cli2:  %s\n",toString(clientArea).c_str());
	return this;
}

void GUIbase::renderLogic(){
	if(!parent){GUIsetFavoriteRenderOptions();}
	for(int I = 0; I < children.size(); I++){
		if(!children[I]->isClient && !children[I]->hidden){
			//pushRenderOptions();
			//drawRectOutlineColored(children[I]->visibleArea(),{255,0,0});
			setScissoring(true);
			setScissor(children[I]->visibleArea());
			children[I]->render();		
			children[I]->renderLogic();
			//popRenderOptions();
		}
	}
	//cropScissor(worldClientArea());
	for(int I = 0; I < children.size(); I++){
		if(children[I]->isClient && !children[I]->hidden){
			//pushRenderOptions();
			//drawRectOutlineColored(children[I]->visibleArea(),{255,0,0});
			setScissoring(true);
			setScissor(children[I]->visibleArea());
			children[I]->render();		
			children[I]->renderLogic();
			//popRenderOptions();
		}
	}
	//popRenderOptions();
}

void GUIbase::render(){}

void GUIbase::onEvent(eventKind event){
	// for(int I = 0; I < children.size(); I++){
		// if(!children[I]->hidden){
			// children[I]->onEvent(event);
		// }
	// }
	if(event.type == EVENT_MOUSE_MOVE){		
		recalcMouseover();	//this check needs to be already 
							//done by the time children start handling events
	}
	event.maskEvent(channel.publishEventParallelMaskable(event));
}

GUIbase *GUIbase::root(){
	if(parent){
		GUIbase *p = parent;
		while(p->parent){p = p->parent;}
		return p;
	}else{
		return this;
	}
}

void GUIbase::invalidate(){
	//printf("invalidate %p (%s)\n",this,typeid(this).name());
	clientArea = rect().setStart({1,1}).setEnd(area.size-(vec2){1,1});//rect(newarea.size);
}

void GUIbase::invalidateTree(){
	if(suppressInvalidate){return;}
	//set invalidateSupress and invalidate children (inv downwards)
	//btw, when an external change is done to an element, it probably supresses invalidations and invalidates upwards and downwards...
	//or invalidates from top.
	if(parent){
		root()->invalidateTree();
	}else{
		invalidateDown();
	}
}

void GUIbase::invalidateDown(){
	invalidate();
	bool prevsuppress = suppressInvalidate;
	suppressInvalidate = true;
	for(auto I = children.begin(); I != children.end(); I++){
		(*I)->invalidateDown();
	}
	suppressInvalidate = prevsuppress;
}

//in the future, this could be used to classify mouse 
//as "over border/title/other parts"
void GUIbase::recalcMouseover(){
	vec2 pos = getMousePos();
	//not mouseover if mouse is somewhere else entirely
	if(!visibleArea().contains(pos)){mouseover = false; return;}
	if(parent){
		//not mouseover if parent doesn't have mouseover
		if(!parent->mouseover){mouseover = false; return;}
		//not mouseover if any of higher siblings have mouseover
		auto I = parent->children.begin();
		while(*I != this){I++;}
		I++;
		for(;I != parent->children.end(); I++){
			if((*I)->mouseover){mouseover = false; return;}
		}
	}
	mouseover = true;
}

//if something has coordinates L within
//the Area of this element,
//it was coordinates W in the world.
vec2 GUIbase::thisToWorld(vec2 L){
	if(parent){
		if(isClient){
			return (parent->clientToWorld(L))+area.start;
		}else{
			return (parent->thisToWorld(L))+area.start;
		}
	}else{
		return L+area.start;
	}
}

rect GUIbase::thisToWorld(rect L){
	return L.setStart(thisToWorld(L.start)).setEnd(thisToWorld(L.end));
}

//if something has coordinates L within
//the clientArea of this element,
//it was coordinates W in the world.
vec2 GUIbase::clientToWorld(vec2 L){
	if(parent){
		if(isClient){
			return (parent->clientToWorld(L))+area.start+clientArea.start;
		}else{
			return (parent->thisToWorld(L))+area.start+clientArea.start;
		}
	}else{
		return L+area.start+clientArea.start;
	}
}

rect GUIbase::clientToWorld(rect L){
	return L.setStart(clientToWorld(L.start)).setEnd(clientToWorld(L.end));
}

rect GUIbase::worldArea(){
	if(parent){
		return thisToWorld(rect(area.size));
	}else{
		vec2 scr = getScreenSize();
		return rect(scr);
	}
}
rect GUIbase::worldClientArea(){
	if(parent){
		return thisToWorld(clientArea);
	}else{
		vec2 scr = getScreenSize();
		return rect(scr);
	}
}
rect GUIbase::visibleArea(){
	if(hidden){return rect();}
	if(parent){
		if(isClient){
			//return parent->worldClientArea().clamp(worldArea());
			return parent->visibleClientArea().clamp(worldArea());
		}else{
			return parent->visibleArea().clamp(worldArea());
		}
	}else{
		return worldArea();
	}
}
rect GUIbase::visibleClientArea(){
	if(hidden){return rect();}
	if(parent){
		if(isClient){
			//return parent->worldClientArea().clamp(worldArea());
			return parent->visibleClientArea().clamp(worldClientArea());
		}else{
			return parent->visibleArea().clamp(worldClientArea());
		}
	}else{
		return worldClientArea();
	}
}
//GUIgrid ----------------------------------------------------------------------

GUIgrid::GUIgrid(){}
void GUIgrid::grid(GUIbase *child, int row, int col, int spanx, int spany){
	//printf("grid(%p, row %d, col %d, spanx %d, spany %d)\n",child,row,col,spanx,spany);
	//if(!rows.size()){resize(1,1);}
	
	if(row == -1){row = rows.size() - 1;}
	spanx = max(spanx,1);
	spany = max(spany,1);
	resize(row+spany,col+spanx);
	if(spanx > 1){colspans.push_back(linespan{col,col+spanx-1,&(rows[row][col])});}
	if(spany > 1){rowspans.push_back(linespan{row,row+spany-1,&(rows[row][col])});}
	
	rows[row][col] = (gridcell){spanx,spany,child};
	invalidateTree();
	//printf("\n");
}
void GUIgrid::configureRow(int row, float min, float max, float weight){
	rowsettings[row] = (gridline){min,max,weight,0,0};
	invalidateTree();
}
void GUIgrid::configureColumn(int col, float min, float max, float weight){
	colsettings[col] = (gridline){min,max,weight,0,0};
	invalidateTree();
}
void GUIgrid::render(){
	//setScissoring(false);
	//GUIbase::render();
	//setColor(vec3(255,128,128));
	//setAlpha(128);
	//drawRect(worldArea());
	//setAlpha(255);
	//setColor(vec3(128,0,0));
	//drawRectOutline(worldArea());
	//setColor(vec3(255,0,255));
	//drawRectOutline(parent->parent->clientToWorld(parent->clientArea));
	//drawRectOutline(parent->visibleClientArea());
	//printf("g    area: %s\n",toString(area).c_str());
	// printf("gp   area: %s\n",toString(parent->area).c_str());
	// printf("gpc  area: %s\n",toString(parent->clientArea).c_str());
	// printf("gpp  area: %s\n",toString(parent->parent->area).c_str());
	// printf("gppc area: %s\n",toString(parent->parent->clientArea).c_str());
	// printf("\n");
	// exit(0);
	//drawRectOutline(visibleArea());
	/*
	setColor(vec3(255,0,0));
	vec2 offset = worldArea().start;
	for(int R = 0; R < rowsettings.size(); R++){
		for(int C = 0; C < colsettings.size(); C++){
			float y = rowsettings[R].pos;
			float ys = rowsettings[R].cur;
			float x = colsettings[C].pos;
			float xs = colsettings[C].cur;
			rect cellrect = rect().setStart(offset+vec2(x,y)).setSize(vec2(xs,ys));
			//frameprint(fstring("row %d, col %d. x=%f, y=%f, xs=%f, ys=%f, rect=%s",R,C,x,y,xs,ys,toString(cellrect).c_str()));
			drawRectOutline(cellrect);
		}
	}
	*/
	
}

void GUIgrid::resize(int numrows, int numcols){
	//printf("resize(%d,%d)\n",numrows,numcols);
	
	//does every row have settings?
	while(rowsettings.size() < numrows){
		rowsettings.push_back((gridline){-1,-1,0,0,0});
		//printf("added settings for row %d (%d rowsets)\n",rowsettings.size()-1,rowsettings.size());
	}
	//does every column have settings?
	while(colsettings.size() < numcols){
		colsettings.push_back((gridline){-1,-1,0,0,0});
		//printf("added settings for col %d (%d colsets)\n",colsettings.size()-1,colsettings.size());
	}
	
	//are there enough rows?
	while(rows.size() < rowsettings.size()){
		rows.push_back(gridrow());
		//printf("added row %d              (%d rows)\n",rows.size()-1, rows.size());
	}
	//does every row have enough columns?
	for(int R = 0; R < rows.size(); R++){
		while(rows[R].size() < colsettings.size()){
			rows[R].push_back((gridcell){1,1,0});
			//printf("new cell at row %d, col %d (%d cols)\n",R,rows[R].size()-1,rows[R].size());
		}
	}
}

void GUIgrid::clear(){
	for(int R = 0; R < rowsettings.size(); R++){
		for(int C = 0; C < colsettings.size(); C++){
			delete rows[R][C].child; //can has suppress-invalidate nao?
		}
	}
	rows.clear();
	rowsettings.clear();
	colsettings.clear();
}
#define nonnan(x) if(isnan(x)){error("result is NaN: %s\n",#x);}

void GUIgrid::invalidate(){
	//printf("grid invalidate");
//start with usual invalidate stuff
	//fill entire parent client area.
	//actually, set size later. expand but do not shrink.
	// if(parent){
		// printf("+parent\n");
		// area = rect(parent->clientArea.size);//parent->clientArea.moveTo(vec2(0,0));
	// }else{printf(", no parent\n");}
	// clientArea = area;
//first pass: measure row/column dimensions
	//debug print
	for(int R = 0; R < rowsettings.size(); R++){for(int C = 0; C < colsettings.size(); C++){
		auto &E = rows[R][C].child; 
		//printf("cell (%d,%d):",R,C);
		//if(E){printf("area %s\n",toString(E->area).c_str());}
		// else{printf("no element\n");}
	}}
	//measure row height
	for(int R = 0; R < rowsettings.size(); R++){
		float maxy = 0;
		for(int C = 0; C < colsettings.size(); C++){
			auto &cell = rows[R][C];
			auto &E = cell.child;
			float y = 0;
			if(E){y = E->area.size.y;}
			if(cell.spany > 1){y = 0;} //span will be dealt with later
			nonnan(y);
			maxy = max(maxy,y);
			//if(C == 0){maxy = y;}
			//else{maxy = max(maxy,y);}
		}
		//preserve max content height for now, we still need to figure
		//out how much extra space we have.
		//grid expands when there is extra space.
		//grid does NOT shrink when there is not enough space.
		rowsettings[R].cur = maxy; //set row height to maximum height
		//printf("row %d max height = %f\n",R,maxy);
		nonnan(maxy);
	}
	//distribute span height
	for(int RS = 0; RS < rowspans.size(); RS++){
		int from = rowspans[RS].from;
		int to = rowspans[RS].to;
		auto &E = rowspans[RS].cell->child;
		if(!E){continue;}
		float availy = 0;
		for(int R = from; R <= to; R++){
			availy = availy + rowsettings[R].cur;
		}
		float desiredy = E->area.size.y;
		if(availy){
			float coeff = desiredy/availy;
			if(coeff > 1.f){
				for(int R = from; R < to; R++){
					rowsettings[R].cur = coeff*rowsettings[R].cur;
				}
			}
		}else{
			for(int R = from; R < to; R++){
				rowsettings[R].cur = desiredy/((float)(to-from+1));
			}
		}
	}
	//measure column width
	for(int C = 0; C < colsettings.size(); C++){
		float maxx = 0;
		for(int R = 0; R < rowsettings.size(); R++){
			auto &cell = rows[R][C];
			auto &E = cell.child;
			float x = 0;
			if(E){x = E->area.size.x;}
			if(cell.spanx > 1){x = 0;} //span will be dealt with later
			nonnan(x);
			maxx = max(maxx,x);
			//if(R == 0){maxx = x;}
			//else{maxx = max(maxx,x);}
		}
		//same here
		colsettings[C].cur = maxx; //set column width to maximum width
		//printf("col %d max width = %f\n",C,maxx);
		nonnan(maxx);
	}
	//distribute span width
	//distribute span height
	for(int CS = 0; CS < colspans.size(); CS++){
		int from = colspans[CS].from;
		int to = colspans[CS].to;
		auto &E = colspans[CS].cell->child;
		if(!E){continue;}
		float availx = 0;
		for(int C = from; C <= to; C++){
			availx = availx + colsettings[C].cur;
		}
		float desiredx = E->area.size.x;
		if(availx){
			float coeff = desiredx/availx;
			if(coeff > 1.f){
				for(int C = from; C <= to; C++){
					colsettings[C].cur = coeff*colsettings[C].cur;
				}
			}
		}else{
			for(int C = from; C <= to; C++){
				colsettings[C].cur = desiredx/((float)(to-from+1));
			}
		}
	}
	//measure total height/width and total weight
	float totaly = 0;
	float totalweighty = 0;
	float totalx = 0;
	float totalweightx = 0;
	for(int R = 0; R < rowsettings.size(); R++){
		totaly = totaly + rowsettings[R].cur; 
		totalweighty = totalweighty + rowsettings[R].weight;
	}
	for(int C = 0; C < colsettings.size(); C++){
		totalx = totalx + colsettings[C].cur;
		totalweightx = totalweightx + colsettings[C].weight;
	}
	nonnan(totaly);
	nonnan(totalweighty);
	nonnan(totalx);
	nonnan(totalweightx);
	float extray = 0;
	float extrax = 0;
	if(parent){
		extray = max(parent->clientArea.size.y - totaly,0);
		extrax = max(parent->clientArea.size.x - totalx,0);
	}
	//if too small to fit everything, set area to total area.
	//printf("area1 = %s\n",toString(area).c_str());
	area = rect(vec2(totalx+extrax,totaly+extray));
	clientArea = area;
	//printf("area2 = %s\n",toString(area).c_str());
	
//second pass: define row/col dimensions
	//new row height
	float posy = 0;
	for(int R = 0; R < rowsettings.size(); R++){
		auto &L = rowsettings[R];
		nonnan(L.cur);
		//row wants <weight>/<totalweight> of the extra dimension
		float extra_desired = 0;
		if(L.weight){
			extra_desired = extray*L.weight/totalweighty;
		}
		//but is constrained by the min/max dimensions
		float new_plain = L.cur;
		float new_expanded = L.cur+extra_desired;
		if(L.min != -1){
			new_plain = max(new_plain,L.min);
			new_expanded = max(new_expanded,L.min);
		}
		if(L.max != -1){
			new_plain = min(new_plain,L.max);
			new_expanded = min(new_expanded,L.max);
		}
		
		//float extra_given = clamp(L->cur+extra_desired,L->min,L->max) - clamp(L->cur,L->min,L->max);
		//L->cur = clamp(L->cur+extra_desired,L->min,L->max);
		//extray = extray - extra_given;
		float extra_given = new_expanded - new_plain;
		L.cur = new_expanded;
		nonnan(L.cur);
		extray = extray - extra_given;
		L.pos = posy;
		nonnan(L.pos);
		posy = posy + L.cur;
	}
	//new column width
	float posx = 0;
	for(int C = 0; C < colsettings.size(); C++){
		auto &L = colsettings[C];
		//row wants <weight>/<totalweight> of the extra dimension
		float extra_desired = 0;
		if(L.weight){
			extra_desired = extrax*L.weight/totalweightx;
		}
		//but is constrained by the min/max dimensions
		float new_plain = L.cur;
		float new_expanded = L.cur+extra_desired;
		if(L.min != -1){
			new_plain = min(new_plain,L.min);
			new_expanded = min(new_expanded,L.min);
		}
		if(L.max != -1){
			new_plain = max(new_plain,L.max);
			new_expanded = max(new_expanded,L.max);
		}
		
		//float extra_given = clamp(L->cur+extra_desired,L->min,L->max) - clamp(L->cur,L->min,L->max);
		//L->cur = clamp(L->cur+extra_desired,L->min,L->max);
		//extray = extray - extra_given;
		float extra_given = new_expanded - new_plain;
		L.cur = new_expanded;
		nonnan(L.cur);
		extrax = extrax - extra_given;
		L.pos = posx;
		nonnan(L.pos);
		posx = posx + L.cur;
	}
//third pass: set new cell position and size
	for(int R = 0; R < rowsettings.size(); R++){
		for(int C = 0; C < colsettings.size(); C++){
			auto &cell = rows[R][C];
			if(cell.child){
				//calc. available y-size
				float ysize = 0;
				for(int I = 0; I < cell.spany; I++){
					ysize += rowsettings[R+I].cur;
				}
				//calc. available x-cells
				float xsize = 0;
				for(int I = 0; I < cell.spanx; I++){
					xsize += colsettings[C+I].cur;
				}
				float ypos = rowsettings[R].pos;
				float xpos = colsettings[C].pos;
				rect c_area = cell.child->area;
				vec2 acenter_offset = c_area.size/2.f;
				vec2 bcenter_offset = vec2(xsize,ysize)/2.f;
				c_area = c_area.moveTo(vec2(xpos,ypos)-acenter_offset+bcenter_offset);
				//printf("cell (%d,%d) span %d,%d area %s\n",R,C,cell.spanx,cell.spany,toString(cell.child->area).c_str());
				cell.child->area = c_area;
				//cell.child->invalidate();
			}
		}
	}
}
//GUIframe ---------------------------------------------------------------------
GUIframe::GUIframe(){
	bgColor = defaultbgcolor;
	borderColor = defaultborderColor;
}
GUIframe *GUIframe::setBgColor(vec3 color){
	bgColor = color;
	return this;
}

GUIframe *GUIframe::setBorderColor(vec3 color){
	borderColor = color;
	return this;
}

void GUIframe::render(){
	//pushRenderOptions();
	//GUIbase::render();
	setColor(bgColor);
	setAlpha(128);
	//drawRect(thisToWorld(rect(area.size)));
	drawRect(worldArea());
	setAlpha(255);
	setColor(borderColor);
	//drawRectOutline(thisToWorld(rect(area.size)));
	drawRectOutline(worldArea());
	//popRenderOptions();
}

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
	//popRenderOptions();
}

//GUIimage
GUIimage::GUIimage(){
	image = defaultimage;
}
GUIimage *GUIimage::setImage(texture *newimage){
	image = newimage;
	return this;
}
GUIbase *GUIimage::sizeToContents(){
	if(image){
		area = area.setSize(image->size());
	}else{
		
	}
	return this;
}
void GUIimage::render(){
	//pushRenderOptions();
	GUIframe::render();
	setColor({255,255,255});
	setTexture(image);
	//setTexturing(true); drawRect(thisToWorld(rect(area.size)));
	setTexturing(true);
	drawRect(worldArea());
	setTexturing(false);
	//popRenderOptions();
}

//GUIbutton
GUIbutton::GUIbutton(){
	pressed = false;
	mouseover = false;
	textColor = defaulttextColor;
	hoverColor = defaultbgcolor*0.9;
	pressedColor = defaultbgcolor*0.8;
	textfont = defaulttextfont;
	F = [](){return;};
	text = defaulttext;
	image = defaultimage;
}
/*
GUIbutton *GUIbutton::setTextColor(vec3 color){
	textColor = color;
	return this;
}
GUIbutton *GUIbutton::setTextFont(font *newfont){
	textfont = newfont;
	return this;
}
GUIbutton *GUIbutton::setText(string newtext){
	text = newtext;
	return this;
}
GUIbutton *GUIbutton::setImage(texture *newimage){
	image = newimage;
	return this;
}
*/
GUIbutton *GUIbutton::setFunction(function<void()> f){
	F = f;
	return this;
}
GUIbase *GUIbutton::sizeToContents(){
	vec2 size1 = {0,0};
	if(image){size1 = image->size();}
	rect R = preprintw(textfont,"%s",text.c_str());
	//printf("sizeToContents:\nimage rect: %s\ntext rect: %s\n",
	//	toString(rect(size1)).c_str(),toString(R).c_str());
	vec2 size2 = R.size;
	//popRenderOptions();
	area = area.setSize({max(size1.x,size2.x),max(size1.y,size2.y)});
	printf("GUIbutton:new area = %s\n",toString(area).c_str());
	return this;
}
void GUIbutton::render(){
	vec3 oldColor = bgColor;
	if(!mouseover){pressed = false;}
	if(mouseover){setAlpha(196); bgColor = hoverColor;}
	if(pressed){setAlpha(255); bgColor = pressedColor;}
	GUIframe::render();
	bgColor = oldColor;
	
	if(image){GUIimage::render();}
	GUIlabel::render();
/*
	//pushRenderOptions();
	vec3 oldColor = bgColor;
	
	vec2 pos = getMousePos();
	//mouseover = thisToWorld(rect(area.size)).contains(pos);
	mouseover = visibleArea().contains(pos);
	if(!mouseover){pressed = false;}
	
	if(mouseover){setAlpha(196); bgColor = hoverColor;}
	if(pressed){setAlpha(255); bgColor = pressedColor;}
	GUIframe::render();
	bgColor = oldColor;
	setColor(bgColor);
	if(image){
		setTexture(image);
		//setTexturing(true); drawRect(thisToWorld(rect(area.size)));
		setTexturing(true);
		drawRect(worldArea());
		setTexturing(false);
	}
	setColor(textColor);
	setFont(textfont);
	//vec2 pos = area.start;
	//pos.y += area.size.y/2;
	rect tRect = preprintw(textfont,"%s",text.c_str());
	vec2 offset;
	offset.x = (area.size.x - tRect.size.x)/2;
	offset.y = (area.size.y-tRect.size.y)/2;
	
	//setTextPos(thisToWorld(offset));
	vec2 tp = getTextCentering(worldArea(),tRect);//+offset;
	//vec2 tpi = {tp.x,tp.y};
	//setColor({0,255,0});
	//drawRectOutline(tRect.moveBy(tp));
	setTextPos(vec2(tp.x,tp.y));
	printw("%s",text.c_str());
	//popRenderOptions();
*/
}
void GUIbutton::onEvent(eventKind event){
	GUIbase::onEvent(event); 
	if(event.isMasked()){return;}
	
	if(event.type == EVENT_MOUSE_BUTTON_DOWN){
		if(mouseover){
			pressed = true;
			//printf("btn click\n");
			event.maskEvent();
		}
	}
	if(event.type == EVENT_MOUSE_BUTTON_UP){
		if(pressed){
			pressed = false;
			//printf("btn unclick\n");
			event.maskEvent();
			F(); //once we call this... we might never return
			//jk but it might kill us (no member access from now on)
		}
	}
	/* 
	if(event.type == EVENT_MOUSE_MOVE){
		vec2 pos = getMousePos();
		mouseover = thisToWorld(rect(area.size)).contains(pos);
		if(!mouseover){pressed = false;}
	} */
}

//GUIscrollbarBar
GUIscrollbarBar::GUIscrollbarBar(){
	mouseover = false;
	pressed = false;
	offset = {0,0};
	hoverColor = bgColor*0.9;
	pressedColor = bgColor*0.8;
}

void GUIscrollbarBar::render(){
	//pushRenderOptions();
	vec2 pos = getMousePos();
	//vec2 min = {parent->area.size.x-21,21};
	//vec2 max = {parent->area.size.x-21,parent->area.size.y-21-area.size.y};
	//if(pressed){moveTo(clamp(offset+pos, min,max));if(parent){parent->invalidate();}}
	//else{moveTo(clamp(area.start, min, max));}
	
	//mouseover = visibleArea().contains(pos);
	//mouseover = thisToWorld(rect(area.size)).contains(pos);
	vec3 oldColor = bgColor;
	if(mouseover){setAlpha(196); bgColor = hoverColor;}
	if(pressed){setAlpha(255); bgColor = pressedColor;}
	GUIframe::render();
	bgColor = oldColor;
	//popRenderOptions();
}

void GUIscrollbarBar::onEvent(eventKind event){
	GUIbase::onEvent(event); 
	if(event.isMasked()){return;}
	
	vec2 pos = getMousePos();
	if(event.type == EVENT_MOUSE_BUTTON_DOWN){
		if(mouseover){
			pressed = true;
			event.maskEvent();
			offset = area.start-pos;
		}
	}
	if(event.type == EVENT_MOUSE_BUTTON_UP){
		if(pressed){
			pressed = false;
			event.maskEvent();
		}
	}
	if(event.type == EVENT_MOUSE_MOVE){		
		vec2 pos = getMousePos();
		if(pressed){moveTo(offset+pos); if(parent){parent->invalidate();}}
	}
}

//GUIscrollbar
GUIscrollbar::GUIscrollbar(){
	innerSize = area.size;
	//scrollPos = {0,0};
	bsizeToParent = false;
	vertical = true;
	horizontal = true;
	
	GUIbutton *btnUp = new GUIbutton();
	btnUp->isClient = false;
	btnUp->setText("^");
	btnUp->setSize({20,20});
	addChild(btnUp);
	
	GUIbutton *btnDown = new GUIbutton();
	btnDown->isClient = false;
	btnDown->setText("v");
	btnDown->setSize({20,20});
	//btnDown->moveTo({0,20});
	addChild(btnDown);
	
	GUIscrollbarBar *vbar = new GUIscrollbarBar();
	vbar->isClient = false;
	//vbar->setSize({20,30});
	addChild(vbar);
	
	GUIbutton *btnLeft = new GUIbutton();
	btnLeft->isClient = false;
	btnLeft->setText("<");
	btnLeft->setSize({20,20});
	addChild(btnLeft);
	
	GUIbutton *btnRight = new GUIbutton();
	btnRight->isClient = false;
	btnRight->setText(">");
	btnRight->setSize({20,20});
	addChild(btnRight);
	
	GUIscrollbarBar *hbar = new GUIscrollbarBar();
	hbar->isClient = false;
	addChild(hbar);
	
	
	btnUp->setFunction([=](){vbar->area = vbar->area.moveBy({0,-max(vbar->area.size.y/4,1)}); invalidate();});
	btnDown->setFunction([=](){vbar->area = vbar->area.moveBy({0,max(vbar->area.size.y/4,1)}); invalidate();});
	btnLeft->setFunction([=](){hbar->area = hbar->area.moveBy({-max(hbar->area.size.x/4,1),0}); invalidate();});
	btnRight->setFunction([=](){hbar->area = hbar->area.moveBy({max(hbar->area.size.x/4,1),0}); invalidate();});
	/* 
	GUIbutton *btnV = new GUIbutton();
	btnV->setText("vert");
	btnV->sizeToContents();
	btnV->moveTo({0,0});
	btnV->setFunction([this](){this->setVertical(!this->vertical);});
	addChild(*btnV);
	
	GUIbutton *btnH = new GUIbutton();
	btnH->setText("horiz");
	btnH->sizeToContents();
	btnH->moveTo({0,20});
	btnH->setFunction([this](){this->setHorizontal(!this->horizontal);});
	addChild(*btnH); */
	
	setVertical(vertical);
	setHorizontal(horizontal);
	invalidate();
}

GUIscrollbar *GUIscrollbar::setInnerSize(vec2 newInnerSize){
	innerSize = newInnerSize;
	invalidate();
	return this;
}

GUIscrollbar *GUIscrollbar::setScrollOffset(vec2 newScrollOffset){	
	//printf("======setScrollOffset %s\n",toString(newScrollOffset).c_str());
	invalidate();
	float x =  (float)newScrollOffset.x / (innerSize.x-clientArea.size.x);
	float y =  (float)newScrollOffset.y / (innerSize.y-clientArea.size.y);
	setScrollRatio({x,y});
	invalidate();
	//printf("======new offset %s\n",toString(scrollOffset()).c_str());
	return this;
}

// vec2 GUIscrollbar::scrollRatio(){
	// rect vtrack = vtrackRect();
	// rect htrack = htrackRect();
	// float xoffset = children[5]->area.start.x - htrack.start.x;
	// float x = (float)xoffset / (htrack.size.x-children[5]->area.size.x);
	// float yoffset = children[2]->area.start.y - vtrack.start.y;
	// float y = (float)yoffset / (vtrack.size.y-children[2]->area.size.y);
	// return {x,y};
// }
GUIscrollbar *GUIscrollbar::setScrollRatio(vec2 newScrollRatio){
	//printf("======setScrollRatio %s\n",toString(newScrollRatio).c_str());
	invalidate();
	rect vtrack = vtrackRect();
	rect htrack = htrackRect();
	float xoffset = newScrollRatio.x * (htrack.size.x-children[5]->area.size.x);
	children[5]->area = children[5]->area.moveTo({xoffset + htrack.start.x,children[5]->area.start.y});
	float yoffset = newScrollRatio.y * (vtrack.size.y-children[2]->area.size.y);
	children[2]->area = children[2]->area.moveTo({children[2]->area.start.x,yoffset + vtrack.start.y});
	invalidate();
	//printf("======new ratio: %s\n",toString(scrollRatio()).c_str());
	return this;
}

GUIscrollbar *GUIscrollbar::sizeToParent(bool b){
	bsizeToParent = b;
	return this;
}

GUIscrollbar *GUIscrollbar::setVertical(bool newVertical){
	vec2 offset = scrollOffset();
	vertical = newVertical;
	if(children.size() >= 6){
		children[0]->setHidden(!vertical);
		children[1]->setHidden(!vertical);
		children[2]->setHidden(!vertical);
	}
	invalidate();
	setScrollOffset(offset);
	return this;
}

GUIscrollbar *GUIscrollbar::setHorizontal(bool newHorizontal){
	vec2 offset = scrollOffset();
	horizontal = newHorizontal;
	if(children.size() >= 6){
		children[3]->setHidden(!horizontal);
		children[4]->setHidden(!horizontal);
		children[5]->setHidden(!horizontal);
	}
	invalidate();
	setScrollOffset(offset);
	return this;
}

void GUIscrollbar::renderLogic(){
	
	vec2 offset = {0,0};
	if(children.size() >= 6){
		offset = scrollOffset();
	}
	for(int I = 0; I < children.size(); I++){
		if(!children[I]->isClient && !children[I]->hidden){
			//pushRenderOptions();
			//drawRectOutlineColored(children[I]->visibleArea(),{255,0,0});
			setScissoring(true);
			setScissor(children[I]->visibleArea());
			children[I]->render();		
			children[I]->renderLogic();
			//popRenderOptions();
		}
	}
	//setScissoring(true);
	//setScissor(visibleClientArea());
	//setColor({255,0,0});
	//drawRectOutline(visibleClientArea());
	for(int I = 0; I < children.size(); I++){
		if(children[I]->isClient && !children[I]->hidden){
			//pushRenderOptions();
			children[I]->area=children[I]->area.moveBy(-offset);
			//drawRectOutlineColored(children[I]->visibleArea(),{255,0,0});
			setScissoring(true);
			setScissor(children[I]->visibleArea());
			children[I]->render();		
			children[I]->renderLogic();
			children[I]->area=children[I]->area.moveBy(offset);
			//popRenderOptions();
		}
	}
	//popRenderOptions();
	
}

void GUIscrollbar::render(){
	GUIframe::render();
	//pushRenderOptions();
	setAlpha(128);
	setColor(bgColor*0.9);
	//rect R = rect().setStart({area.size.x-22,1}).setEnd({area.size.x-1,area.size.y-1});
	//drawRect(thisToWorld(R));
	if(vertical){
		rect R = vtrackRect().setStart(vtrackRect().start-(vec2){0,21})\
					.setEnd(vtrackRect().end+(vec2){0,21});
		drawRect(thisToWorld(R));
	}
	if(horizontal){
		rect R = htrackRect().setStart(htrackRect().start-(vec2){21,0})\
					.setEnd(htrackRect().end+(vec2){21,0});
		drawRect(thisToWorld(R));
	}
	//popRenderOptions();
	
}

void GUIscrollbar::invalidate(){
	//printf("scrollbar invalidate");
	if(parent && bsizeToParent){
		//printf("+parent\n");
		area = area.setSize(parent->clientArea.size);
	}else{
		//printf(", no parent\n");
	}
	clientArea = clientArea.setStart({1,1}).setEnd(area.size-(vec2){vertical? 22: 0, horizontal? 22:0});
	if(children.size() >= 6){
		rect vtrack = vtrackRect();
		children[0]->moveTo(vtrack.start-(vec2){0,21});
		children[1]->moveTo(vtrack.end-children[1]->area.size+(vec2){0,21});
		rect Vr = children[2]->area;
		Vr.size = {20,trackDimensions().y * areaRatio().y};
		Vr.size = clamp(Vr.size,{0,0},vtrack.size);
		Vr = Vr.moveTo(vtrack.clamp(Vr.end)-Vr.size);
		Vr = Vr.moveTo(vtrack.clamp(Vr.start));
		children[2]->area = Vr;
		
		rect htrack = htrackRect();
		children[3]->moveTo(htrack.start-(vec2){21,0});
		children[4]->moveTo(htrack.end-children[4]->area.size+(vec2){21,0});
		rect Hr = children[5]->area;
		Hr.size = {trackDimensions().x * areaRatio().x,20};
		Hr.size = clamp(Hr.size,{0,0},htrack.size);
		Hr = Hr.moveTo(htrack.clamp(Hr.end)-Hr.size);
		Hr = Hr.moveTo(htrack.clamp(Hr.start));
		children[5]->area = Hr;
		
		// printf("--------------\n");
		// printf("vtrackRect: %s\n",toString(vtrackRect()).c_str());
		// printf("htrackRect: %s\n",toString(htrackRect()).c_str());
		// printf("trackDimensions: %s\n",toString(trackDimensions()).c_str());
		// printf("areaRatio: %s\n",toString(areaRatio()).c_str());
		// printf("normalizedAreaRatio: %s\n",toString(normalizedAreaRatio()).c_str());
		// printf("scrollRatio: %s\n",toString(scrollRatio()).c_str());
		// printf("scrollOffset: %s\n",toString(scrollOffset()).c_str());
		
	}
}

rect GUIscrollbar::vtrackRect(){
	int height = trackDimensions().y;
	return rect().setStart({area.size.x-21,22}).setSize({20,height});
}

rect GUIscrollbar::htrackRect(){
	int width = trackDimensions().x;
	return rect().setStart({22,area.size.y-21}).setSize({width,20});
}

vec2 GUIscrollbar::trackDimensions(){
	return {vertical? area.size.x - 64 : area.size.x - 44, 
			horizontal? area.size.y - 64 : area.size.y - 44};
}

vec2 GUIscrollbar::areaRatio(){
	return {innerSize.y ? (float)clientArea.size.y/innerSize.y : 1.0,
			innerSize.x ? (float)clientArea.size.x/innerSize.x : 1.0};
}
vec2 GUIscrollbar::normalizedAreaRatio(){
	vec2 ar = areaRatio();
	ar.x = min(ar.x,1.0);
	ar.y = min(ar.y,1.0);
	return ar;
}

vec2 GUIscrollbar::scrollRatio(){
	rect vtrack = vtrackRect();
	rect htrack = htrackRect();
	float xoffset = children[5]->area.start.x - htrack.start.x;
	float x = (float)xoffset / (htrack.size.x-children[5]->area.size.x);
	float yoffset = children[2]->area.start.y - vtrack.start.y;
	float y = (float)yoffset / (vtrack.size.y-children[2]->area.size.y);
	return {x,y};
}
//float -> integer round down causes x,y to drift towards 0
vec2 GUIscrollbar::scrollOffset(){
	int x = (innerSize.x-clientArea.size.x) * scrollRatio().x;
	int y = (innerSize.y-clientArea.size.y) * scrollRatio().y;
	return clamp({x,y},{0,0},innerSize-clientArea.size);
}

//GUIwindow
GUIwindow::GUIwindow(){
	hasCloseButton = true;
	moveable = true;
	pressed = false;
	mouseover = false;
	mouseovertitle = false;
	
	GUIbutton *btnClose = new GUIbutton();
	btnClose->isClient = false;
	btnClose->setSize({21,21});
	btnClose->setText({"x"});
	btnClose->setFunction([this](){
		printf("closebtn pressed\n");
		this->close();
	});
	addChild(btnClose);
	
	GUIlabel *title = new GUIlabel();
	title->isClient = false;
	title->moveTo({5,5});
	title->setText({"Title"});
	title->sizeToContents();
	addChild(title);
	invalidate();
}

GUIwindow *GUIwindow::setCloseButton(bool hasbtn){
	children[0]->setHidden(!hasbtn);
	return this;
}

GUIwindow *GUIwindow::setMoveable(bool newmoveable){
	moveable = newmoveable;
	return this;
}

GUIwindow *GUIwindow::setTitle(string title){
//	((GUIlabel*)children[1])->setText(title);
//	((GUIlabel*)children[1])->sizeToContents();
	dynamic_cast<GUIlabel*>(children[1])->setText(title);
	dynamic_cast<GUIlabel*>(children[1])->sizeToContents();
	return this;
}

void GUIwindow::close(){
	delete this; //hello?
	// if(parent){
		// auto &pc = parent->children;
		// for(int I = 0; I < pc.size(); I++){
			// if(pc[I] == this){
				// pc.erase(pc.begin()+I);
				// I--;
			// }
		// }
	// }
}

void GUIwindow::render(){
	vec2 pos = getMousePos();
	//mouseover = visibleArea().clamp(thisToWorld(rect(area.size.x-23,23))).contains(pos);
	
	//pushRenderOptions();
	setColor(borderColor);
	drawRect(thisToWorld(rect(area.size.x,23)));
	//popRenderOptions();
	GUIframe::render();
}

void GUIwindow::onEvent(eventKind event){
	GUIbase::onEvent(event); 
	if(event.isMasked()){return;}
	
	vec2 pos = getMousePos();
	if(event.type == EVENT_MOUSE_BUTTON_DOWN){
		if(mouseover){
			printf("click window\n");
			event.maskEvent();
			if(mouseovertitle){
				pressed = true;
				offset = area.start-pos;
			}
		}
	}
	if(event.type == EVENT_MOUSE_BUTTON_UP){
		if(pressed){
			pressed = false;
			printf("unclick window\n");
			event.maskEvent();
		}
	}
	if(event.type == EVENT_MOUSE_MOVE){
		vec2 pos = getMousePos();
		mouseovertitle = visibleArea().clamp(thisToWorld(rect(area.size.x-23,23))).contains(pos);
		if(pressed){moveTo(pos+offset);} //this shouldn't happen on render
	}
}

void GUIwindow::invalidate(){
	clientArea = area.setStart({0,23}).setEnd(area.size-(vec2){1,1});
	if(children.size() >= 2){
		children[0]->moveTo({area.size.x-(1+children[0]->area.size.x),1});
	}
}

GUItextEntry::GUItextEntry(){
	//pressed = false;
	//mouseover = false;
	//textColor = defaulttextColor;
	//hoverColor = defaultbgcolor;
	//pressedColor = (vec3){255,255,255};
	//textfont = defaulttextfont;
	//F = [](){return;};
	//text = defaulttext;
	//image = defaultimage;
	focusedColor = (vec3){255,255,255};
	
	hasfocus = false;
	
	callOnEdit		= false;
	callOnEnter		= true;
	callOnUnfocus	= true;
	unfocusOnEnter	= true;
	clearOnEnter	= false;
	
	multiline = false;
	numeric = false;
	text = "";

	alignment_horizontal = ALIGN_LEFT;
	alignment_vertical = ALIGN_CENTER;//ALIGN_BOTTOM;
	const_height = false;//true; //alignment doesn't work correctly with const_height
}

GUItextEntry *GUItextEntry::setText(string newtext){text = newtext; return this;}
//GUItextEntry *GUItextEntry::setCallOnEdit(bool b){callOnEdit = b; return this;}
//GUItextEntry *GUItextEntry::setCallOnEnter(bool b){callOnEnter = b; return this;}
//GUItextEntry *GUItextEntry::setMultiline(bool b){multiline = b; return this;}
//GUItextEntry *GUItextEntry::setNumeric(bool b){numeric = b; return this;}
GUItextEntry *GUItextEntry::setFunction(function<void()> f){F = f; return this;}
double GUItextEntry::getNumber(){float N = 0; sscanf(text.c_str(),"%f",&N); return N;}

//#define preserve(i,x) {auto temp = i; x; i = temp;}
void GUItextEntry::render(){
	vec3 oldColor1 = bgColor;
	vec3 oldColor2 = hoverColor;
	if(hasfocus){hoverColor = bgColor = focusedColor;}
	GUIbutton::render();
	bgColor = oldColor1;
	hoverColor = oldColor2;
}

void GUItextEntry::onEvent(eventKind event){
	GUIbase::onEvent(event); 
	//if(event.isMasked()){return;}
	
	bool unfocus = 0;
	bool edit = 0;
	bool enter = 0;
	bool call = 0;
	if(event.type == EVENT_MOUSE_BUTTON_DOWN){
		if(mouseover){
			hasfocus = true;
		}else{
			if(hasfocus){unfocus = true;}
		}
	}
	if(hasfocus){
		if(event.type == EVENT_KEY_DOWN){
			const char *K = event.keyboard.key;
			char C = event.keyboard.keycode;
			//printf("key_down [%s]/[%d]\n",K,C);
			if(string("Backspace") == K){	//backspace
				event.maskEvent();
				if(text != ""){text.pop_back();}
				edit = true;
			}
			if(string("Return") == K){	//enter
				event.maskEvent();
				enter = true;
				if(multiline){
					text += '\n';
					edit = true;
				}
				if(clearOnEnter){
					text = "";
					edit = true;
				}
				//printf("TE:enter:[%s]\n",text.c_str());
			}
			if(event.keyboard.printchar){
				event.maskEvent();
				text += event.keyboard.printchar;
				edit = true;
			}
		}
	}
	if(callOnEdit && edit){call = true;}
	if(callOnEnter && enter){call = true;}
	if(unfocusOnEnter && enter){unfocus = true;}
	if(callOnUnfocus && unfocus){call = true;}
	if(unfocus){hasfocus = false;}
	if(call){F();}
}
