#ifndef TIMER_GUARD
#define TIMER_GUARD
#include <functional>
#include <string>
#include <list>
using std::function;
using std::string;
using std::list;

class timer{
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

void simpletimer(function<void(timer *T)> F, int ticks_max);

void timersTick();
int getGameTicks();
float getGameTime();
float getRealTime();

int getCalendarTime();
string getCalendarTimeStr();
int getCalendarDate();
string getCalendarDateStr();

void initTimers();

struct gs_timerKind {
	list<timer*> g_timers;
	int g_t = 0;		//game time in ticks
	float g_t2 = 0;	//game time in seconds
};

#endif
