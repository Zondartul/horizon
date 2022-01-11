#ifndef EVENT_GUARD
#define EVENT_GUARD
#include <vector>
using std::vector;
#include "vec.h"

#define MOD_NONE 0
#define MOD_SHIFT 1
#define MOD_CTRL 2
#define MOD_ALT 4

struct event_keyboard{
	int keycode;
	const char *key;
	char printchar;
	int mod;
};

enum mouseButtonType{MOUSE_LEFT,MOUSE_MIDDLE,MOUSE_RIGHT,MOUSE_X1,MOUSE_X2};
struct event_mouse_button{
	mouseButtonType button;
};

struct event_mouse_move{
	vec2 diff;
	vec2 pos;
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
			EVENT_MOUSE_WHEEL,
			EVENT_FRAME};
struct eventKind{
	eventType type;
	int *mask;
	void maskEvent(int newmask = 1);
	int isMasked();
	eventKind();
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
	int publishEventSequentialMaskable(eventKind event);	//returns non-zero if the event was handled/masked
	int publishEventParallelMaskable(eventKind event);
	void addListener(eventListener *listener);
	void addListenerFront(eventListener *listener);
	void moveListenerToFront(eventListener *listener);
	void moveListenerToBack(eventListener *listener);
	void removeListener(eventListener *listener);
};
typedef eventListenerList eventChannel;

class eventListener{
	public:
    virtual ~eventListener() = default;

	virtual void onEvent(eventKind event);
};

extern eventChannel *globalChannel;
void initEvents();

#endif
