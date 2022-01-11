#ifndef HOOK_GUARD
#define HOOK_GUARD
#include "util/globals.h"
#include "util/functor.h"
class hookKind{
	public:
	map<string,vector<pair<string, functor*>>> hooks;
	void add(string type, string name, functor*);
	void run(string type);
	void remove(string type, string name);
};
extern hookKind hook;
#endif