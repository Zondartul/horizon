#ifndef TIMER_GUARD
#define TIMER_GUARD
#include "util/event.h"
#include "util/globals_render.h"
#include <functional>
#include <string>
#include <list>
using std::function;
using std::string;
using std::list;

class DLLAPI_RENDER timer{
	public:
	int ticks_max;
	int ticks_left;
	bool run;
	bool repeat;
	bool selfdestruct;
	bool deleteRequested = false;
	function<void(timer *T)> F;
	timer(function<void(timer *T)> F, int ticks_max=0, bool repeat=0, bool run=1, bool selfdestruct=0);
	~timer();
	void tick();
};

void DLLAPI_RENDER simpletimer(function<void(timer *T)> F, int ticks_max);
void DLLAPI_RENDER timersTick();
int DLLAPI_RENDER getGameTicks();
float DLLAPI_RENDER getGameTime();
DLLAPI_RENDER float getRealTime();
int DLLAPI_RENDER getCalendarTime();
string DLLAPI_RENDER getCalendarTimeStr();
int DLLAPI_RENDER getCalendarDate();
string DLLAPI_RENDER getCalendarDateStr();

//void initTimers();

//struct gs_timerKind {
//	list<timer*> g_timers;
//	int g_t = 0;		
//	float g_t2 = 0;	
//};

class DLLAPI_RENDER sysTimerKind {
public:
	sysTimerKind(sysEventKind &sysEvent);
	list<timer*> timers;
	int t = 0;
	float t2 = 0;
};

#endif
