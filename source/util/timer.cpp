#include "timer.h"
#include <list>
using std::list; //pointers stay valid

list<timer*> timers;

timer::timer(function<void()> F, int ticks_max, bool repeat, bool run, bool selfdestruct){
	this->ticks_max = ticks_max;
	this->ticks_left = ticks_max;
	this->repeat = repeat;
	this->run = run;
	this->selfdestruct = selfdestruct;
	this->F = F;
	
	timers.push_back(this);
}

timer::~timer(){
	for(auto I = timers.begin(); I != timers.end(); I++){
		if(*I = this){I = timers.erase(I); return;}
	}
}

void timer::tick(){
	if(!run){return;}
	ticks_left--;
	if(!ticks_left){
		F();
		if(repeat){ticks_left = ticks_max;}else
		if(selfdestruct){delete this;}
	}
}

void simpletimer(function<void()> F, int ticks_max){
	new timer(F,ticks_max,0,1,1);
}

void timersTick(){
	for(auto I = timers.begin(); I != timers.end(); I++){
		(*I)->tick();
	}
}