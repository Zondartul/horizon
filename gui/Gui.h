#include <string>
using std::string;

void paintRect(int X1, int Y1, int X2, int Y2);

void paintRectOutline(int X1, int Y1, int X2, int Y2);

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
    GUIobj();
    virtual void render();
	virtual void onClick(int mb);
	virtual ~GUIobj();
	virtual void keyDown(int kb);
	virtual void keyUp(int kb);
	virtual void invalidate();
};




class GUIManager //manages windows and controlls, their allocation and message passing
{
    public:
	
	GUIobj *focus;
	GUIobj *lastClick;
	
    //render tree
    GUIManager();
    void activate(GUIobj* it);
    void remove(GUIobj* curObj, GUIobj* obj);
	void axe(GUIobj* obj);
    void render(GUIobj* obj);
	bool checkMouseOver(GUIobj* obj, int x, int y);
	void reposition(GUIobj* obj, vec2i pos1, vec2i pos2);
	void click(GUIobj* obj, int mb);
	void keyboard(int kb);
    void input();
    listNode Root;
};

class GUIbutton: public GUIobj
{
	public:
	void (*func)(void*);
	void *funcHolder;
	string text;
	bool pressed;
	color3i textcolor;
	GUIbutton();
	void onClick(int mb);
	void render();
};

class GUIframe: public GUIobj
{
	public:
	vec2i dragStart;
	vec2i oldpos;
	bool dragging;
	GUIbutton *myButton;
	static void btnClose_wrapper(void* me);
	
	void btnClose();
	GUIframe();
	~GUIframe();
	void invalidate();
	void onClick(int mb);
	void render();
};

class GUItextEntry: public GUIobj
{
	public:
	color3i textColor;
	string text;
	void *font;
	bool hasFocus;
	GUItextEntry();
	void render();
	void onClick(int mb);
	void keyDown(int kb);
};

