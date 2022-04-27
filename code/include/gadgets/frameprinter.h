#ifndef FRAMEPRINTER_GUARD
#define FRAMEPRINTER_GUARD
#include <string>
#include "event.h"
#include "global_vars.h"
using std::string;
class renderLayer;

//add the string to the currently selected frameprinter.
//it will be printed during the next frame only.
void frameprint(string S);

class frameprinter:public eventListener{
	public:
	vector<string> strings;
	frameprinter();
	~frameprinter();
	virtual void print(string S);			//adds the string to the cache
	virtual void render(); 					//prints all cached strings
	virtual void onEvent(eventKind event);	//checks for frame events
};

void setFramePrinter(frameprinter *printer);


struct gs_frameprinterKind {
	frameprinter* g_P;
	renderLayer* g_layerFP;
};

#endif