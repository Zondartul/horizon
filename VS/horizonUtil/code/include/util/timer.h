#ifndef TIMER_GUARD
#define TIMER_GUARD
#include "util/event.h"
#include "util/globals_util.h"
#include <functional>
#include <string>
#include <list>
using std::function;
using std::string;
using std::list;

class DLLAPI_UTIL timer{
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

void DLLAPI_UTIL simpletimer(function<void(timer *T)> F, int ticks_max);
void DLLAPI_UTIL timersTick();
int DLLAPI_UTIL getGameTicks();
float DLLAPI_UTIL getGameTime();
DLLAPI_UTIL float getRealTime();
int DLLAPI_UTIL getCalendarTime();
string DLLAPI_UTIL getCalendarTimeStr();
int DLLAPI_UTIL getCalendarDate();
string DLLAPI_UTIL getCalendarDateStr();

#define ON_NEXT_FRAME(x) simpletimer([&](timer *T){x;}, 2);

//void initTimers();

//struct gs_timerKind {
//	list<timer*> g_timers;
//	int g_t = 0;		
//	float g_t2 = 0;	
//};

class DLLAPI_UTIL sysTimerKind {
public:
	sysTimerKind(sysEventKind &sysEvent);
	list<timer*> timers;
	int t = 0;
	float t2 = 0;
};

#endif
