#ifndef HOOK_GUARD
#define HOOK_GUARD
#include "../util/globals.h"
class hookKind{
	public:
	map<string,vector<pair<string, funcptr>>> hooks;
	void add(string type, string name, funcptr func);
	void run(string type);
	void remove(string type, string name);
};
extern hookKind hook;
#endif