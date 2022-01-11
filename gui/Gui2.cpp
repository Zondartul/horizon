#include "Gui2.h"
//Gui.h
#include <string>
#include <cstdio>
#include <math.h>
#include "../util/globals.h"
#include "../util/messenger.h"
#include "../display/paint.h"
#include "../resource/fonts.h"
#include "../resource/textureloader.h"
#define GUI2_GUARD
#include "../main/control.h" //bad

using std::string;

//#include "paint.h"

color3i paintColor;
byte paintAlpha;
vec2i mouseP; // mid-man variable for extra GUI's
void setColor(color3i color)
{
	paintColor = color;
	glColor4f(paintColor.r/255.0f,paintColor.g/255.0f,paintColor.b/255.0f,paintAlpha/255.0f);
}
void setAlpha(byte alpha)
{
	paintAlpha = alpha;
	glColor4f(paintColor.r/255.0f,paintColor.g/255.0f,paintColor.b/255.0f,paintAlpha/255.0f);
}
GUIbase* GUIbase::lastClicked = NULL; //I hope you won't need to click two things at once.
GUIbase* GUIbase::focus = NULL; //wtfffff
GUIbase::GUIbase(){
		pos = size = {0,0};
		crect = {0,0,0,0};
		color_border = color_text = {0,0,0};
		color_panel = {255,255,255};
		border = 2;
		strata = 0;
		dockup = dockdown = dockleft = dockright = false;
		
		children = new listNode;
		children->thing = NULL;
		children->next = NULL;
		parent = NULL;
		
		resizible = true;
		movable = true;
		visible = true;
		scissor = true;
		
		resizingW = 0;
		resizingH = 0;
		pressed = false;
		dragging = false;
		
		mouseOver = false;
		kbFocus = false;
		
		startTouch = {0,0};
		counter = 0;
		tag = "";
		think = NULL;
}
void GUIbase::setPos(int x, int y){
		invalidate((vec2i){x,y},size);
		setdock(dockup,dockdown,dockleft,dockright);
}
void GUIbase::setSize(int x, int y)
{
	invalidate(pos, (vec2i){x,y});
	setdock(dockup,dockdown,dockleft,dockright);
}
void GUIbase::setdock(bool up, bool down, bool left, bool right)
{

	dockup = up;
	dockdown = down;
	dockleft = left;
	dockright = right;
	if(parent)
	{
		dock.x1 = (parent->pos.x+parent->size.x)-pos.x; // distance from left edge to right border
		dock.x2 = (parent->pos.x+parent->size.x)-(pos.x+size.x); // distance from right edge to right border
		dock.y1 = (parent->pos.y+parent->size.y)-pos.y; // distance from upper edge to lower border
		dock.y2 = (parent->pos.y+parent->size.y)-(pos.y+size.y); // distance from lower edge to lower border
	}
	dockCheck();

}

void GUIbase::dockCheck()
{
	if(!(dockup||dockdown||dockleft||dockright)){return;}
	if(parent)
	{
		//printf("\n------------\ndock debug:\n (x1 l2r):%d, (x2 r2r):%d\n(y1 u2d):%d, (y2 d2d):%d\n",dock.x1,dock.x2,dock.y1,dock.y2);
		//printf("parent->pos: x = %d, y = %d\nparent->size: x = %d, y = %d\n",parent->pos.x,parent->pos.y,parent->size.x,parent->size.y);
		//printf("before:pos: x=%d, y=%d; size: x=%d, y=%d\n",pos.x,pos.y,size.x,size.y);
		if(dockup){pos.y = parent->pos.y+parent->size.y-dock.y1;}
		if(dockdown){size.y = parent->pos.y+parent->size.y-dock.y2-pos.y;}
		if(dockleft){pos.x = parent->pos.x+parent->size.x-dock.x1;} // I dunno why not use only 2 coordinates.
		if(dockright){size.x = parent->pos.x+parent->size.x-dock.x2-pos.x;}	
		//printf("after:pos: x=%d, y=%d; size: x=%d, y=%d\n",pos.x,pos.y,size.x,size.y);
		//printf("-------------\n");
	}
}
void GUIbase::onClick(int mb)
{
	if(mb>0)
	{
		startTouch = mouseP-pos; //has one?
		pressed = true;
		if(resizible)
		{
			int mx = mouseP.x;
			int my = mouseP.y;
			int px = pos.x;
			int py = pos.y;
			int psx = px + size.x;
			int psy = py + size.y;
			int b = border-1;
			if(mx-px <= b){resizingW = 1;}
			if(psx - mx <= b){resizingW = 2;}
			if(my - py <= b){resizingH = 1;}
			if(psy - my <= b){resizingH = 2;}
		}
		if(!resizingW&&!resizingH&&movable){dragging = 1;}
	}
	else //what if there was no first click?
	{
		pressed = false;
		resizingW = 0; resizingH = 0; dragging = 0;
	}
	counter++;
}
void GUIbase::onKeyboard(string kb)
{
}
void GUIbase::PSreceive(message msg){
	if(msg.type == "key_down" || msg.type == "key_still_down"){
		propagateKeyboard(msg.str);
	}
}
//void onKeyboard();
void GUIbase::invalidate(vec2i newPos, vec2i newSize)
{
	//this one should also reposition children and stuff
	vec2i sendPos = newPos-pos;//+newSize-size; and guess what happens when x is parented to y of size {0,0}?
	
	
	pos = newPos;
	recalculateClientRect();
	
	size = newSize; //don't transmit size to chilren (use pack[2] if needed)
	dockCheck();
	foreach(this, &wrapInvalidate, (void*)(&sendPos), 0);
}
void GUIbase::recalculateClientRect()
{
	crect.x1 = pos.x+border;
	crect.y1 = pos.y+border;
	crect.x2 = pos.x+size.x-border;
	crect.y2 = pos.y+size.y-border;
}
void GUIbase::resizeCheck()
{
	if(!pressed){return;}
	vec2i newSize = size;
	vec2i newPos = pos;
	if(resizingW==2)
	{
		newSize.x = mouseP.x-pos.x;
		newSize.x = max(newSize.x, (border*2));
	}
	if(resizingW==1)
	{
		int diff = (mouseP).x-pos.x;
		diff = min(diff, size.x-(border*2));
		newPos.x+=diff;
		newSize.x-=diff;
	}
	if(resizingH==2)
	{
		newSize.y = (mouseP).y-pos.y;
		newSize.y = max(newSize.y, (border*2));
	}
	if(resizingH==1)
	{
		int diff = (mouseP).y-pos.y;
		diff = min(diff, size.y-(border*2));
		newPos.y+=diff;
		newSize.y-=diff;
	}
	invalidate(newPos, newSize);
}
void GUIbase::dragCheck()
{
	if(dragging)
	{
		invalidate(mouseP-startTouch, size);
	}
}
void GUIbase::scissorCheck(void* arg)
{
	if(scissor)
	{
		vec4i scissor = *((vec4i*)arg);
		if(!(scissor == (vec4i){0,0,0,0})){glEnable(GL_SCISSOR_TEST);}
		glScissor(scissor.x1-1,height-scissor.y2-1,scissor.x2-scissor.x1+1,scissor.y2-scissor.y1+1);
	}
}
void GUIbase::render(void* arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	setColor(color_panel);
	if(!mouseOver){setAlpha(0);}
	paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setColor(color_border);
	setAlpha(255);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setColor(color_text);
	printw(pos.x, pos.y+size.y/2, size.x, size.y, "clicks: %d", counter);
	
	glDisable(GL_SCISSOR_TEST);
}

bool GUIbase::foreach(GUIbase* obj, int (*func)(GUIbase*, void*, int), void* arg, int rec) 
{
	//calls func (3rd argument) upon all filled or all (arg2) children nodes of obj(arg1)
	// i couldn't think of saner way. sorry.
	// rec is recursion counter for debugging.
	int stop = false;
	listNode* Cur = obj->children;
	while(Cur!=NULL)
	{
		if(Cur->thing){stop = stop+func((GUIbase*)Cur->thing, arg, rec+1);}
		Cur = Cur->next;
		if(stop==1){Cur=NULL;}
	}
	return stop;
}

bool GUIbase::foreachbackwards_check(listNode* Cur, int (*func)(GUIbase*, void*, int), void* arg, int rec)
{
	bool stop = 0;
	if(Cur->next){stop = foreachbackwards_check(Cur->next, func, arg, rec+1);}
	if(Cur->thing){return func((GUIbase*)Cur->thing, arg, rec+1)||stop;}
	else return stop;
}
bool GUIbase::foreachbackwards(GUIbase* obj, int (*func)(GUIbase*, void*, int), void* arg, int rec) 
{
	//calls func (3rd argument) upon all filled or all (arg2) children nodes of obj(arg1)
	// i couldn't think of saner way. sorry.
	// rec is recursion counter for debugging.
	listNode* Cur = obj->children;
	bool stop = 0;
	if(Cur->next){stop = foreachbackwards_check(Cur->next, func, arg, rec+1);}
	if(Cur->thing){return func((GUIbase*)Cur->thing, arg, rec+1)||stop;}
	else return stop;
}
void GUIbase::setParent(GUIbase* obj)
{
	parent = obj;
	parent->recalculateClientRect();
	invalidate(pos+(vec2i){parent->crect.x1,parent->crect.y1}, size);
	GUIbase::addChild(parent, this);
}
GUIbase* GUIbase::findByTag(string tag)
{
	listNode* Cur = children;
	while(Cur!=NULL)
	{
		if(Cur->thing)
		{
			if(((GUIbase*)(Cur->thing))->tag==tag){return ((GUIbase*)(Cur->thing));}
		}
		Cur = Cur->next;
	}
	return NULL;
}
void GUIbase::addChild(GUIbase* parent, GUIbase* obj)
{
	bool stop = false;
	listNode* Cur = parent->children;
	while(!stop)
	{
		if(!Cur->thing){stop = true; Cur->thing = (void*)obj;}
		if(!Cur->next)
		{
			Cur->next = new listNode;
			Cur->next->next = NULL;
			Cur->next->thing = NULL;
		}
		Cur = Cur->next;
	}
}
int GUIbase::wrapInvalidate(GUIbase* obj, void* arg, int rec)
{
	obj->invalidate(*((vec2i*)arg)+obj->pos,obj->size);
}
int GUIbase::propagateMouseOver(GUIbase* obj, void* arg, int rec)
{
	vec2i mouse = ((vec2i*)arg)[0];
	int x1,x2,y1,y2;
	/*
	x1 = obj->pos.x;
	y1 = obj->pos.y;
	x2 = x1+obj->size.x;
	y2 = y1+obj->size.y;
	*/
	if(true)//temp: if(obj->visible)
	{
		//scary scissor support
		/*
		x1 = max(((vec2i*)arg)[1].x,obj->pos.x);
		y1 = max(((vec2i*)arg)[1].y,obj->pos.y);
		x2 = min(((vec2i*)arg)[2].x,obj->pos.x+obj->size.x);
		y2 = min(((vec2i*)arg)[2].y,obj->pos.y+obj->size.y);
		*/
		if(obj->scissor)
		{
			x1 = max(((vec2i*)arg)[1].x,obj->pos.x);
			y1 = max(((vec2i*)arg)[1].y,obj->pos.y);
			x2 = min(((vec2i*)arg)[2].x,obj->pos.x+obj->size.x);
			y2 = min(((vec2i*)arg)[2].y,obj->pos.y+obj->size.y);
		}
		else
		{
			x1 = obj->pos.x;
			y1 = obj->pos.y;
			x2 = x1+obj->size.x;
			y2 = y1+obj->size.y;
		}
		int x1c,x2c,y1c,y2c;
		x1c = max(((vec2i*)arg)[1].x,obj->crect.x1);
		y1c = max(((vec2i*)arg)[1].y,obj->crect.y1);
		x2c = min(((vec2i*)arg)[2].x,obj->crect.x2);
		y2c = min(((vec2i*)arg)[2].y,obj->crect.y2);
		
		vec2i newarg[3];
		newarg[0] = ((vec2i*)arg)[0];
		newarg[1] = (vec2i){x1c,y1c};
		newarg[2] = (vec2i){x2c,y2c};
		
		if((rec!=0)&&(obj->visible==false)){obj->mouseOver=false; return false;}
		if(foreach(obj, &propagateMouseOver, (void*)newarg, rec+1)){obj->mouseOver = false; return true;}
		if((x1<=mouse.x)&&(x2>=mouse.x)&&(y1<=mouse.y)&&(y2>=mouse.y))
		{
			obj->mouseOver = true;
			
			//overly fancy crap of little use
			if(obj->resizible)
			{// normally you'd need abs() for these checks
				int mx = mouse.x;
				int my = mouse.y;
				int px = obj->pos.x;
				int py = obj->pos.y;
				int psx = px + obj->size.x;
				int psy = py + obj->size.y;
				int b = obj->border-1;
				bool n,s,e,w;
				n=s=e=w=false;
				if(mx-px <= b){w=true;}
				if(psx - mx <= b){e=true;}
				if(my - py <= b){n=true;}
				if(psy - my <= b){s=true;}
				if((n&&e)||(s&&w)){SetCursor(LoadCursor(NULL,IDC_SIZENESW));}
				else if((n&&w)||(s&&e)){SetCursor(LoadCursor(NULL,IDC_SIZENWSE));}
				else if(n||s){SetCursor(LoadCursor(NULL,IDC_SIZENS));}
				else if(e||w){SetCursor(LoadCursor(NULL,IDC_SIZEWE));}
			}
			return 1;//found do not continue//but continue
		}
		else
		{
			obj->mouseOver = false;
			return 0;//don't stop
		}
	}
	return false;
}
int GUIbase::propagateKeyboard(string kb)
{
	if(focus!=NULL){focus->onKeyboard(kb);return 1;}else{return 0;}
}
int GUIbase::propagateClick(GUIbase* obj, void* arg, int rec)
{
	int mb = *((int*)arg);
	if(mb>0)
	{
		if(obj->mouseOver&&(rec!=0))
		{
			obj->onClick(mb); 
			lastClicked = obj; 
			focus = obj; 
			//fix strata by re-arranging children list
			fixstrata(obj);
			//
			return true;
		}
		else
		{
			if(((rec!=0)&&(obj->visible==false))||(foreach(obj,&propagateClick,arg,rec+1)==false))
			{
				if(rec==0)
				{
					//lastClicked=NULL; focus = NULL;return false;
					
					lastClicked=NULL;
					//if(focus){return true;} I don't actually know what this does
					//else {return false;}
					return false;
				}//rec==0 is when invisible master-parent thingy is cliked.
				return false;
			} else {if(rec!=0){fixstrata(obj);} return true;} //fuck yes
		}
	}
	else //releases are delivered to who you clicked, not current mouseover.
	{
		if(lastClicked){lastClicked->onClick(mb);lastClicked=NULL;}
		else{if(focus){focus = NULL; return false;}else{return false;}} // was true - false, idk.
	}
	//its ok to have no return? What?
}
void GUIbase::fixstrata(GUIbase* obj)
{
	listNode* prevFirst = obj->parent->children;
	listNode* prevThis = NULL;
	listNode* Cur = obj->parent->children;
	if(Cur->thing != (void *)obj)
	{
	while(Cur)
	{
		if((Cur->next)&&(Cur->next->thing==(void *)obj))
		{
			prevThis = Cur->next;
			Cur->next = prevThis->next;
			obj->parent->children = prevThis;
			prevThis->next = prevFirst;
			Cur = NULL;
		}else{Cur = Cur->next;}
	}
	}
}
int GUIbase::propagateRender(GUIbase* obj, void* arg, int rec)
{
	if(true) // temp: if(obj->visible)
	{
		if(obj->visible)
		{
			if(obj->think){obj->think((void*)obj);}
			obj->render(arg);
		}
		if(obj->visible||rec==0)
		{
		vec4i scissor = *((vec4i*)arg);
		vec4i crect = obj->crect;
		scissor.x1 = max(scissor.x1,crect.x1);
		scissor.y1 = max(scissor.y1,crect.y1);
		scissor.x2 = min(scissor.x2,crect.x2);
		scissor.y2 = min(scissor.y2,crect.y2);
		foreachbackwards(obj,&propagateRender, (void*)(&scissor), rec+1);
		}
	}
	return false;
}
GUIbase::~GUIbase()
{
	listNode *Cur, *Prev;
	Cur = parent->children;
	bool stop = false;
	if(lastClicked == this){lastClicked = NULL;}
	
	while(!stop)
	{
		if(Cur == NULL){stop = true;}
		else if(Cur->thing == (void*)this)
		{
			Cur->thing = NULL;
			//if(Cur!=parent->children){Prev->next = Cur->next; delete Cur;}//WHY CANT I DELETE THAT FAGGOT
			stop = true;
		}
		else
		{
			Prev = Cur;
			Cur = Cur->next;
		}
	}
	//delete all children;
	/*
	Cur = parent->children;
	GUIbase* delObj = NULL;
	while(Cur!=NULL)
	{
		if(Cur->thing){delObj = (GUIbase*)(Cur->thing);}
		Prev = Cur;
		Cur = Cur->next;
		delete Prev; //yes - root node too.
		if(delObj){delete delObj;} // avoiding double-edged destructors.
	}*/
	//we're done with the tree.
}

GUIbutton::GUIbutton():GUIbase()
{
	func = NULL;
	arg = NULL;
	text = "";
	image = 0;
}
void GUIbutton::onClick(int mb)
{
	if((mb==0)&&mouseOver)
	{
		GUIbase::onClick(mb);
		if(func)(func(arg));
		printf("[%s]\n",toString(this).c_str());
	}
}
void GUIbutton::setImage(string path)
{
	//ImageTex = LoadTextureRAW(path, 1);
	image = GenTextureBMP(path).t;
}
void GUIbutton::render(void *arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	
	setColor(color_panel);
	if(mouseOver){setAlpha(255);}else{setAlpha(64);}
	if(image==0){paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);}
	else{paintTexturedRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y,image);}
	setAlpha(255);
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setColor(color_text);
	printw(pos.x+2,pos.y-4, size.x, -1,text);
	
	glDisable(GL_SCISSOR_TEST);
}


void GUIframe::btnClose(void* arg)
{
	delete ((GUIframe*)arg);
}
GUIframe::GUIframe():GUIbase()
{
	title = "Title";
	CloseButton = new GUIbutton();
	CloseButton->func = &btnClose;
	CloseButton->movable = false;
	CloseButton->resizible = false;
	CloseButton->scissor = false; //see we can't paint it outside client area and that's a problem.
	CloseButton->setSize(24,24);
	CloseButton->arg = (void*)this;
	CloseButton->setParent((GUIbase*)this);
	CloseButton->tag = "btnClose";
}
void GUIframe::invalidate(vec2i newPos, vec2i newSize)
{
	GUIbase::invalidate(newPos, newSize);
	CloseButton->setPos(pos.x+size.x-border-CloseButton->size.x,pos.y+border);
}


void GUIframe::recalculateClientRect()
{
	crect.x1 = pos.x+border;
	crect.y1 = pos.y+border+32;
	crect.x2 = pos.x+size.x-border;
	crect.y2 = pos.y+size.y-border;
}

void GUIframe::render(void* arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	setColor(color_panel);
	if(!mouseOver){setAlpha(128);}
	paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setColor(color_border);
	setAlpha(255);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+32);
	setColor(color_text);
	printw(pos.x,pos.y+4, size.x, size.y,title);
	
	glDisable(GL_SCISSOR_TEST);
}

GUIlabel::GUIlabel():GUIbase()
{
	text = "label";
	resizible = false;
	movable = false;
	size.y = 22;
	size.x = 256;
}
void GUIlabel::render(void* arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	printwrich(pos.x,pos.y, size.x, size.y, &color_text.r, text);
	//printw(pos.x,pos.y,size.x,size.y,"print error");
	//size.x = printw;
	glDisable(GL_SCISSOR_TEST);
}


GUItextEntry::GUItextEntry():GUIbase()
{
	text = "Text entry";
	resizible = false;
	movable = false;
	multiline = false;
	size.y = 22;
	color_panel = {255,255,255};
	color_border = {0,0,0};
	color_text = {0,0,0};
	sizeToContents = false;
}
void GUItextEntry::render(void* arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	
	setColor(color_panel);
	if(focus==this){setAlpha(255);}else{setAlpha(196);}
	paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setAlpha(255);
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setColor(color_text);
	if(sizeToContents){size.x = printw(pos.x+2,pos.y, -1, -1,text)+4;}
	else{printw(pos.x+2,pos.y, size.x-BRDB, size.y-BRDB,"%s", text.c_str())+4;}
	
	glDisable(GL_SCISSOR_TEST);
}
void GUItextEntry::onKeyboard(string kb)
{
	/*
	if(kb<0){return;}
	if(isprint(kb))
	{
		text += kb;printf("TE[0],");
	}
	
	if(kb==13)
	{
		if(multiline){text += '\n';}
	}
	if(kb==8)
	{
		if(text.length()){text.erase(text.length()-1);}
	}
	if(kb=='v')//ctrl+v = copy-paste
	{
		if(GetKeyState(VK_LCONTROL) & 0x8000)
		{
			text.erase(text.length()-1);
			OpenClipboard(NULL);
			HANDLE pasta = GetClipboardData(CF_TEXT);
			text += (char *)GlobalLock(pasta);
			GlobalUnlock(pasta);
			CloseClipboard();
		}
	}
	*/
	//cout << kb;
	if(kb == "backspace"){
		if(text.length()){text.erase(text.length()-1);}
	}else if((kb == "v") && input.keybuffer["ctrl"]){
		OpenClipboard(NULL);
		HANDLE pasta = GetClipboardData(CF_TEXT);
		text += (char *)GlobalLock(pasta);
		GlobalUnlock(pasta);
		CloseClipboard();
	}else if(kb == "enter"){
		if(multiline){text += '\n';}
	}else if(kb == "space"){
		text += " ";
	}else if(kb.length() == 1){
		if(input.keybuffer["shift"]){
			text += input.getShifted(kb[0]);
		}else{
			text += kb;
		}
	}
	//printf("TE[1],");
	if(callback){callback((void*)&kb);}
	
	//printf("TE[2],");
	
}


GUIcheckbox::GUIcheckbox():GUIbase()
{
	checked = false;
	resizible = false;
	movable = false;
	size = {14,14};
	func = NULL;
}

void GUIcheckbox::onClick(int mb)
{
	//GUIbase::onClick(mb);
	if((mb==0)&&mouseOver)
	{
		checked = !checked;
		if(func){func((void*)&checked);}
	}
}
void GUIcheckbox::render(void* arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	
	setColor(color_panel);
	if(focus==this){setAlpha(255);}else{setAlpha(196);}
	paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setAlpha(255);
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setColor(color_text);
	if(checked){printw(pos.x+size.x/2-4,pos.y-4, -1, -1,"v");}
	glDisable(GL_SCISSOR_TEST);
}


GUIradiogroup::GUIradiogroup()
{
	for(int I = 0;I<32;I++){buttons[I] = NULL;}
	selection = 0;
}

GUIradiobutton::GUIradiobutton():GUIbase()
{
	checked = false;
	resizible = false;
	movable = false;
	size = {16,16};
}
void GUIradiobutton::onClick(int mb)
{
	if((mb==0)&&mouseOver&&(group!=NULL)){group->checkButton((void*)this);}
}
void GUIradiobutton::render(void* arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	
	setColor(color_panel);
	if(focus==this){setAlpha(255);}else{setAlpha(196);}
	paintCircle(pos.x+size.x/2,pos.y+size.y/2,size.x/2);
	//paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setAlpha(255);
	setColor(color_border);
	paintCircleOutline(pos.x+size.x/2,pos.y+size.y/2,size.x/2);
	setColor(color_text);
	if(checked){printw(pos.x+size.x/2-5,pos.y-4, size.x, size.y,"o");}
	glDisable(GL_SCISSOR_TEST);
}

void GUIradiogroup::addButton(void *btn)
{
	((GUIradiobutton*)btn)->group = this;
	for(int I = 0;I<32;I++)
	{
		if(buttons[I]==NULL){buttons[I] = btn; return;}
	}
};
void GUIradiogroup::checkButton(void *btn)
{
	((GUIradiobutton*)btn)->checked = true;
	for(int I = 0;I<32;I++)
	{
		if(buttons[I]!=NULL)
		{
			if(buttons[I] != btn){((GUIradiobutton*)(buttons[I]))->checked = false;}
			else{selection = I+1;}
		}
	}
};

void GUIspinner::fUp(void* arg)
{
	double *vals = ((GUIspinner*)arg)->vals;
	vals[1]+=vals[3];
	if(vals[1]>vals[2]){vals[1]=vals[2];}
	if(((GUIspinner*)arg)->func){((GUIspinner*)arg)->func(((GUIspinner*)arg)->arg);}
}
void GUIspinner::fDown(void* arg)
{
	double *vals = ((GUIspinner*)arg)->vals;
	vals[1]-=vals[3];
	if(vals[1]<vals[0]){vals[1]=vals[0];}
	if(((GUIspinner*)arg)->func){((GUIspinner*)arg)->func(((GUIspinner*)arg)->arg);}
}
GUIspinner::GUIspinner():GUIbase()
{
	counter = 0;
	size = {64,18};
	pos = {0,0};
	//vals = {-10,0,10,3,2};
	vals[0]=-10;vals[1]=0;vals[2]=10;vals[3]=3;vals[4]=2;
	movable = false;
	resizible = false;
	func = NULL;
	btnUp = new GUIbutton;
	btnUp->func = &fUp;
	btnUp->arg = (void*)this;
	btnUp->size = {18,9};
	btnUp->resizible = false;
	btnUp->pos = {size.x-18,0};
	btnUp->text = "^";
	btnUp->setParent((GUIbase*)this);
	
	btnDown = new GUIbutton;
	btnDown->func = &fDown;
	btnDown->arg = (void*)this;
	btnDown->size = {18,9};
	btnDown->resizible = false;
	btnDown->pos = {size.x-18,9};
	btnDown->text = "v";
	btnDown->setParent((GUIbase*)this);

}
void GUIspinner::setVals(double a,double b,double c,double d,double e)
{
	vals[0]=a;vals[1]=b;vals[2]=c;vals[3]=d;vals[4]=e;
}
void GUIspinner::invalidate(vec2i newPos, vec2i newSize)
{
	GUIbase::invalidate(newPos, newSize);
	btnUp->setPos(pos.x+size.x-18,pos.y);
	btnDown->setPos(pos.x+size.x-18,pos.y+9);
}
void GUIspinner::render(void* arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	
	setColor(color_panel);
	paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setColor(color_text);
	printw(pos.x+2,pos.y,-1,-1,"%.*f", (int)vals[4], (double)vals[1]);
	
	glDisable(GL_SCISSOR_TEST);
}


GUIlistbox::GUIlistbox():GUIbase()
{
	size = {96,64};
	selected = 0;
	callback = NULL;
	callarg = NULL;
	selText = "";
	//color_panel = {196,196,196};
	for(int I = 0;I<32;I++)
	{
		sel[I] = NULL;
	}
}
void GUIlistbox::wrapFunc(void* arg)
{
	//arg:
	// 0 - I
	// 1 - self
	// 2 - prev arg
	// 3 - prev func
	void** newArg = (void**)arg;
	GUIlistbox* L = (GUIlistbox*)(newArg[1]);
	int I = *((int*)newArg[0]);
	L->selected = I+1;
	L->selText = L->sel[I]->text;
	if(L->callback){L->callback(L->callarg);}
	if(newArg[3]!=NULL)
	{
		void (*func)(void*) = (void (*)(void*))newArg[3];
		func(newArg[2]);
	}
}
void GUIlistbox::addOption(string text, void (*func)(void*), void* arg)
{
	for(int I = 0;I<32;I++)
	{
		if(sel[I]==NULL)
		{
			sel[I] = new GUIbutton;
			void** newArg = new void*[4];
			int* aye;
			aye = new int;
			aye[0] = I;
			newArg[0] = (void*)aye;
			newArg[1] = (void*)this;
			newArg[2] = arg;
			newArg[3] = (void*)func;
			sel[I]->func = &wrapFunc;
			sel[I]->arg = (void*)newArg;
			sel[I]->text = text;
			sel[I]->size = {size.x, 14};
			sel[I]->setParent((GUIbase*)this);
			return;
		}
	}
}
void GUIlistbox::render(void* arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	//setAlpha(128);
	//setColor(color_panel);
	//paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	//setAlpha(255);
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	
	glDisable(GL_SCISSOR_TEST);
}

void GUIlistbox::invalidate(vec2i newPos, vec2i newSize)
{
	GUIbase::invalidate(newPos, newSize);
	for(int I = 0; I<32;I++)
	{
		if(sel[I]!=NULL)
		{
			sel[I]->setPos(pos.x,pos.y+14*I);
			sel[I]->setSize(size.x,14);
		}
	}
}


void GUIdropdownlist::showList(void* arg)
{
	GUIdropdownlist *L = ((GUIdropdownlist*)arg);
	if(!L->open){L->setSize(L->size.x,96);}else{L->setSize(L->size.x,14);}
	L->open = !L->open;
}
void GUIdropdownlist::setCurOption(void* arg)
{
	GUIdropdownlist* dd = (GUIdropdownlist*)arg;
	dd->text = dd->list->selText;
	showList(dd);
}
GUIdropdownlist::GUIdropdownlist():GUIbase()
{
	open = false;
	movable = false;
	resizible = false;
	text = "";
	size = {64,14};
	list = new GUIlistbox;
	list->callback = &setCurOption;
	list->callarg = (void*)this;
	list->setParent((GUIbase*)this);
	btn = new GUIbutton;
	btn->func = &showList;
	btn->arg = (void*)this;
	btn->setSize(14,14);
	btn->text = "<";
	btn->setParent((GUIbase*)this);
}
void GUIdropdownlist::addOption(string text, void (*func)(void*), void* arg)
{
	list->addOption(text, func, arg);
}
void GUIdropdownlist::render(void* arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	//setAlpha(128);
	//setColor(color_panel);
	//paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	//setAlpha(255);
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setColor(color_text);
	printw(pos.x-2,pos.y-2, size.x, size.y,text);
	
	glDisable(GL_SCISSOR_TEST);
}

void GUIdropdownlist::invalidate(vec2i newPos, vec2i newSize)
{
	GUIbase::invalidate(newPos, newSize);
	btn->setPos(pos.x+size.x-14,pos.y);
	list->setPos(pos.x,pos.y+14);
	recalculateClientRect();
}


GUIslider::GUIslider():GUIbase()
{
	movable = false;
	resizible = false;
	color_panel = {128,128,256};
	size = {128,14};
	vals = {-100,0,100};
	//vals[0]=-100;vals[1]=0;vals[2]=100;
	sliding = false;
}
void GUIslider::render(void *arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	updateSlider();
	//setAlpha(128);
	//setColor(color_panel);
	//paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	
	//setAlpha(255);
	setColor(color_border);
	paintRectOutline(pos.x,pos.y+size.y/2,pos.x+size.x,pos.y+size.y/2);
	setColor(color_panel);
	double xdot = (int)(size.x*((vals[1]-vals[0])/(vals[2]-vals[0])));
	paintCircle(pos.x+xdot,pos.y+size.y/2,size.y/2);
	setColor(color_border);
	paintCircleOutline(pos.x+xdot,pos.y+size.y/2,size.y/2);
	setColor(color_text);
	printw(pos.x+size.x,pos.y, size.x, size.y,"%f",vals[1]);
	glDisable(GL_SCISSOR_TEST);
}
void GUIslider::onClick(int mb)
{
	sliding = mb;
}
void GUIslider::updateSlider()
{
	if(sliding)
	{
		vals[1] = (mouseP.x-pos.x)*(vals[2]-vals[0])/size.x+vals[0];
		vals[1] = (vals[1] < vals[0]) ? vals[0] : ((vals[1] > vals[2]) ? vals[2] : vals[1]);
	}
}
// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)
vec3i RGBtoHSV(vec3i RGB)
{
	float r = RGB.x;
	float g = RGB.y;
	float b = RGB.z;
	float h = 0;
	float s = 0;
	float v = 0;
	float min, max, delta;
	min = fmin( r, fmin(g, b) );
	max = fmax( r, fmax(g, b) );
	v = max;				// v
	delta = max - min;
	if( max != 0 )
		s = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		s = 0;
		h = -1;
		return {(int)h, (int)(s*255), (int)(v*255)};
	}
	if( r == max )
		h = ( g - b ) / delta;		// between yellow & magenta
	else if( g == max )
		h = 2 + ( b - r ) / delta;	// between cyan & yellow
	else
		h = 4 + ( r - g ) / delta;	// between magenta & cyan
	h *= 60;				// degrees
	if( h < 0 )
		h += 360;
	
	return {(int)h, (int)(s*255), (int)(v*255)};
}
vec3i HSVtoRGB(vec3i HSV)
{
	float r = 0;
	float g = 0;
	float b = 0;
	int i;
	float f, p, q, t;
	float h = HSV.x;
	float s = HSV.y;
	float v = HSV.z;
	h /= 60;			// sector 0 to 5
	s /= 255;
	v /= 255;
	if( s == 0 ) {
		// achromatic (grey)
		r = g = b = v;
		return {(int)(r*255),(int)(g*255),(int)(b*255)};
	}
	
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
		case 0:
			r = v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = v;
			b = p;
			break;
		case 2:
			r = p;
			g = v;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = v;
			break;
		case 4:
			r = t;
			g = p;
			b = v;
			break;
		default:		// case 5:
			r = v;
			g = p;
			b = q;
			break;
	}
	
	return {(int)(r*255),(int)(g*255),(int)(b*255)};
}


GUIcolorbox::GUIcolorbox():GUIbase()
{
	movable = false;
	resizible = false;
	size = {256,256};
	colLU = {0,255,255};//hue,saturation,value
	colRU = {360,255,255};
	colLD = {0,0,255};
	colRD = {360,0,255};
	colorHSV = {0,0,0};
	colorRGB = {0,0,0};
	cursor = {-1,-1};
}
void GUIcolorbox::onClick(int mb)
{
	if(mb>0)
	{
		cursor = mouseP-pos;
		colorHSV = (colLU*(size.x-cursor.x)/size.x+colRU*(cursor.x)/size.x)*(size.y-cursor.y)/size.y +
					(colLD*(size.x-cursor.x)/size.x+colRD*(cursor.x)/size.x)*(cursor.y)/size.y;
		colorRGB = HSVtoRGB(colorHSV);
	}
}
void GUIcolorbox::render(void *arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	setColor(color_border);
	paintRectOutline(pos.x-1,pos.y-1,pos.x+size.x+1,pos.y+size.y+1);
	
	glBegin(GL_LINES);
	int X = pos.x;
	float E = size.x;
	int Yi = pos.y;
	int Yf = pos.y+size.y;
	vec3i col = {0,0,0};
	for(float I = 0; I<E;I++)
	{
		col = HSVtoRGB((colLU*(E-I))/E+(colRU*I)/E);
		glColor3f(col.x/255.0f,col.y/255.0f,col.z/255.0f);
		glVertex2i(X, Yi);
		col = HSVtoRGB((colLD*(E-I))/E+(colRD*I)/E);
		//col = HSVtoRGB({(int)(360*I/E),255,255});
		glColor3f(col.x/255.0f,col.y/255.0f,col.z/255.0f);
		glVertex2i(X, Yf);
		X++;
	}
	glEnd();
	
	if(cursor.x>=0)
	{
		//setColor(colorRGB);
		//paintRectOutline(pos.x+cursor.x-1,pos.y+cursor.y-1,pos.x+cursor.x+1,pos.y+cursor.y+1);
		
		setColor(color_border);
		paintRectOutline(pos.x+cursor.x-2,pos.y+cursor.y-2,pos.x+cursor.x+2,pos.y+cursor.y+2);
	}
	glDisable(GL_SCISSOR_TEST);
}


GUIvaluedisplay::GUIvaluedisplay():GUIbase()
{
	size = {128,32};
	resizible = 0;
	precision = 2;
	movable = 0;
	val = NULL;
	mode = 'd';
}
void GUIvaluedisplay::render(void *arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setColor(color_text);
	if(val!=NULL)
	{
	switch(mode)
	{
		case 'd':
			sprintf(str,"%.*d",precision,*((int*)val));
			break;
		case 'f':
			sprintf(str,"%.*f",precision,*((double*)val));
			break;
		case 'c':
			sprintf(str,"%c",*((char*)val));
			break;
		case 's':
			sprintf(str,"%s",*((char**)val));
			break;
		case 'p':
			sprintf(str,"%p",*((void**)val));
			break;
		default:
			sprintf(str,"val:[%p]",val);
			break;
	}
	}else{sprintf(str,"val:[%p]",val);}
	printw(pos.x,pos.y,pos.x+size.x,pos.y+size.y,str);
	
	glDisable(GL_SCISSOR_TEST);
}



GUIImage::GUIImage():GUIbase()
{
	movable = false;
	resizible = false;
	size = {256,256};
	ImageTex = 0;
}
void GUIImage::setImage(char *path)
{
	//ImageTex = LoadTextureRAW(path, 1);
	ImageTex = GenTextureBMP(path).t;
}
void GUIImage::render(void *arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x+1,pos.y+size.y+1);
	
	//draw

	paintTexturedRect(pos.x,pos.y+1,pos.x+size.x,pos.y+size.y+1, ImageTex);
	//int X = pos.x;
	//float E = size.x;
	//int Yi = pos.y;
	//int Yf = pos.y+size.y;
	//vec3i col = {0,0,0};
	
	glDisable(GL_SCISSOR_TEST);
}


GUIscrollslidey::GUIscrollslidey():GUIbase()
{
	callback = NULL;
	arg = NULL;
	movable = true;
	vertical = true;
}
void GUIscrollslidey::onClick(int mb)
{
	if(mb==1)
	{
		startTouch = mouseP-pos;
		if(callback)(callback(arg));
		printf("clicky.");
		if(movable){dragging = 1;}
	}
	else{dragging = 0;}
}
void GUIscrollslidey::dragCheck()
{
	
	if(dragging)
	{
		vec2i newPos = mouseP-startTouch;
		if(vertical){newPos.x = pos.x;}else{newPos.y=pos.y;}
		
		if(parent)
		{
			newPos.x = clamp(newPos.x, parent->pos.x, parent->pos.x+parent->size.x-size.x);
			newPos.y = clamp(newPos.y, parent->pos.y, parent->pos.y+parent->size.y-size.y);
		}
		if(newPos!=pos)
		{
			invalidate(newPos, size);
			callback(arg);
		}	
	}
}
void GUIscrollslidey::moveupdown(int dist)
{
	vec2i newPos = {pos.x+dist, pos.y+dist};
	if(vertical){newPos.x = pos.x;}else{newPos.y=pos.y;}
	
	if(parent)
	{
		newPos.x = clamp(newPos.x, parent->pos.x, parent->pos.x+parent->size.x-size.x);
		newPos.y = clamp(newPos.y, parent->pos.y, parent->pos.y+parent->size.y-size.y);
	}
	if(newPos!=pos)
	{
		invalidate(newPos, size);
		callback(arg);
	}	
}
void GUIscrollslidey::render(void *arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	
	setColor(color_panel);
	if(mouseOver){setAlpha(255);}else{setAlpha(64);}
	paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setAlpha(255);
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	
	glDisable(GL_SCISSOR_TEST);
}
/*
void invalidate(vec2i newPos, vec2i newSize)
{
	size = newSize;
	if(parent)
	{
		newPos.x = clamp(newPos.x, parent->pos.x, parent->pos.x+parent->size.x-size.x);
		newPos.y = clamp(newPos.y, parent->pos.y, parent->pos.y+parent->size.y-size.y);
	}
	if(newPos!=pos)
	{
		pos = newPos;
		callback(arg);
	}
}
*/


void GUIscrollBar::btnupFunc(void *arg){
	GUIscrollBar* S = (GUIscrollBar*)arg;
	S->slidey->moveupdown(-S->slidey->size.y/2);
}
void GUIscrollBar::btndnFunc(void *arg){
	GUIscrollBar* S = (GUIscrollBar*)arg;
	S->slidey->moveupdown(S->slidey->size.y/2);
}
void GUIscrollBar::updateBar(void *arg)
{
	GUIscrollBar* S = (GUIscrollBar*)arg;
	
	if(S->vertical)
	{
		S->Amin = ((float)(S->slidey->pos.y-S->track->pos.y))/((float)(S->track->size.y));
		//S->Amax = ((float)(S->slidey->pos.y-S->track->pos.y+S->slidey->size.y+1))/((float)(S->track->size.y));
	}
	printf("Scroll: %f to %f ", S->Amin,S->Amax);
	foreach((GUIbase*)arg,&propagateScroll,arg,0);//'this' and 'arg' are NOT the same down the line.
	printf(" *\n");
}
int GUIscrollBar::propagateScroll(GUIbase* obj, void* arg, int rec)
{
	GUIscrollBar *S = (GUIscrollBar*)arg;
	if((obj!=S->btnup)&&(obj!=S->btndn)&&(obj!=S->track))
	{
		vec2i newPos = obj->pos;
		
		if(S->vertical)
		{
			newPos.y = newPos.y-S->Aoffset; 
			S->Aoffset = -(S->Amin)*S->insideSize.y;
			//S->Aoffset = -(S->Amin/S->Amax)*S->insideSize.y;
			newPos.y = newPos.y+S->Aoffset;
		}
		else
		{
			newPos.x = newPos.x-S->Aoffset;
			S->Aoffset = -(S->Amin/S->Amax)*S->insideSize.x;
			newPos.x = newPos.x+S->Aoffset;
		}
		obj->setPos(newPos.x,newPos.y);
	}
}
void GUIscrollBar::setSize(int x, int y)
{
	vec2i newSize = {x,y};
	if(parent){sizeOff = parent->size-newSize;}
	invalidate(pos, newSize);
	printf("newSize = {%d, %d}\n", newSize.x, newSize.y);
}
GUIscrollBar::GUIscrollBar():GUIbase()
{
	movable = false;
	resizible = false;
	size = {128,128};
	insideSize = size;
	insideSize.y += size.y;
	sizeOff = {0,0};
	vertical = true;
	Amin = 0.0;
	Amax = 0.5;
	Aoffset = 0;
	btnup = new GUIbutton;
	btnup->setSize(16,16);
	btnup->text = "^";
	btnup->scissor = false;
	btnup->func = btnupFunc;
	btnup->arg = (void*)this;
	
	btndn = new GUIbutton;
	btndn->setSize(16,16);
	btndn->text = "v";
	btndn->func = btndnFunc;
	btndn->arg = (void*)this;
	btndn->scissor = false; //for the future - use intrinsic children who use parent's scissor rect.
							//or, make "false scissor" do that...
	track = new GUIbutton;
	track->scissor = false;
	color3i darken = {-64,-64,-64};
	track->color_panel.r *= 3;// + darken;
	track->color_panel.g *= 3;// + darken;
	track->color_panel.b *= 3;// + darken;
	track->color_panel.r /= 4;// + darken;
	track->color_panel.g /= 4;// + darken;
	track->color_panel.b /= 4;// + darken;
	
	
	slidey = new GUIscrollslidey;
	slidey->setSize(16,32);
	slidey->scissor = false;
	slidey->callback = &updateBar;
	slidey->arg = (void*)this;
	
	btnup->setParent(this);
	btndn->setParent(this);
	track->setParent(this);
	slidey->setParent(track);
	slidey->setPos(track->pos.x,track->pos.y);
}
void GUIscrollBar::setParent(GUIbase* obj)
{
	parent = obj;
	parent->recalculateClientRect();
	sizeOff = parent->size-size;
	printf("1 Psize = %d, size = %d, sizeOff = %d\n",parent->size.x, size.x, sizeOff.x);
	invalidate(pos+(vec2i){parent->crect.x1,parent->crect.y1}, size);
	GUIbase::addChild(parent, this);
	printf("2 Psize = %d, size = %d, sizeOff = %d\n",parent->size.x, size.x, sizeOff.x);
}
void GUIscrollBar::invalidate(vec2i newPos, vec2i newSize)
{
	//this one should also reposition children and stuff
	vec2i sendPos = newPos-pos;//+newSize-size; and guess what happens when x is parented to y of size {0,0}?
	
	
	pos = newPos;
	recalculateClientRect();
	
	
	
	//size = newSize; //don't transmit size to chilren (use pack[2] if needed)
	
	if(parent&&(newSize==size)){size = parent->size-sizeOff;}
	else{size = newSize;}
	
	foreach(this, &wrapInvalidate, (void*)(&sendPos), 0);
	
	if(vertical)
	{
		btnup->setPos(pos.x+size.x-16,pos.y); //upper right
		btndn->setPos(pos.x+size.x-16,pos.y+size.y-15); //bottom right
		track->setSize(16,size.y-33);		
		track->setPos(pos.x+size.x-16,pos.y+17);
		//printf("tracksize = %d, size = %d, insideSize = %d, result = %d", track->size.y, size.y, insideSize.y,track->size.y*(size.y/insideSize.y));
		slidey->setSize(16,track->size.y*((float)(size.y-4)/(float)(insideSize.y)));//4 for border
	}
	else
	{
		btnup->setPos(pos.x,pos.y+size.y+2); //bottom left
		btndn->setPos(pos.x+size.x-15,pos.y+size.y+2); //bottom right
		track->setSize(size.x-33,16);		
		track->setPos(pos.x+18,pos.y+size.y+2);
		slidey->setSize(16,track->size.y*(size.y/insideSize.y));
	}
	
}
void GUIscrollBar::render(void *arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x+1,pos.y+size.y+1);
	
	//draw

	//int X = pos.x;
	//float E = size.x;
	//int Yi = pos.y;
	//int Yf = pos.y+size.y;
	//vec3i col = {0,0,0};
	
	glDisable(GL_SCISSOR_TEST);
}
/*
GUI element check list!
--GENERAL--
FIX STRATA! 
SizeToContents()
vec2i = printw
--INPUT--
Button		*
Checkbox	*
Radiobutton *
TextEntry	* (sorta)
Spinner		*
DropDownList*
ListBox		*
Slider		*
RoundSlider
ColorWheel
ColorBox	*
--NAVIGATION--
Frame		*
ScrollBar	*
Tab
--OUTPUT--
Label		*
Image		*
--WINDOWS--
Messagebox
valueDisplay*
Console
--debug--
linked list browser
ideas:
move scissor checks into base class
false scissor inherents parent rect
OR extra list of children exempt from client rect
bool clickable
callbacks for everything
dock left/right/up/down both moves and resizes element
size-to-contents and min-size.
vals = {1,2,3} should be in declaration but unimplemented by gcc.
*/


