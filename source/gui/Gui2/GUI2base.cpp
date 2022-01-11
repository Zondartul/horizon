#include "gui/Gui2/GUI2base.h"
#include "main/control.h"
#include "display/paint.h"
#include "resource/fonts.h"
GUI2base::GUI2base(){
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

void GUI2base::setPos(int x, int y){
		invalidate((vec2i){x,y},size);
		setdock(dockup,dockdown,dockleft,dockright);
}
void GUI2base::setSize(int x, int y)
{
	invalidate(pos, (vec2i){x,y});
	setdock(dockup,dockdown,dockleft,dockright);
}
void GUI2base::setdock(bool up, bool down, bool left, bool right)
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

void GUI2base::dockCheck()
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
void GUI2base::onClick(int mb)
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
void GUI2base::onKeyboard(string kb)
{
}
void GUI2base::PSreceive(message msg){
	if(msg.type == "key_down" || msg.type == "key_still_down"){
		propagateKeyboard(msg.str);
	}
}
//void onKeyboard();
void GUI2base::invalidate(vec2i newPos, vec2i newSize)
{
	//this one should also reposition children and stuff
	vec2i sendPos = newPos-pos;//+newSize-size; and guess what happens when x is parented to y of size {0,0}?
	
	
	pos = newPos;
	recalculateClientRect();
	
	size = newSize; //don't transmit size to chilren (use pack[2] if needed)
	dockCheck();
	foreach(this, &wrapInvalidate, (void*)(&sendPos), 0);
}
void GUI2base::recalculateClientRect()
{
	crect.x1 = pos.x+border;
	crect.y1 = pos.y+border;
	crect.x2 = pos.x+size.x-border;
	crect.y2 = pos.y+size.y-border;
}
void GUI2base::resizeCheck()
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
void GUI2base::dragCheck()
{
	if(dragging)
	{
		invalidate(mouseP-startTouch, size);
	}
}
void GUI2base::scissorCheck(void* arg)
{
	if(scissor)
	{
		vec4i scissor = *((vec4i*)arg);
		if(!(scissor == (vec4i){0,0,0,0})){glEnable(GL_SCISSOR_TEST);}
		glScissor(scissor.x1-1,height-scissor.y2-1,scissor.x2-scissor.x1+1,scissor.y2-scissor.y1+1);
	}
}
void GUI2base::render(void* arg)
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

bool GUI2base::foreach(GUI2base* obj, int (*func)(GUI2base*, void*, int), void* arg, int rec) 
{
	//calls func (3rd argument) upon all filled or all (arg2) children nodes of obj(arg1)
	// i couldn't think of saner way. sorry.
	// rec is recursion counter for debugging.
	int stop = false;
	listNode* Cur = obj->children;
	while(Cur!=NULL)
	{
		if(Cur->thing){stop = stop+func((GUI2base*)Cur->thing, arg, rec+1);}
		Cur = Cur->next;
		if(stop==1){Cur=NULL;}
	}
	return stop;
}

bool GUI2base::foreachbackwards_check(listNode* Cur, int (*func)(GUI2base*, void*, int), void* arg, int rec)
{
	bool stop = 0;
	if(Cur->next){stop = foreachbackwards_check(Cur->next, func, arg, rec+1);}
	if(Cur->thing){return func((GUI2base*)Cur->thing, arg, rec+1)||stop;}
	else return stop;
}
bool GUI2base::foreachbackwards(GUI2base* obj, int (*func)(GUI2base*, void*, int), void* arg, int rec) 
{
	//calls func (3rd argument) upon all filled or all (arg2) children nodes of obj(arg1)
	// i couldn't think of saner way. sorry.
	// rec is recursion counter for debugging.
	listNode* Cur = obj->children;
	bool stop = 0;
	if(Cur->next){stop = foreachbackwards_check(Cur->next, func, arg, rec+1);}
	if(Cur->thing){return func((GUI2base*)Cur->thing, arg, rec+1)||stop;}
	else return stop;
}
void GUI2base::setParent(GUI2base* obj)
{
	parent = obj;
	parent->recalculateClientRect();
	invalidate(pos+(vec2i){parent->crect.x1,parent->crect.y1}, size);
	GUI2base::addChild(parent, this);
}
GUI2base* GUI2base::findByTag(string tag)
{
	listNode* Cur = children;
	while(Cur!=NULL)
	{
		if(Cur->thing)
		{
			if(((GUI2base*)(Cur->thing))->tag==tag){return ((GUI2base*)(Cur->thing));}
		}
		Cur = Cur->next;
	}
	return NULL;
}
void GUI2base::addChild(GUI2base* parent, GUI2base* obj)
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
int GUI2base::wrapInvalidate(GUI2base* obj, void* arg, int rec)
{
	obj->invalidate(*((vec2i*)arg)+obj->pos,obj->size);
}
int GUI2base::propagateMouseOver(GUI2base* obj, void* arg, int rec)
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
int GUI2base::propagateKeyboard(string kb)
{
	if(focus!=NULL){focus->onKeyboard(kb);return 1;}else{return 0;}
}
int GUI2base::propagateClick(GUI2base* obj, void* arg, int rec)
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
void GUI2base::fixstrata(GUI2base* obj)
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
int GUI2base::propagateRender(GUI2base* obj, void* arg, int rec)
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
GUI2base::~GUI2base()
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