#include "timer.h"
#include "event.h"
#include "hook.h"
#include <list>

#ifndef NO_SDL
#include <SDL2/SDL.h>
#endif

#include "time.h"
#include "stringUtils.h"
using std::list; //pointers stay valid
#include <sstream>
using std::stringstream;
#include <iomanip>

list<timer*> g_timers;

timer::timer(function<void(timer *T)> F, int ticks_max, bool repeat, bool run, bool selfdestruct){
	this->ticks_max = ticks_max;
	this->ticks_left = ticks_max;
	this->repeat = repeat;
	this->run = run;
	this->selfdestruct = selfdestruct;
	this->F = F;

	g_timers.push_back(this);
}

timer::~timer(){
	for(auto I = g_timers.begin(); I != g_timers.end();){
		if(*I == this){ *I = 0; I++;}else{I++;}
	}
}

void timer::tick(){     //honestly the most convoluted destructor in all of Horizon
	if(!run){return;}
	ticks_left--;
	if(!ticks_left){
		if(repeat){ticks_left = ticks_max;}
		if(selfdestruct){
            F(this);
            delete this;
		}else{
            F(this);
		}
	}
}

void simpletimer(function<void(timer *T)> F, int ticks_max){
	new timer(F,ticks_max,0,1,1);
}

void timersTick(){
	for(auto I = g_timers.begin(); I != g_timers.end();){
        if(*I){
            (*I)->tick();
            I++;
		}else{
            I = g_timers.erase(I);
		}
	}
}

int g_t = 0;		//game time in ticks
float g_t2 = 0;	//game time in seconds

int getGameTicks(){return g_t;}
float getGameTime(){return g_t2;}
float getRealTime(){
#ifndef NO_SDL
	return 0.001f*SDL_GetTicks();
#else
	return 0;
#endif
}
int getCalendarTime(){
	time_t t = time(0);
	struct tm *tinfo = localtime(&t);
	return tinfo->tm_sec+60*tinfo->tm_min+3600*tinfo->tm_hour;
}

#define FMT_4DIG std::setfill('0') << std::setw(4)
#define FMT_2DIG std::setfill('0') << std::setw(2)

string getCalendarTimeStr(){
	time_t t = time(0);
	struct tm *tinfo = localtime(&t);
	stringstream ss;
	ss << FMT_2DIG << tinfo->tm_hour << "." << FMT_2DIG << tinfo->tm_min << "." << FMT_2DIG << tinfo->tm_sec;
	return ss.str();
}
int getCalendarDate(){
	time_t t = time(0);
	struct tm *tinfo = localtime(&t);
	return tinfo->tm_yday;
}


string getCalendarDateStr(){
	time_t t = time(0);
	struct tm *tinfo = localtime(&t);
	stringstream ss;
	ss << FMT_2DIG << tinfo->tm_mday << "." << FMT_2DIG << tinfo->tm_mon << "." << FMT_4DIG <<(tinfo->tm_year+1900);
	return ss.str();
}

void initTimers(){
	timer *T1 = new timer([&](timer *T){g_t++;},1,1);
	timer *T2 = new timer([&](timer *T){g_t2+=1/60.f;},1,1);
	hookAdd(g_globalChannel,EVENT_FRAME,"timers",
		[](eventKind e){
			timersTick();
		}
	);
}
