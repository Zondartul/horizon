#include "util/hook.h"
#include "util/globals.h"

void hookKind::add(string type, string name, functor *F){
	hooks[type].push_back({name, F});
}
void hookKind::run(string type){
	vector<pair<string, functor*>> V = hooks[type];
	for(int I = 0; I<V.size(); I++){
		printf("firing [%s] hook [%s:%p]\n",type.c_str(),V[I].first.c_str(),&(V[I].second));
		(*(V[I].second))();
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