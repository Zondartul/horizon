#include "hook.h"
#include "../util/globals.h"

void hookKind::add(string type, string name, funcptr func){
	hooks[type].push_back({name, func});
}
void hookKind::run(string type){
	vector<pair<string, funcptr>> V = hooks[type];
	for(int I = 0; I<V.size(); I++){
		V[I].second(NULL);
	}
}
void hookKind::remove(string type, string name){
	for(int I = 0; I<hooks[type].size(); I++){
		if(hooks[type][I].first == name){
			//printf("found it at %d\n",I);
			hooks[type].erase(hooks[type].begin()+I);
			I--;
		}
	}
}
hookKind hook;