#ifndef EVENT_GUARD
#define EVENT_GUARD
#include "math/vec.h"
#include "util/hook.h"
#include "util/globals_util.h"
#include <vector>
#include <functional>
using std::vector;
using std::function;
#define MOD_NONE 0
#define MOD_SHIFT 1
#define MOD_CTRL 2
#define MOD_ALT 4

struct DLLAPI_UTIL event_keyboard{
	int keycode;
	const char *key;
	char printchar;
	int mod;
};

enum DLLAPI_UTIL mouseButtonType{MOUSE_LEFT,MOUSE_MIDDLE,MOUSE_RIGHT,MOUSE_X1,MOUSE_X2};

struct DLLAPI_UTIL event_mouse_button{
	mouseButtonType button;
};

struct DLLAPI_UTIL event_mouse_move{
	vec2 diff;
	vec2 pos;
};

struct DLLAPI_UTIL event_mouse_wheel{
	int x;
	int y;
};

struct DLLAPI_UTIL event_window_resize {
	int width;
	int height;
};

enum DLLAPI_UTIL eventType{	
			EVENT_KEY_UP,				
			EVENT_KEY_DOWN,
			EVENT_MOUSE_BUTTON_UP,		
			EVENT_MOUSE_BUTTON_DOWN,
			EVENT_MOUSE_MOVE,			
			EVENT_MOUSE_WHEEL,			
			EVENT_FRAME,				
			EVENT_TICK,					
			EVENT_CLEANUP,
			EVENT_WINDOW_RESIZE
			};

struct DLLAPI_UTIL eventKind{
	eventType type;
	int *mask;
	void maskEvent(int newmask = 1);
	int isMasked();
	eventKind();
	eventKind(eventType type);
	union{
		event_keyboard keyboard;
		event_mouse_button mousebutton;
		event_mouse_move mousemove;
		event_mouse_wheel mousewheel;
		event_window_resize windowresize;
	};
};

string DLLAPI_UTIL toString(eventType et);

class eventListener;

struct DLLAPI_UTIL eventListenerList{
	vector<eventListener*> listeners;
	void publishEvent(eventKind event);
	int publishEventSequentialMaskable(eventKind event);	
	int publishEventParallelMaskable(eventKind event);
	void addListener(eventListener *listener);
	void addListenerFront(eventListener *listener);
	void moveListenerToFront(eventListener *listener);
	void moveListenerToBack(eventListener *listener);
	void removeListener(eventListener *listener);
};

typedef eventListenerList eventChannel;

class DLLAPI_UTIL eventListener{
	public:
	vector<eventChannel*> channels;
	virtual ~eventListener();
	virtual void onEvent(eventKind event);
};

//void initEvents();

//struct gs_eventKind {
	//eventChannel* g_globalChannel;
//};
class hook;

class DLLAPI_UTIL sysEventKind {
public:
	sysEventKind();
	eventChannel globalChannel;
	vector<hook*> hooks;
	void hookAdd(eventChannel* ch, eventType type, string name, function<void(eventKind event)> handler);
	void hookRemove(eventChannel* ch, string name);
};


class DLLAPI_UTIL hook :public eventListener {
public:
	eventChannel* ch;
	eventType type;
	string name;
	function<void(eventKind event)> handler;
	void onEvent(eventKind event);
};

#endif
