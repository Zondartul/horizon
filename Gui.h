
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
    int counter;
	GUIobj *parent;
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
		strata = 0;
    }
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
                //setFont(Calibri20n2);
                printw(renderX1, renderY1+size.y/2, "Button");
            }
        printw(renderX2+8, renderY1+size.y/2, "Clicks: %d",counter);
    }
	void onClick(int mb)
	{
		counter++;
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
            if(Cur->thing==NULL){Cur->thing=(void*)it; it->strata = strata; done = 1;}
            else if(Cur->next==NULL)
            {
                Cur->next = new listNode;
                Cur->next->next = NULL;
				it->strata = strata+1;
                Cur->next->thing = (void*)it;
                done = 1;
            }
            else{Cur = Cur->next;strata++;}
        }
    }
    void remove() //removes control from render queue
    {

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
	void click(GUIobj* obj, int mb)
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
				if(obj2->mouseOver){obj2->onClick(mb);}		
				this->click(obj2, mb);
			}
			Cur = Cur->next;
		}
	}
    void input()
    {

    }
    listNode Root;
};

class GUIframe: public GUIobj
{
	//vec2i dragStart;
	void render()
	{
		int rX1 = pos.x;
        int rY1 = pos.y;
        int rX2 = pos.x+size.x;
        int rY2 = pos.y+size.y;
		glColor3f(color.r/255.0f,color.g/255.0f,color.b/255.0f);
		paintRect(rX1+2, rY1+2, rX2-2, rY1+32);
		paintRectOutline(rX1,rY1,rX2,rY2);
	}
};
