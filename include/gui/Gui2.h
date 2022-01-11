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
//void setColor(color3i color);
//void setAlpha(byte alpha);

class GUI2base: public PSsubscriber
{
	public:
	listNode* children;
	GUI2base* parent;
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
	
	GUI2base();
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
	
	static bool foreach(GUI2base* obj, int (*func)(GUI2base*, void*, int), void* arg, int rec);
	
	static bool foreachbackwards_check(listNode* Cur, int (*func)(GUI2base*, void*, int), void* arg, int rec);
	static bool foreachbackwards(GUI2base* obj, int (*func)(GUI2base*, void*, int), void* arg, int rec);
	virtual void setParent(GUI2base* obj);
	GUI2base* findByTag(string tag);
	static void addChild(GUI2base* parent, GUI2base* obj);
	static int wrapInvalidate(GUI2base* obj, void* arg, int rec);
	static int propagateMouseOver(GUI2base* obj, void* arg, int rec);
	static GUI2base* lastClicked; //I hope you won't need to click two things at once.
	static GUI2base* focus;
	static int propagateKeyboard(string kb);
	static int propagateClick(GUI2base* obj, void* arg, int rec);
	static void fixstrata(GUI2base* obj);
	static int propagateRender(GUI2base* obj, void* arg, int rec);
	~GUI2base();
};

class GUI2button: public GUI2base
{
	public:
	void (*func)(void*);
	void* arg;
	string text;
	GLuint image;
	GUI2button();
	void onClick(int mb);
	void setImage(string path);
	void render(void *arg);
};

class GUI2frame: public GUI2base
{
	public:
	string title;
	GUI2button* CloseButton;
	static void btnClose(void* arg);
	GUI2frame();
	void invalidate(vec2i newPos, vec2i newSize);
	
	
	void recalculateClientRect();
	void render(void* arg);
};

class GUI2label: public GUI2base
{
	public:
	string text;
	GUI2label();
	void render(void* arg);
};

class GUI2textEntry: public GUI2base
{
	public:
	string text;
	bool multiline;
	bool sizeToContents;
	void (*callback)(void*);
	void* arg;
	GUI2textEntry();
	void render(void* arg);
	void onKeyboard(string kb);
};

class GUI2checkbox: public GUI2base
{
	public:
	bool checked;
	void (*func)(void*);
	GUI2checkbox();
	
	void onClick(int mb);
	void render(void* arg);
};

class GUI2radiogroup
{
	public:
	void *buttons[32];
	int selection;
	GUI2radiogroup();
	void checkButton(void *btn);
	void addButton(void *btn);
};

class GUI2radiobutton: public GUI2base
{
	public:
	bool checked;
	GUI2radiogroup* group;
	GUI2radiobutton();
	void onClick(int mb);
	void render(void* arg);
};
class GUI2spinner: public GUI2base
{
	public:
	double vals[5];// min - cur - max - speed - precision
	GUI2button* btnUp;
	GUI2button* btnDown;
	void (*func)(void*);
	void* arg;
	string text;
	
	static void fUp(void* arg);
	static void fDown(void* arg);
	GUI2spinner();
	void setVals(double a,double b,double c,double d,double e);
	void invalidate(vec2i newPos, vec2i newSize);
	void render(void* arg);
};

class GUI2listbox: public GUI2base
{
	public:
	GUI2button* sel[32];
	int selected;
	string selText;
	void (*callback)(void*);
	void* callarg;
	GUI2listbox();
	static void wrapFunc(void* arg);
	void addOption(string text, void (*func)(void*), void* arg);
	void render(void* arg);
	
	void invalidate(vec2i newPos, vec2i newSize);
};

class GUI2dropdownlist: public GUI2base
{
	public:
	GUI2listbox* list;
	GUI2button* btn;
	string text;
	bool open;
	static void showList(void* arg);
	static void setCurOption(void* arg);
	GUI2dropdownlist();
	void addOption(string text, void (*func)(void*), void* arg);
	void render(void* arg);
	void invalidate(vec2i newPos, vec2i newSize);
};

class GUI2slider: public GUI2base
{
	public:
	//double vals[3];
	vector<double> vals;
	bool sliding;
	GUI2slider();
	void render(void *arg);
	void onClick(int mb);
	void updateSlider();
};
// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)
vec3i RGBtoHSV(vec3i RGB);
vec3i HSVtoRGB(vec3i HSV);

class GUI2colorbox: public GUI2base
{
	public:
	vec3i colorHSV;
	vec3i colorRGB;
	vec3i colLU;
	vec3i colRU;
	vec3i colLD;
	vec3i colRD;
	vec2i cursor;
	GUI2colorbox();
	void onClick(int mb);
	void render(void *arg);
};

class GUI2valuedisplay:public GUI2base
{
	public:
	void *val;
	char mode;
	int precision;
	char str[256];
	GUI2valuedisplay();
	void render(void *arg);
};

class GUI2Image:public GUI2base
{
	public:
	GLuint ImageTex;
	GUI2Image();
	void setImage(char *path);
	void render(void *arg);
};

class GUI2scrollslidey:public GUI2base
{
	public:
	void (*callback)(void*);
	void* arg;
	bool vertical;
	GUI2scrollslidey();
	void onClick(int mb);
	void dragCheck();
	void moveupdown(int dist);
	void render(void *arg);
};

class GUI2scrollBar:public GUI2base
{
	public:
	bool vertical;
	GUI2button *btnup;
	GUI2button *btndn;
	GUI2button *track;
	GUI2scrollslidey *slidey;
	vec2i insideSize;
	vec2i sizeOff;
	float Amin; //slider position
	float Amax;
	int Aoffset;
	static void btnupFunc(void *arg);
	static void btndnFunc(void *arg);
	static void updateBar(void *arg);
	static int propagateScroll(GUI2base* obj, void* arg, int rec);
	void setSize(int x, int y);
	GUI2scrollBar();
	void setParent(GUI2base* obj);
	void invalidate(vec2i newPos, vec2i newSize);
	void render(void *arg);
};

#endif
