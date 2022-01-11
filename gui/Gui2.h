#ifndef GUI2_GUARD
#define GUI2_GUARD

#include <string>
#include <cstdio>
#include <math.h>
#include "../util/messenger.h"
#include "../util/globals.h"
using std::string;

//#include "paint.h"

extern color3i paintColor; //???
extern byte paintAlpha;
extern vec2i mouseP; // mid-man variable for extra GUI's
void setColor(color3i color);
void setAlpha(byte alpha);

class GUIbase: public PSsubscriber
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
	bool dockup, dockdown, dockleft, dockright;
	vec4i dock;
	vec2i startTouch; //place of mouse click
	int counter; //mostly useless
	string tag;
	void (*think)(void*); //think func, different for each window
	
	GUIbase();
	virtual void setPos(int x, int y);
	virtual void setSize(int x, int y);
	virtual void setdock(bool up, bool down, bool left, bool right);
	
	virtual void dockCheck();
	virtual void onClick(int mb);
	virtual void onKeyboard(string kb);
	virtual void PSreceive(message msg);
	//virtual void onKeyboard();
	virtual void invalidate(vec2i newPos, vec2i newSize);
	virtual void recalculateClientRect();
	void resizeCheck();
	virtual void dragCheck();
	void scissorCheck(void* arg);
	virtual void render(void* arg);
	
	static bool foreach(GUIbase* obj, int (*func)(GUIbase*, void*, int), void* arg, int rec);
	
	static bool foreachbackwards_check(listNode* Cur, int (*func)(GUIbase*, void*, int), void* arg, int rec);
	static bool foreachbackwards(GUIbase* obj, int (*func)(GUIbase*, void*, int), void* arg, int rec);
	virtual void setParent(GUIbase* obj);
	GUIbase* findByTag(string tag);
	static void addChild(GUIbase* parent, GUIbase* obj);
	static int wrapInvalidate(GUIbase* obj, void* arg, int rec);
	static int propagateMouseOver(GUIbase* obj, void* arg, int rec);
	static GUIbase* lastClicked; //I hope you won't need to click two things at once.
	static GUIbase* focus;
	static int propagateKeyboard(string kb);
	static int propagateClick(GUIbase* obj, void* arg, int rec);
	static void fixstrata(GUIbase* obj);
	static int propagateRender(GUIbase* obj, void* arg, int rec);
	~GUIbase();
};

class GUIbutton: public GUIbase
{
	public:
	void (*func)(void*);
	void* arg;
	string text;
	GLuint image;
	GUIbutton();
	void onClick(int mb);
	void setImage(string path);
	void render(void *arg);
};

class GUIframe: public GUIbase
{
	public:
	string title;
	GUIbutton* CloseButton;
	static void btnClose(void* arg);
	GUIframe();
	void invalidate(vec2i newPos, vec2i newSize);
	
	
	void recalculateClientRect();
	void render(void* arg);
};

class GUIlabel: public GUIbase
{
	public:
	string text;
	GUIlabel();
	void render(void* arg);
};

class GUItextEntry: public GUIbase
{
	public:
	string text;
	bool multiline;
	bool sizeToContents;
	void (*callback)(void*);
	void* arg;
	GUItextEntry();
	void render(void* arg);
	void onKeyboard(string kb);
};

class GUIcheckbox: public GUIbase
{
	public:
	bool checked;
	void (*func)(void*);
	GUIcheckbox();
	
	void onClick(int mb);
	void render(void* arg);
};

class GUIradiogroup
{
	public:
	void *buttons[32];
	int selection;
	GUIradiogroup();
	void checkButton(void *btn);
	void addButton(void *btn);
};

class GUIradiobutton: public GUIbase
{
	public:
	bool checked;
	GUIradiogroup* group;
	GUIradiobutton();
	void onClick(int mb);
	void render(void* arg);
};
class GUIspinner: public GUIbase
{
	public:
	double vals[5];// min - cur - max - speed - precision
	GUIbutton* btnUp;
	GUIbutton* btnDown;
	void (*func)(void*);
	void* arg;
	string text;
	
	static void fUp(void* arg);
	static void fDown(void* arg);
	GUIspinner();
	void setVals(double a,double b,double c,double d,double e);
	void invalidate(vec2i newPos, vec2i newSize);
	void render(void* arg);
};

class GUIlistbox: public GUIbase
{
	public:
	GUIbutton* sel[32];
	int selected;
	string selText;
	void (*callback)(void*);
	void* callarg;
	GUIlistbox();
	static void wrapFunc(void* arg);
	void addOption(string text, void (*func)(void*), void* arg);
	void render(void* arg);
	
	void invalidate(vec2i newPos, vec2i newSize);
};

class GUIdropdownlist: public GUIbase
{
	public:
	GUIlistbox* list;
	GUIbutton* btn;
	string text;
	bool open;
	static void showList(void* arg);
	static void setCurOption(void* arg);
	GUIdropdownlist();
	void addOption(string text, void (*func)(void*), void* arg);
	void render(void* arg);
	void invalidate(vec2i newPos, vec2i newSize);
};

class GUIslider: public GUIbase
{
	public:
	//double vals[3];
	vector<double> vals;
	bool sliding;
	GUIslider();
	void render(void *arg);
	void onClick(int mb);
	void updateSlider();
};
// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)
vec3i RGBtoHSV(vec3i RGB);
vec3i HSVtoRGB(vec3i HSV);

class GUIcolorbox: public GUIbase
{
	public:
	vec3i colorHSV;
	vec3i colorRGB;
	vec3i colLU;
	vec3i colRU;
	vec3i colLD;
	vec3i colRD;
	vec2i cursor;
	GUIcolorbox();
	void onClick(int mb);
	void render(void *arg);
};

class GUIvaluedisplay:public GUIbase
{
	public:
	void *val;
	char mode;
	int precision;
	char str[256];
	GUIvaluedisplay();
	void render(void *arg);
};

class GUIImage:public GUIbase
{
	public:
	GLuint ImageTex;
	GUIImage();
	void setImage(char *path);
	void render(void *arg);
};

class GUIscrollslidey:public GUIbase
{
	public:
	void (*callback)(void*);
	void* arg;
	bool vertical;
	GUIscrollslidey();
	void onClick(int mb);
	void dragCheck();
	void moveupdown(int dist);
	void render(void *arg);
};

class GUIscrollBar:public GUIbase
{
	public:
	bool vertical;
	GUIbutton *btnup;
	GUIbutton *btndn;
	GUIbutton *track;
	GUIscrollslidey *slidey;
	vec2i insideSize;
	vec2i sizeOff;
	float Amin; //slider position
	float Amax;
	int Aoffset;
	static void btnupFunc(void *arg);
	static void btndnFunc(void *arg);
	static void updateBar(void *arg);
	static int propagateScroll(GUIbase* obj, void* arg, int rec);
	void setSize(int x, int y);
	GUIscrollBar();
	void setParent(GUIbase* obj);
	void invalidate(vec2i newPos, vec2i newSize);
	void render(void *arg);
};

#endif
