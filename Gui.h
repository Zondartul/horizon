
/*
render tree
root node:                                 root
children render queue:          ch1       ch2   ch3   ch4
grandchildren render queue: ch1.1 ch1.2
*/
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

void setColor3(color3i color)
{
	glColor3f(color.r/255.0f, color.g/255.0f, color.b/255.0f);
}

enum GUIMessage
{
    GUIM_RENDER,
    GUIM_CHECK_MOUSEOVER,
    GUIM_CLICK,
    GUIM_KEYBOARD,
    GUIM_DESTROY
};


class GUIobj
{
    public:
    vec2i pos;
    vec2i size;
    color3i color;
	color3i color2;
    int counter;
	GUIobj *parent;
	void *GUIM;
    bool mouseOver;
    listNode children;
	int strata;
    GUIobj()
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
    virtual void render()
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
                setFont(Calibri20);
                printw(renderX1, renderY1+size.y/2, "Button");
            }
        printw(renderX2+8, renderY1+size.y/2, "Clicks: %d",counter);
    }
	virtual void onClick(int mb)
	{
		counter++;
	}
	virtual ~GUIobj()
	{
	}
	virtual void keyDown(int kb)
	{
	}
	virtual void keyUp(int kb)
	{
	}
	virtual void invalidate()
	{
	}
};




class GUIManager //manages windows and controlls, their allocation and message passing
{
    public:
    //render tree
    GUIManager()
    {

    }
    void activate(GUIobj* it) //inserts control into render queue
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
    void remove(GUIobj* curObj, GUIobj* obj) //removes control from render queue
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
				if(Cur->thing){remove(((GUIobj*)Cur->thing),obj);}
				Cur=Cur->next;
			}
		}
    }
    void render(GUIobj* obj)
    {
		listNode* Cur;
		if(obj){Cur = &(obj->children);}else{Cur = &Root;}
		while(Cur!=NULL)
		{
			if(Cur->thing!=NULL)
			{
				((GUIobj*)(Cur->thing))->render();
				this->render((GUIobj*)(Cur->thing));
			}
			Cur = Cur->next;
		}
    }
	bool checkMouseOver(GUIobj* obj, int x, int y)
	{
		// only one control can have mouse-over and it's always the top one.
		listNode* Cur;
		GUIobj *obj2, *obj3;
		obj3 = NULL;
		int x1,x2,y1,y2;
		int strata = 0;
		if(obj == NULL){Cur = &Root;}
		else{Cur = &(obj->children);}
			
		while(Cur!=NULL)
		{
			if(Cur->thing!=NULL)
			{
				
				obj2 = (GUIobj*)(Cur->thing);
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
	void reposition(GUIobj* obj, vec2i pos1, vec2i pos2)
	{
		obj->pos = obj->pos+pos2-pos1;
		listNode* Cur = &(obj->children);
		while(Cur!=NULL)
		{
			if(Cur->thing){reposition((GUIobj*)Cur->thing,pos1,pos2);}
			Cur = Cur->next;
		}
	}
	GUIobj* lastClick;
	void click(GUIobj* obj, int mb)
	{
		if(mb<1)
		{
			if(lastClick){lastClick->onClick(mb);}
		}
		else
		{
		listNode* Cur;
		GUIobj* obj2;
		
		if(obj == NULL){Cur = &Root;}
		else{Cur = &(obj->children);}
			
		while(Cur!=NULL)
		{
			if(Cur->thing!=NULL)
			{
				obj2 = (GUIobj*)(Cur->thing);
				if(obj2->mouseOver){obj2->onClick(mb);lastClick = obj2;}		
				this->click(obj2, mb);
			}
			Cur = Cur->next;
		}
		}
	}
	GUIobj *focus;
	void keyboard(int kb)
	{
		if(focus)
		{
			if(kb>0){focus->keyDown(kb);}
			else{focus->keyUp(kb);}
		}
	}
    void input()
    {

    }
    listNode Root;
};

class GUIbutton: public GUIobj
{
	public:
	void (*func)(void*);
	void *funcHolder;
	char *text;
	bool pressed;
	color3i textcolor;
	GUIbutton()
	{
		//func = NULL;
		text = "X";
		textcolor = {0,0,0};
	}
	void onClick(int mb)
	{
		if(mb){pressed = 1;}
		else
		{
			if(pressed)
			{
				if(func&&funcHolder)
				{func(funcHolder);}
			}
			pressed = 0;
		}
	}
	void render()
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
			printw(pos.x+4,pos.y+4,"%s",text);
		}
		else
		{
			if(!mouseOver){pressed = 0;}
			glColor3f(color2.r/255.0f,color2.g/255.0f,color2.b/255.0f);
			paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
			glColor3f(textcolor.r/255.0f,textcolor.g/255.0f,textcolor.b/255.0f);
			printw(pos.x+4,pos.y+4,"%s",text);
		}
	}
};

class GUIframe: public GUIobj
{
	public:
	vec2i dragStart;
	vec2i oldpos;
	bool dragging;
	GUIbutton *myButton;
	static void btnClose_wrapper(void* me) //wtf are these shenanigans
	{									   //upper one actually gets sent
		((GUIframe*)me)->btnClose();	   //and used for onClick(mb)
	}
	
	void btnClose()
	{
		delete this;
	}										//:|
	
	GUIframe()
	{
		dragging = 0;
		dragStart = {0,0};
		pos = {0,0};
		oldpos = {0,0};
		myButton = new GUIbutton;
		myButton->size = {24,24};
		myButton->parent = (GUIobj*)this;//commenting this makes me crash?
		myButton->func = &btnClose_wrapper;
		myButton->funcHolder = (void*)this;
		((GUIManager*)GUIM)->activate((GUIobj*)myButton); //this button gets positioned to the moon. FIND IT!
	}
	~GUIframe()
	{
		//delete myButton;//uhh do I need to?...
		((GUIManager*)GUIM)->remove(NULL, (GUIobj*)this);
		//delete this; NEVER do this in a destructor
	}
	void invalidate()
	{
		myButton->pos = {size.x-26,2};
		myButton->color = color+(color3i){32,32,32};
		myButton->color2 = color2;
	}
	void onClick(int mb)
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
	void render() // later will be delegated to skins to draw on their own
	{
		if(dragging){pos = mousePos-dragStart;}
		if(!(oldpos==pos))
		{
			vec2i pos2 = pos;
			pos = oldpos;
			((GUIManager*)GUIM)->reposition(this, oldpos, pos2);
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
		setFont(Tahoma22);
		printw(rX1+4, rY1+5, "Title");//todo: centering func
		
		//printw(rX2+8, rY1+size.y/2, "Clicks: %d",counter);
	}
};

class GUItextEntry: public GUIobj
{
	public:
	color3i textColor;
	char *text;
	char oneChar;
	void *font;
	bool hasFocus;
	GUItextEntry()
	{
		textColor = {0,0,0};
		text = "All the pretty little horses";
		font = Calibri18;
		color = {255,255,255};
		color2 = {0,0,0};
	}
	void render()
	{
		setColor3(color);
		paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
		
		setColor3(color2);
		paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
		
		setColor3(textColor);
		setFont(font);
		printw(pos.x+4,pos.y+4, text);
	}
	void onClick(int mb)
	{
		hasFocus = true;
		((GUIManager*)GUIM)->focus = (GUIobj*)this;
	}
	void keyDown(int kb)
	{
		if(hasFocus)
		{
			text = &oneChar;
			oneChar = (char)kb;
		}
	}
};

