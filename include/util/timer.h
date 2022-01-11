#ifndef TIMER_GUARD
#define TIMER_GUARD
#include <functional>
using std::function;

class timer{
	public:
	int ticks_max;
	int ticks_left;
	bool run;
	bool repeat;
	bool selfdestruct;
	function<void()> F;
	timer(function<void()> F, int ticks_max=0, bool repeat=0, bool run=1, bool selfdestruct=0);
	~timer();
	void tick();
};

void simple_timer(function<void()> F, int ticks_max);

void timersTick();

#endif