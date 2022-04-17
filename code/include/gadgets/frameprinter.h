#ifndef FRAMEPRINTER_GUARD
#define FRAMEPRINTER_GUARD
#include "event.h"
#include <string>
using std::string;
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

#endif