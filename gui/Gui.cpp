#include <string>
#include "Gui.h"
#include "../util/globals.h"
#include "../display/paint.h"
#include "../resource/fonts.h"

//using std::string;
/*
render tree
root node:                                 root
children render queue:          ch1       ch2   ch3   ch4
grandchildren render queue: ch1.1 ch1.2

printw - newline, limit width/height



*/
/*
void paintRect(int X1, int Y1, int X2, int Y2)
{
    glBegin(GL_QUADS);
    glVertex2i(X1, Y1);
    glVertex2i(X1, Y2);
    glVertex2i(X2, Y2);
    glVertex2i(X2, Y1);
    glEnd();
}

void paintRectOutline(int X1, int Y1, int X2, int Y2)
{
    float x1 = X1;// /width-1.0f;
    float x2 = X2;// /width-1.0f;
    float y1 = Y1;// /height-1.0f;
    float y2 = Y2;// /height-1.0f;
    glBegin(GL_LINES);

    glVertex2f(x1, y1);
    glVertex2f(x2, y1);

    glVertex2f(x2, y1);
    glVertex2f(x2, y2);

    glVertex2f(x2, y2);
    glVertex2f(x1, y2);


    glVertex2f(x1, y2);
    glVertex2f(x1, y1);

    glEnd();
}
*/
/*
enum GUIMessage
{
    GUIM_RENDER,
    GUIM_CHECK_MOUSEOVER,
    GUIM_CLICK,
    GUIM_KEYBOARD,
    GUIM_DESTROY
};
*/

GUI1obj::GUI1obj()
{
counter = 0;
children.next = NULL;
children.thing = NULL;
parent = NULL;
pos.x = 30;
pos.y = 30;
size.x = 32;
size.y = 32;
color.r = 32;
color.g = 255;
color.b = 32;
color2 = {0,0,0};
strata = 0;
}
/*
int ParseMessage(GUIMessage msg, int param, bool propagate)
{
	if(propagate)
	{
		listNode* Cur = &children;
		while(Cur!=NULL)
		{
			if(Cur->thing!=NULL)
			{
				((GUIobj*)(Cur->thing))->ParseMessage(msg, param, propagate);
			}
			Cur = Cur->next;
		}
	}
	switch(msg)
	{
		case GUIM_RENDER:
			render();
		break;
		default:
		break;
	}
	return 0;
}
*/
void GUI1obj::render()
{
	int renderX1 = pos.x;
	int renderY1 = pos.y;
	int renderX2 = pos.x+size.x;
	int renderY2 = pos.y+size.y;
	
	glColor3f(color.r/255.0f,color.g/255.0f, color.b/255.0f);
	
	if(mouseOver)
		{
			paintRect(renderX1, renderY1, renderX2, renderY2);
		}
	else
		{
			paintRectOutline(renderX1, renderY1, renderX2, renderY2);
			//if(Calibri20n2 == 0){Calibri20n2 = GenerateFont("C:/Stride/calibri.ttf", 20);}
			//setFont(Calibri20);
			printw(renderX1, renderY1+size.y/2, size.x, size.y, "Button");
		}
	printw(renderX2+8, renderY1+size.y/2, size.x, size.y, "Clicks: %d",counter);
}
void GUI1obj::onClick(int mb)
{
	counter++;
}
GUI1obj::~GUI1obj()
{
}
void GUI1obj::keyDown(int kb)
{
}
void GUI1obj::keyUp(int kb)
{
}
void GUI1obj::invalidate()
{
}




GUI1Manager::GUI1Manager()
{
}
void GUI1Manager::activate(GUI1obj* it) //inserts control into render queue
{                       //GUIobj* parent
	listNode* Cur;
	if(it->parent){Cur = &(it->parent->children);}else{Cur = &Root;}
	bool done = false;
	int strata = 0;
	while(!done)
	{
		if(Cur->thing==NULL)
		{
			Cur->thing=(void*)it; 
			it->strata = strata; 
			it->GUIM = (void*)this;
			it->invalidate();
			done = 1;
		}
		else if(Cur->next==NULL)
		{
			Cur->next = new listNode;
			Cur->next->next = NULL;
			it->strata = strata+1;
			it->GUIM = (void*)this;
			it->invalidate();
			//if(it->parent){reposition(it,{0,0}, it->parent->pos);}
			Cur->next->thing = (void*)it;
			done = 1;
		}
		else{Cur = Cur->next;strata++;}
	}
}
void GUI1Manager::remove(GUI1obj* curObj, GUI1obj* obj) //removes control from render queue
{
	bool done = false;
	listNode *Cur;
	void* objPtr = (void*)obj;
	if(curObj){Cur = &(curObj->children);}
	else{Cur = &Root;}
	while(Cur!=NULL)
	{
		if(Cur->thing==objPtr){Cur->thing=NULL;done = true;}
		Cur=Cur->next;
	}
	if(!done)	//if we didn't find the deletee on this tier, check all children recursively.
	{
		if(curObj){Cur = &(curObj->children);}
		else{Cur = &Root;}
		while(Cur!=NULL)
		{
			if(Cur->thing){remove(((GUI1obj*)Cur->thing),obj);}
			Cur=Cur->next;
		}
	}
}
void GUI1Manager::axe(GUI1obj* obj) // recursively deletes all object's descendants.
{					  // (apparently wiping root is a bad idea...)
						// BLUH
	/*
	listNode *Cur;
	listNode *old;
	Cur = &Root;//if(obj){Cur = &(obj->children);}else{Cur = &Root;}
	
	while(Cur!=NULL)
	{
		//if(Cur->thing){axe((GUIobj*)Cur->thing);}
		old = Cur;
		Cur = Cur->next;
		if(old->thing)
		{
		if(old->thing==(void*)lastClick){lastClick = NULL;}
		if(old->thing==(void*)focus){focus = NULL;}
		//if(old->thing!=(void*)me){delete (GUIobj*)old->thing;}
		}
		
		if(old==&Root)
			{
				old->next = NULL;
				delete ((GUIobj*)old->thing);
			}
		else
			{delete old;} // wipe the linked list.
		
	}*/
	
}
void GUI1Manager::render(GUI1obj* obj)
{
	listNode* Cur;
	if(obj){Cur = &(obj->children);}else{Cur = &Root;}
	while(Cur!=NULL)
	{
		if(Cur->thing!=NULL)
		{
			((GUI1obj*)(Cur->thing))->render();
			this->render((GUI1obj*)(Cur->thing));
		}
		Cur = Cur->next;
	}
}
bool GUI1Manager::checkMouseOver(GUI1obj* obj, int x, int y)
{
	// only one control can have mouse-over and it's always the top one.
	listNode* Cur;
	GUI1obj *obj2, *obj3;
	obj3 = NULL;
	int x1,x2,y1,y2;
	int strata = 0;
	if(obj == NULL){Cur = &Root;}
	else{Cur = &(obj->children);}
		
	while(Cur!=NULL)
	{
		if(Cur->thing!=NULL)
		{
			
			obj2 = (GUI1obj*)(Cur->thing);
			//reset mouse-over values for all of them
			obj2->mouseOver = 0;
			x1 = obj2->pos.x;
			y1 = obj2->pos.y;
			x2 = x1+obj2->size.x;
			y2 = y1+obj2->size.y;
								
			if((x1<=x)&&(x2>=x)&&(y1<=y)&&(y2>=y))
			{
				if(obj3!=NULL){if(obj3->strata<=strata){obj3 = obj2;}}else{obj3 = obj2;}
			}
			//max strata check here.
			//this->checkMouseOver(obj2, x, y);
		}
		
		Cur = Cur->next;
		strata++;
	}
	//obj3 is now confirmed for maximum strata of current tier mouse-over-ables. Check it's chilren.
		
	if(obj3!=NULL)
	{
		bool found = this->checkMouseOver(obj3,x,y);
		obj3->mouseOver = !found;
		return true;
	}
	return false;
}
void GUI1Manager::reposition(GUI1obj* obj, vec2i pos1, vec2i pos2)
{
	obj->pos = obj->pos+pos2-pos1;
	listNode* Cur = &(obj->children);
	while(Cur!=NULL)
	{
		if(Cur->thing){reposition((GUI1obj*)Cur->thing,pos1,pos2);}
		Cur = Cur->next;
	}
}
void GUI1Manager::click(GUI1obj* obj, int mb)
{
	if(mb==1)
	{
		if(lastClick){lastClick->onClick(mb);lastClick = NULL;}
	}
	else
	{
	listNode* Cur;
	GUI1obj* obj2;
	
	if(obj == NULL){Cur = &Root;}
	else{Cur = &(obj->children);}
		
	while(Cur!=NULL)
	{
		if(Cur->thing!=NULL)
		{
			obj2 = (GUI1obj*)(Cur->thing);
			if(obj2->mouseOver){obj2->onClick(mb);lastClick = obj2;}		
			this->click(obj2, mb);
		}
		Cur = Cur->next;
	}
	}
}
void GUI1Manager::keyboard(int kb)
{
	if(focus)
	{
		if(kb>0){focus->keyDown(kb);}
		else{focus->keyUp(kb);}
	}
}
void GUI1Manager::input()
{

}

GUI1button::GUI1button():GUI1obj()
{
	//func = NULL;
	text = "X";
	textcolor = {0,0,0};
}
void GUI1button::onClick(int mb)
{
	if(mb){pressed = 1;}
	else
	{
		if(pressed)
		{
			pressed = 0;
			if(func&&funcHolder)
			{func(funcHolder);}
		}
	}
}
void GUI1button::render()
{
	if(!pressed)
	{
			glColor3f(color.r/255.0f,color.g/255.0f,color.b/255.0f);
			paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
		if(mouseOver)
		{
			glColor3f(color2.r/255.0f,color2.g/255.0f,color2.b/255.0f);
			paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
		}
		glColor3f(textcolor.r/255.0f,textcolor.g/255.0f,textcolor.b/255.0f);
		printw(pos.x+4,pos.y+4, size.x, size.y,text);
	}
	else
	{
		if(!mouseOver){pressed = 0;}
		glColor3f(color2.r/255.0f,color2.g/255.0f,color2.b/255.0f);
		paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
		glColor3f(textcolor.r/255.0f,textcolor.g/255.0f,textcolor.b/255.0f);
		printw(pos.x+4,pos.y+4, size.x, size.y,text);
	}
}
/*
color3i paintColor;
byte paintAlpha;
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
*/

//class GUIframe: public GUIobj

void GUI1frame::btnClose_wrapper(void* me) //wtf are these shenanigans
{									   //upper one actually gets sent
	((GUI1frame*)me)->btnClose();	   //and used for onClick(mb)
}

void GUI1frame::btnClose()
{
	delete this;
}										//:|

GUI1frame::GUI1frame():GUI1obj()
{
	dragging = 0;
	dragStart = {0,0};
	pos = {0,0};
	oldpos = {0,0};
	myButton = new GUI1button;
	myButton->size = {24,24};
	myButton->parent = (GUI1obj*)this;//commenting this makes me crash?
	myButton->func = &btnClose_wrapper;
	myButton->funcHolder = (void*)this;
	((GUI1Manager*)GUIM)->activate((GUI1obj*)myButton); //this button gets positioned to the moon. FIND IT!
}
GUI1frame::~GUI1frame()
{
	//delete myButton;//uhh do I need to?...
	((GUI1Manager*)GUIM)->remove(NULL, (GUI1obj*)this);
	((GUI1Manager*)GUIM)->axe((GUI1obj*)this);
	//delete this; NEVER do this in a destructor
}
void GUI1frame::invalidate()
{
	myButton->pos = {size.x-26,2};
	myButton->color = color+(color3i){32,32,32};
	myButton->color2 = color2;
}
void GUI1frame::onClick(int mb)
{
	if(mb)
	{
		dragStart = mousePos-pos;
		dragging = 1;
	}
	else
	{
		dragging = 0;
	}
}
void GUI1frame::render() // later will be delegated to skins to draw on their own
{
	if(dragging){pos = mousePos-dragStart;}
	if(!(oldpos==pos))
	{
		vec2i pos2 = pos;
		pos = oldpos;
		((GUI1Manager*)GUIM)->reposition(this, oldpos, pos2);
		oldpos = pos;
	}
	int rX1 = pos.x;
	int rY1 = pos.y;
	int rX2 = pos.x+size.x;
	int rY2 = pos.y+size.y;
	
	//window
	glColor4f(color.r/255.0f,color.g/255.0f,color.b/255.0f,0.5f);
	paintRect(rX1,rY1,rX2,rY2);
	glColor4f(color2.r/255.0f,color2.g/255.0f,color2.b/255.0f,1.0f);
	paintRectOutline(rX1,rY1,rX2,rY2);
	//title bar
	glColor4f(color.r/255.0f,color.g/255.0f,color.b/255.0f,1.0f);
	paintRect(rX1+2, rY1+2, rX2-2, rY1+26);
	glColor4f(color2.r/255.0f,color2.g/255.0f,color2.b/255.0f,1.0f);
	//setFont(Tahoma22);
	printw(rX1+4, rY1+5,  size.x, size.y,"Title");//todo: centering func
	
	//printw(rX2+8, rY1+size.y/2, "Clicks: %d",counter);
}

GUI1textEntry::GUI1textEntry():GUI1obj()
{
	textColor = {0,0,0};
	text = "All the pretty little horses";
	//font = Calibri18;
	color = {255,255,255};
	color2 = {0,0,0};
}
void GUI1textEntry::render()
{
	setColor(color);
	paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	
	setColor(color2);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	
	setColor(textColor);
	//setFont(font);
	printw(pos.x+4,pos.y+4, size.x, size.y, text);
}
void GUI1textEntry::onClick(int mb)
{
	hasFocus = true;
	((GUI1Manager*)GUIM)->focus = (GUI1obj*)this;
}
void GUI1textEntry::keyDown(int kb)
{
	if(hasFocus)
	{
		text+=(char)kb;
	}
}

