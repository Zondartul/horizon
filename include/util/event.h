#ifndef EVENT_GUARD
#define EVENT_GUARD
#include <vector>
using std::vector;
#include "vec.h"

struct event_keyboard{
	int keycode;
	const char *key;
};

struct event_mouse_button{
	int button;
};

struct event_mouse_move{
	vec2i diff;
	vec2i pos;
};

struct event_mouse_wheel{
	int x;
	int y;
};
enum eventType{	EVENT_KEY_UP, 
			EVENT_KEY_DOWN, 
			EVENT_MOUSE_BUTTON_UP,
			EVENT_MOUSE_BUTTON_DOWN,
			EVENT_MOUSE_MOVE, 
			EVENT_MOUSE_WHEEL};
struct eventKind{
	eventType type;
	union{
		event_keyboard keyboard;
		event_mouse_button mousebutton;
		event_mouse_move mousemove;
		event_mouse_wheel mousewheel;
	};
};

struct eventListener;
struct eventListenerList{
	vector<eventListener*> listeners;
	void publishEvent(eventKind event);
	void addListener(eventListener *listener);
	void removeListener(eventListener *listener);
};

class eventListener{
	public:
	virtual void onEvent(eventKind event);
};



#endif