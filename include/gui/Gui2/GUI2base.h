#ifndef GUI2BASE_GUARD
#define GUI2BASE_GUARD
#include "util/messenger.h"
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
#endif