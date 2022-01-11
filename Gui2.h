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
	}
	
	void onClick();
	void onKeyboard();
	void invalidate();
	void render()
	{
		setColor(color_panel);
		if(!mouseOver){setAlpha(128);}
		paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
		setColor(color_border);
		setAlpha(255);
		paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);		
	}
	
	static void foreach(GUIbase* obj, bool (*func)(GUIbase*, void*, int), void* arg, int rec) // i couldn't think of saner way. sorry.
	{//calls func (3rd argument) upon all filled or all (arg2) children nodes of obj(arg1)
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
		y2 = x1+obj->size.y;
		if((x1<=mouse.x)&&(x2>=mouse.x)&&(y1<=mouse.y)&&(y2>=mouse.y))
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
	//static bool propagateClick(GUIbase* obj, void* arg)
	//{
	//}
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