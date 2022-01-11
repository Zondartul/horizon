#ifndef TIMER_GUARD
#define TIMER_GUARD
#include <functional>
using std::function;
#include <string>
using std::string;

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

#endif
