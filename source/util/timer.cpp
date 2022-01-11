#include "timer.h"
#include "event.h"
#include "hook.h"
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

int t = 0;		//game time in ticks
float t2 = 0;	//game time in seconds

int getGameTicks(){return t;}
float getGameTime(){return t2;}

void initTimers(){
	timer *T1 = new timer([&](){t++;},1,1);
	timer *T2 = new timer([&](){t2+=1/60.f;},1,1);
	hookAdd(globalChannel,EVENT_FRAME,"timers",
		[](eventKind e){
			timersTick();
		}
	);
}
