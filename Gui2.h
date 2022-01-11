//Gui.h
//#include <string>
//using std::string;
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
	vec2i cpos; //client area rect
	vec2i csize; //client area size
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
		pos = size = cpos = csize = {0,0};
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
		pos = newPos;
		size = newSize;
	}
	void resizeCheck()
	{
		if(resizingW==2)
		{
			size.x = (*mousePtr).x-pos.x;
			if(size.x<0){size.x=1;}
		}
		if(resizingW==1)
		{
			int diff = (*mousePtr).x-pos.x;
			if(diff>size.x){diff=size.x-1;}
			pos.x+=diff;
			size.x-=diff;
		}
		if(resizingH==2)
		{
			size.y = (*mousePtr).y-pos.y;
			if(size.y<0){size.y=1;}
		}
		if(resizingH==1)
		{
			int diff = (*mousePtr).y-pos.y;
			if(diff>size.y){diff=size.y-1;}
			pos.y+=diff;
			size.y-=diff;
		}
	}
	void dragCheck()
	{
		if(dragging)
		{
			invalidate(*mousePtr-startTouch, size);
		}
	}
	void render()
	{
		resizeCheck();
		dragCheck();
	
		setColor(color_panel);
		if(!mouseOver){setAlpha(128);}
		paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
		setColor(color_border);
		setAlpha(255);
		paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
		setColor(color_text);
		printw(pos.x, pos.y+size.y/2, "clicks: %d", counter);
	}
	
	static void foreach(GUIbase* obj, bool (*func)(GUIbase*, void*, int), void* arg, int rec) 
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
	}
	static bool propagateMouseOver(GUIbase* obj, void* arg, int rec)
	{
		vec2i mouse = *((vec2i*)arg);
		int x1,x2,y1,y2;
		x1 = obj->pos.x;
		y1 = obj->pos.y;
		x2 = x1+obj->size.x;
		y2 = y1+obj->size.y;
		if((obj->visible)&&(x1<=mouse.x)&&(x2>=mouse.x)&&(y1<=mouse.y)&&(y2>=mouse.y))
		{
			obj->mouseOver = true;
			foreach(obj, &propagateMouseOver, arg, rec+1);
			return true;//stop
		}
		else
		{
			obj->mouseOver = false;
			foreach(obj, &propagateMouseOver, arg, rec+1);
			return false;//don't stop
		}
	
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
			obj->render();
			foreach(obj,&propagateRender, arg, rec+1);
		}
		return false;
	}
};