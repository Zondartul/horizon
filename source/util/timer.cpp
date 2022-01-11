#include "timer.h"
#include "event.h"
#include "hook.h"
#include <list>
#include "SDL2/SDL.h"
#include "time.h"
#include "stringUtils.h"
using std::list; //pointers stay valid
#include <sstream>
using std::stringstream;
#include <iomanip>

list<timer*> timers;

timer::timer(function<void(timer *T)> F, int ticks_max, bool repeat, bool run, bool selfdestruct){
	this->ticks_max = ticks_max;
	this->ticks_left = ticks_max;
	this->repeat = repeat;
	this->run = run;
	this->selfdestruct = selfdestruct;
	this->F = F;

	timers.push_back(this);
}

timer::~timer(){
    //printf("timer %p destructor...", this);
	for(auto I = timers.begin(); I != timers.end();){
		if(*I == this){/*printf("found, cut\n");*/ *I = 0; I++;/*I = timers.erase(I);*/}else{I++;}
	}
	//printf("did not find timer in the list\n");
}

void timer::tick(){     //honestly the most convoluted destructor in all of Horizon
	if(!run){return;}
	ticks_left--;
	if(!ticks_left){
		//printf("TIMERa %p: sd = %d, dr = %d\n", this, selfdestruct,deleteRequested);
		if(repeat){ticks_left = ticks_max;}
		if(selfdestruct){
            F(this);
            delete this;
		}else{//deleteRequested = true;}
            //printf("TIMERb %p: sd = %d, dr = %d\n", this, selfdestruct,deleteRequested);
            F(this);
		}
	}
}

void simpletimer(function<void(timer *T)> F, int ticks_max){
	new timer(F,ticks_max,0,1,1);
}

void timersTick(){
	for(auto I = timers.begin(); I != timers.end();){
        if(*I){
            //printf("tick(%p)",*I);
            (*I)->tick();
            I++;
		}else{
            I = timers.erase(I);
		}
	}
	//printf("timers tick---------\n");
	//for(auto I = timers.rbegin(); I != timers.rend();){
    //    timer *T = *I;
        //printf("T %p, dr = %d\n",T,T->deleteRequested);
    //    if(T->deleteRequested){delete T;/*invalidates iterators*/ I = timers.rbegin();}
    //    else{I++;}
	//}
}

int t = 0;		//game time in ticks
float t2 = 0;	//game time in seconds

int getGameTicks(){return t;}
float getGameTime(){return t2;}
float getRealTime(){return 0.001f*SDL_GetTicks();}
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
	//return toString(tinfo->tm_hour)+"."+toString(tinfo->tm_min)+"."+toString(tinfo->tm_sec);
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
	//return toString(tinfo->tm_mday)+"."+toString(tinfo->tm_mon)+"."+toString(tinfo->tm_year+1900);
	return ss.str();
}

void initTimers(){
	timer *T1 = new timer([&](timer *T){t++;},1,1);
	timer *T2 = new timer([&](timer *T){t2+=1/60.f;},1,1);
	hookAdd(globalChannel,EVENT_FRAME,"timers",
		[](eventKind e){
			timersTick();
		}
	);
}
