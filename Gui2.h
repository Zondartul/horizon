//Gui.h
#include <string>
using std::string;
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

class GUIbase
{
	public:
	listNode* children;
	GUIbase* parent;
	vec2i pos;
	vec2i size;
	vec4i crect; //client area rect
	int strata; //how much is window above others.
	color3i color_border;
	color3i color_panel;
	color3i color_text;
	bool mouseOver;
	bool kbFocus;
	bool resizible;
	bool movable;
	bool visible;
	int resizingW;
	int resizingH;
	bool dragging;
	vec2i *mousePtr;
	vec2i startTouch; //place of mouse click
	int counter; //mostly useless
	
	GUIbase()
	{
		pos = size = {0,0};
		crect = {0,0,0,0};
		color_border = color_text = {0,0,0};
		color_panel = {255,255,255};
		strata = 0;
		children = new listNode;
		children->thing = NULL;
		children->next = NULL;
		parent = NULL;
		mouseOver = false;
		kbFocus = false;
		resizible = false;
		movable = false;
		visible = false;
		mousePtr = NULL;
		resizingW = 0;
		resizingH = 0;
		dragging = false;
		startTouch = {0,0};
		counter = 0;
	}
	void onClick(int mb)
	{
		if(mb>0)
		{
			startTouch = *mousePtr-pos; //has one?
			if(mousePtr->x == pos.x){resizingW = 1;}
			if(mousePtr->x == pos.x+size.x){resizingW = 2;}
			if(mousePtr->y == pos.y){resizingH = 1;}
			if(mousePtr->y == pos.y+size.y){resizingH = 2;}
			if(!resizingW&&!resizingH){dragging = 1;}
		}
		else //what if there was no first click?
		{
			resizingW = 0; resizingH = 0; dragging = 0;
		}
		counter++;
	}
	void onKeyboard();
	void invalidate(vec2i newPos, vec2i newSize)
	{
		//this one should also reposition children and stuff
		vec2i sendPos = newPos-pos+newSize-size;
		recalculateClientRect();
		
		pos = newPos;
		
		size = newSize; //don't transmit size to chilren (use pack[2] if needed)
		foreach(this, &wrapInvalidate, (void*)(&sendPos), 0);
	}
	virtual void recalculateClientRect()
	{
		crect.x1 = pos.x;
		crect.y1 = pos.y;
		crect.x2 = pos.x+size.x;
		crect.y2 = pos.y+size.y;
	}
	void resizeCheck()
	{
		if(!mousePtr){return;}
		vec2i newSize = size;
		vec2i newPos = pos;
		if(resizingW==2)
		{
			newSize.x = (*mousePtr).x-pos.x;
			if(newSize.x<0){newSize.x=1;}
		}
		if(resizingW==1)
		{
			int diff = (*mousePtr).x-pos.x;
			if(diff>size.x){diff=size.x-1;}
			newPos.x+=diff;
			newSize.x-=diff;
		}
		if(resizingH==2)
		{
			newSize.y = (*mousePtr).y-pos.y;
			if(newSize.y<0){newSize.y=1;}
		}
		if(resizingH==1)
		{
			int diff = (*mousePtr).y-pos.y;
			if(diff>size.y){diff=size.y-1;}
			newPos.y+=diff;
			newSize.y-=diff;
		}
		invalidate(newPos, newSize);
	}
	void dragCheck()
	{
		if(dragging&&mousePtr)
		{
			invalidate(*mousePtr-startTouch, size);
		}
	}
	virtual void render(void* arg)
	{
		resizeCheck();
		dragCheck();
		
		vec4i scissor = *((vec4i*)arg);
		if(!(scissor == (vec4i){0,0,0,0})){glEnable(GL_SCISSOR_TEST);}
		glScissor(scissor.x1-1,height-scissor.y2-1,scissor.x2-scissor.x1+1,scissor.y2-scissor.y1+1);
		
		setColor(color_panel);
		if(!mouseOver){setAlpha(128);}
		paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
		setColor(color_border);
		setAlpha(255);
		paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
		setColor(color_text);
		printw(pos.x, pos.y+size.y/2, "clicks: %d", counter);
		glDisable(GL_SCISSOR_TEST);
	}
	
	static bool foreach(GUIbase* obj, bool (*func)(GUIbase*, void*, int), void* arg, int rec) 
	{
		//calls func (3rd argument) upon all filled or all (arg2) children nodes of obj(arg1)
		// i couldn't think of saner way. sorry.
		// rec is recursion counter for debugging.
		bool stop;
		listNode* Cur = obj->children;
		while(Cur!=NULL)
		{
			if(Cur->thing){stop = func((GUIbase*)Cur->thing, arg, rec+1);}
			Cur = Cur->next;
			if(stop){Cur=NULL;}
		}
		return stop;
	}
	
	void setParent(GUIbase* obj)
	{
		parent = obj;
		pos = pos+(vec2i){parent->crect.x1,parent->crect.y1};
		mousePtr = parent->mousePtr;
		GUIbase::addChild(parent, this);
	}
	static void addChild(GUIbase* parent, GUIbase* obj)
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
	static bool wrapInvalidate(GUIbase* obj, void* arg, int rec)
	{
		obj->invalidate(*((vec2i*)arg)+obj->pos,obj->size);
	}
	static bool propagateMouseOver(GUIbase* obj, void* arg, int rec)
	{
		vec2i mouse = ((vec2i*)arg)[0];
		int x1,x2,y1,y2;
		/*
		x1 = obj->pos.x;
		y1 = obj->pos.y;
		x2 = x1+obj->size.x;
		y2 = y1+obj->size.y;
		*/
		if(obj->visible)
		{
			//scary scissor support
			x1 = max(((vec2i*)arg)[1].x,obj->pos.x);
			y1 = max(((vec2i*)arg)[1].y,obj->pos.y);
			x2 = min(((vec2i*)arg)[2].x,obj->pos.x+obj->size.x);
			y2 = min(((vec2i*)arg)[2].y,obj->pos.y+obj->size.y);
			
			vec2i newarg[3];
			newarg[0] = ((vec2i*)arg)[0];
			newarg[1] = (vec2i){x1,y1};
			newarg[2] = (vec2i){x2,y2};
			
			if(foreach(obj, &propagateMouseOver, (void*)newarg, rec+1)){obj->mouseOver = false; return true;}
			if((x1<=mouse.x)&&(x2>=mouse.x)&&(y1<=mouse.y)&&(y2>=mouse.y))
			{
				obj->mouseOver = true;
				return true;//stop
			}
			else
			{
				obj->mouseOver = false;
				return false;//don't stop
			}
		}
		return false;
	}
	static bool propagateClick(GUIbase* obj, void* arg, int rec)
	{
		static GUIbase* lastClicked; //I hope you won't need to click two things at once.
		int mb = *((int*)arg);
		if(mb>0)
		{
			if(obj->mouseOver){obj->onClick(mb); lastClicked = obj; return true;}
			else{foreach(obj,&propagateClick,arg,rec+1); return false;}
		}
		else //releases are delivered to who you clicked, not current mouseover.
		{
			if(lastClicked){lastClicked->onClick(mb);}
		}
		//its ok to have no return? What?
	}
	static bool propagateRender(GUIbase* obj, void* arg, int rec)
	{
		if(obj->visible)
		{
			obj->render(arg);
			vec4i scissor = *((vec4i*)arg);
			vec4i crect = obj->crect;
			scissor.x1 = max(scissor.x1,crect.x1);
			scissor.y1 = max(scissor.y1,crect.y1);
			scissor.x2 = min(scissor.x2,crect.x2);
			scissor.y2 = min(scissor.y2,crect.y2);
			foreach(obj,&propagateRender, (void*)(&scissor), rec+1);
		}
		return false;
	}
};

class GUIframe: public GUIbase
{
	public:
	string title;
	GUIframe():GUIbase()
	{
		title = "Title";
	}
	void invalidate(vec2i newPos, vec2i newSize)
	{
		
	}
	void render(void* arg)
	{
		resizeCheck();
		dragCheck();
	
		vec4i scissor = *((vec4i*)arg);
		if(!(scissor == (vec4i){0,0,0,0})){glEnable(GL_SCISSOR_TEST);}
		glScissor(scissor.x1-1,height-scissor.y2-1,scissor.x2-scissor.x1+1,scissor.y2-scissor.y1+1);
		
		setColor(color_panel);
		if(!mouseOver){setAlpha(128);}
		paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
		setColor(color_border);
		setAlpha(255);
		paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
		paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+32);
		setColor(color_text);
		printw(pos.x,pos.y+4,title);
		
		glDisable(GL_SCISSOR_TEST);
	}
};