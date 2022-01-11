//Gui.h
#include <string>
#include <cstdio>
#include <math.h>
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
void paintCircle(int X1, int Y1, int Radius)
{
	glBegin(GL_POLYGON);
	for(double I = 0; I<36;I++)
	{
		glVertex2i(X1+Radius*sin(3.1415926535897932384626433832795*I/18), Y1+Radius*cos(3.1415926535897932384626433832795*I/18));
	}
	glEnd();
}
void paintCircleOutline(int X1, int Y1, int Radius)
{
	glBegin(GL_LINE_LOOP);
	for(double I = 0; I<36;I++)
	{
		glVertex2i(X1+Radius*sin(3.1415926535897932384626433832795*I/18), Y1+Radius*cos(3.1415926535897932384626433832795*I/18));
	}
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
	int border;
	bool dragging;
	bool pressed;
	bool scissor;
	vec2i startTouch; //place of mouse click
	int counter; //mostly useless
	
	GUIbase()
	{
		pos = size = {0,0};
		crect = {0,0,0,0};
		color_border = color_text = {0,0,0};
		color_panel = {255,255,255};
		border = 2;
		strata = 0;
		
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
	}
	virtual void setPos(int x, int y)
	{
		invalidate((vec2i){x,y},size);
	}
	virtual void setSize(int x, int y)
	{
		invalidate(pos, (vec2i){x,y});
	}
	virtual void onClick(int mb)
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
	virtual void onKeyboard(int kb)
	{
	}
	//virtual void onKeyboard();
	virtual void invalidate(vec2i newPos, vec2i newSize)
	{
		//this one should also reposition children and stuff
		vec2i sendPos = newPos-pos;//+newSize-size; and guess what happens when x is parented to y of size {0,0}?
		
		
		pos = newPos;
		recalculateClientRect();
		
		size = newSize; //don't transmit size to chilren (use pack[2] if needed)
		foreach(this, &wrapInvalidate, (void*)(&sendPos), 0);
	}
	virtual void recalculateClientRect()
	{
		crect.x1 = pos.x+border;
		crect.y1 = pos.y+border;
		crect.x2 = pos.x+size.x-border;
		crect.y2 = pos.y+size.y-border;
	}
	void resizeCheck()
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
	void dragCheck()
	{
		if(dragging)
		{
			invalidate(mouseP-startTouch, size);
		}
	}
	void scissorCheck(void* arg)
	{
		if(scissor)
		{
			vec4i scissor = *((vec4i*)arg);
			if(!(scissor == (vec4i){0,0,0,0})){glEnable(GL_SCISSOR_TEST);}
			glScissor(scissor.x1-1,height-scissor.y2-1,scissor.x2-scissor.x1+1,scissor.y2-scissor.y1+1);
		}
	}
	virtual void render(void* arg)
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
		printw(pos.x, pos.y+size.y/2, "clicks: %d", counter);
		
		glDisable(GL_SCISSOR_TEST);
	}
	
	static bool foreach(GUIbase* obj, int (*func)(GUIbase*, void*, int), void* arg, int rec) 
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
	
	void setParent(GUIbase* obj)
	{
		parent = obj;
		parent->recalculateClientRect();
		invalidate(pos+(vec2i){parent->crect.x1,parent->crect.y1}, size);
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
	static int wrapInvalidate(GUIbase* obj, void* arg, int rec)
	{
		obj->invalidate(*((vec2i*)arg)+obj->pos,obj->size);
	}
	static int propagateMouseOver(GUIbase* obj, void* arg, int rec)
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
			x1 = max(((vec2i*)arg)[1].x,obj->pos.x);
			y1 = max(((vec2i*)arg)[1].y,obj->pos.y);
			x2 = min(((vec2i*)arg)[2].x,obj->pos.x+obj->size.x);
			y2 = min(((vec2i*)arg)[2].y,obj->pos.y+obj->size.y);
			
			int x1c,x2c,y1c,y2c;
			x1c = max(((vec2i*)arg)[1].x,obj->crect.x1);
			y1c = max(((vec2i*)arg)[1].y,obj->crect.y1);
			x2c = min(((vec2i*)arg)[2].x,obj->crect.x2);
			y2c = min(((vec2i*)arg)[2].y,obj->crect.y2);
			
			vec2i newarg[3];
			newarg[0] = ((vec2i*)arg)[0];
			newarg[1] = (vec2i){x1c,y1c};
			newarg[2] = (vec2i){x2c,y2c};
			
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
				return 2;//found but continue
			}
			else
			{
				obj->mouseOver = false;
				return 0;//don't stop
			}
		}
		return false;
	}
	static GUIbase* lastClicked; //I hope you won't need to click two things at once.
	static GUIbase* focus;
	static int propagateKeyboard(int kb)
	{
		if(focus){focus->onKeyboard(kb);}
	}
	static int propagateClick(GUIbase* obj, void* arg, int rec)
	{
		int mb = *((int*)arg);
		if(mb>0)
		{
			if(obj->mouseOver){obj->onClick(mb); lastClicked = obj; focus = obj; return true;}
			else
			{
				if(foreach(obj,&propagateClick,arg,rec+1)==false)
				{
					if(rec==0){lastClicked=NULL; focus = NULL;}
					return false;
				} else {return true;}
			}
		}
		else //releases are delivered to who you clicked, not current mouseover.
		{
			if(lastClicked){lastClicked->onClick(mb);lastClicked=NULL;}
		}
		//its ok to have no return? What?
	}
	static int propagateRender(GUIbase* obj, void* arg, int rec)
	{
		if(true) // temp: if(obj->visible)
		{
			if(obj->visible){obj->render(arg);}
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
	~GUIbase()
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
};

GUIbase* GUIbase::lastClicked = NULL;
GUIbase* GUIbase::focus = NULL;

class GUIbutton: public GUIbase
{
	public:
	void (*func)(void*);
	void* arg;
	string text;
	GUIbutton():GUIbase()
	{
		func = NULL;
		arg = NULL;
		text = "";
	}
	void onClick(int mb)
	{
		if((mb==0)&&mouseOver)
		{
			GUIbase::onClick(mb);
			if(func)(func(arg));
		}
	}
	void render(void *arg)
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
		setColor(color_text);
		printw(pos.x+2,pos.y-4,text);
		
		glDisable(GL_SCISSOR_TEST);
	}
};

class GUIframe: public GUIbase
{
	public:
	string title;
	GUIbutton* CloseButton;
	static void btnClose(void* arg)
	{
		delete ((GUIframe*)arg);
	}
	GUIframe():GUIbase()
	{
		title = "Title";
		CloseButton = new GUIbutton();
		CloseButton->func = &btnClose;
		CloseButton->movable = false;
		CloseButton->resizible = false;
		//CloseButton->scissor = false; see we can't paint it outside client area and that's a problem.
		CloseButton->setSize(24,24);
		CloseButton->arg = (void*)this;
		CloseButton->setParent((GUIbase*)this);
	}
	void invalidate(vec2i newPos, vec2i newSize)
	{
		GUIbase::invalidate(newPos, newSize);
		CloseButton->setPos(pos.x+size.x-border-CloseButton->size.x,pos.y+border);
	}
	/*
	void recalculateClientRect()
	{
		crect.x1 = pos.x+border;
		crect.y1 = pos.y+border+32;
		crect.x2 = pos.x+size.x-border;
		crect.y2 = pos.y+size.y-border;
	}
	*/
	void render(void* arg)
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
		printw(pos.x,pos.y+4,title);
		
		glDisable(GL_SCISSOR_TEST);
	}
};

class GUIlabel: public GUIbase
{
	public:
	string text;
	GUIlabel():GUIbase()
	{
		text = "label";
		resizible = false;
		movable = false;
		size.y = 22;
	}
	void render(void* arg)
	{
		resizeCheck();
		dragCheck();
		scissorCheck(arg);
		
		size.x = printw(pos.x,pos.y,text);
		
		glDisable(GL_SCISSOR_TEST);
	}
};

class GUItextEntry: public GUIbase
{
	public:
	string text;
	GUItextEntry():GUIbase()
	{
		text = "Text entry";
		resizible = false;
		movable = false;
		size.y = 22;
		color_panel = {255,255,255};
		color_border = {0,0,0};
		color_text = {0,0,0};
	}
	void render(void* arg)
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
		size.x = printw(pos.x+2,pos.y,text)+4;
		
		glDisable(GL_SCISSOR_TEST);
	}
	void onKeyboard(int kb)
	{
		if(isprint(kb))
		{
			text += kb;
		}
		if(kb==8)
		{
			if(text.length()){text.erase(text.end()-1);}
		}
	}
};

class GUIcheckbox: public GUIbase
{
	public:
	bool checked;
	GUIcheckbox():GUIbase()
	{
		checked = false;
		resizible = false;
		movable = false;
		size = {14,14};
	}
	void onClick(int mb)
	{
		if((mb==0)&&mouseOver){checked = !checked;}
	}
	void render(void* arg)
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
		if(checked){printw(pos.x+size.x/2-4,pos.y-4,"v");}
		glDisable(GL_SCISSOR_TEST);
	}
};

class GUIradiogroup
{
	public:
	void *buttons[32];
	int selection;
	GUIradiogroup()
	{
		for(int I = 0;I<32;I++){buttons[I] = NULL;}
		selection = 0;
	}
	void checkButton(void *btn);
	void addButton(void *btn);
};

class GUIradiobutton: public GUIbase
{
	public:
	bool checked;
	GUIradiogroup* group;
	GUIradiobutton():GUIbase()
	{
		checked = false;
		resizible = false;
		movable = false;
		size = {16,16};
	}
	void onClick(int mb)
	{
		if((mb==0)&&mouseOver&&(group!=NULL)){group->checkButton((void*)this);}
	}
	void render(void* arg)
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
		if(checked){printw(pos.x+size.x/2-5,pos.y-4,"o");}
		glDisable(GL_SCISSOR_TEST);
	}
};
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
class GUIspinner: public GUIbase
{
	public:
	double vals[4];// min - cur - max - speed
	GUIbutton* btnUp;
	GUIbutton* btnDown;
	string text;
	
	static void fUp(void* arg)
	{
		double *vals = ((GUIspinner*)arg)->vals;
		vals[1]+=vals[3];
		if(vals[1]>vals[2]){vals[1]=vals[2];}
	}
	static void fDown(void* arg)
	{
		double *vals = ((GUIspinner*)arg)->vals;
		vals[1]-=vals[3];
		if(vals[1]<vals[0]){vals[1]=vals[0];}
	}
	GUIspinner():GUIbase()
	{
		counter = 0;
		size = {64,18};
		pos = {0,0};
		vals = {-10,0,10,3};
		movable = false;
		resizible = true;
		btnUp = new GUIbutton;
		btnUp->func = &fUp;
		btnUp->arg = (void*)this;
		btnUp->size = {18,9};
		btnUp->pos = {size.x-18,0};
		btnUp->text = "^";
		btnUp->setParent((GUIbase*)this);
		
		btnDown = new GUIbutton;
		btnDown->func = &fDown;
		btnDown->arg = (void*)this;
		btnDown->size = {18,9};
		btnDown->pos = {size.x-18,9};
		btnDown->text = "v";
		btnDown->setParent((GUIbase*)this);
	
	}
	void invalidate(vec2i newPos, vec2i newSize)
	{
		GUIbase::invalidate(newPos, newSize);
		btnUp->setPos(pos.x+size.x-18,pos.y);
		btnDown->setPos(pos.x+size.x-18,pos.y+9);
	}
	void render(void* arg)
	{
		resizeCheck();
		dragCheck();
		scissorCheck(arg);
		
		
		setColor(color_panel);
		paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
		setColor(color_border);
		paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
		setColor(color_text);
		printw(pos.x+2,pos.y,"%d", (int)vals[1]);
		
		glDisable(GL_SCISSOR_TEST);
	}
};

class GUIlistbox: public GUIbase
{
	public:
	GUIbutton* sel[32];
	int selected;
	string selText;
	void (*callback)(void*);
	void* callarg;
	GUIlistbox():GUIbase()
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
	static void wrapFunc(void* arg)
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
	void addOption(string text, void (*func)(void*), void* arg)
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
	void render(void* arg)
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
	
	void invalidate(vec2i newPos, vec2i newSize)
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
};

class GUIdropdownlist: public GUIbase
{
	public:
	GUIlistbox* list;
	GUIbutton* btn;
	string text;
	bool open;
	static void showList(void* arg)
	{
		GUIdropdownlist *L = ((GUIdropdownlist*)arg);
		if(!L->open){L->setSize(L->size.x,96);}else{L->setSize(L->size.x,14);}
		L->open = !L->open;
	}
	static void setCurOption(void* arg)
	{
		GUIdropdownlist* dd = (GUIdropdownlist*)arg;
		dd->text = dd->list->selText;
	}
	GUIdropdownlist():GUIbase()
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
	void addOption(string text, void (*func)(void*), void* arg)
	{
		list->addOption(text, func, arg);
	}
	void render(void* arg)
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
		printw(pos.x-2,pos.y-2,text);
		
		glDisable(GL_SCISSOR_TEST);
	}
	
	void invalidate(vec2i newPos, vec2i newSize)
	{
		GUIbase::invalidate(newPos, newSize);
		btn->setPos(pos.x+size.x-14,pos.y);
		list->setPos(pos.x,pos.y+14);
		recalculateClientRect();
	}
};
/*
GUI element check list!
--INPUT--
Button		*
Checkbox	*
Radiobutton *
TextEntry	* (sorta)
Spinner		*
DropDownList
ListBox		*
Slider
RoundSlider
ColorWheel
ColorBox
--NAVIGATION--
Frame		*
ScrollBar
Tab
--OUTPUT--
Label		*
Image


*/