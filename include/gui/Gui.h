#ifndef GUI_GUARD
#define GUI_GUARD
#include <string>
#include "../util/globals.h"
using std::string;

//void paintRect(int X1, int Y1, int X2, int Y2);

//void paintRectOutline(int X1, int Y1, int X2, int Y2);

enum GUI1Message
{
    GUIM_RENDER,
    GUIM_CHECK_MOUSEOVER,
    GUIM_CLICK,
    GUIM_KEYBOARD,
    GUIM_DESTROY
};


class GUI1obj
{
    public:
    vec2i pos;
    vec2i size;
    color3i color;
	color3i color2;
    int counter;
	GUI1obj *parent;
	void *GUIM;
    bool mouseOver;
    listNode children;
	int strata;
    GUI1obj();
    virtual void render();
	virtual void onClick(int mb);
	virtual ~GUI1obj();
	virtual void keyDown(int kb);
	virtual void keyUp(int kb);
	virtual void invalidate();
};




class GUI1Manager //manages windows and controlls, their allocation and message passing
{
    public:
	
	GUI1obj *focus;
	GUI1obj *lastClick;
	
    //render tree
    GUI1Manager();
    void activate(GUI1obj* it);
    void remove(GUI1obj* curObj, GUI1obj* obj);
	void axe(GUI1obj* obj);
    void render(GUI1obj* obj);
	bool checkMouseOver(GUI1obj* obj, int x, int y);
	void reposition(GUI1obj* obj, vec2i pos1, vec2i pos2);
	void click(GUI1obj* obj, int mb);
	void keyboard(int kb);
    void input();
    listNode Root;
};

class GUI1button: public GUI1obj
{
	public:
	void (*func)(void*);
	void *funcHolder;
	string text;
	bool pressed;
	color3i textcolor;
	GUI1button();
	void onClick(int mb);
	void render();
};

class GUI1frame: public GUI1obj
{
	public:
	vec2i dragStart;
	vec2i oldpos;
	bool dragging;
	GUI1button *myButton;
	static void btnClose_wrapper(void* me);
	
	void btnClose();
	GUI1frame();
	~GUI1frame();
	void invalidate();
	void onClick(int mb);
	void render();
};

class GUI1textEntry: public GUI1obj
{
	public:
	color3i textColor;
	string text;
	void *font;
	bool hasFocus;
	GUI1textEntry();
	void render();
	void onClick(int mb);
	void keyDown(int kb);
};
#endif
