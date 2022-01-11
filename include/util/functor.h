#ifndef FUNCTOR_GUARD
#define FUNCTOR_GUARD
#include "util/globals.h"

class functor{
	public:
	virtual void operator()();
};

class functor_from_funcptr:public functor{
	public:
	functor_from_funcptr(funcptr F, void *arg);
	funcptr F;
	void *arg;
	virtual void operator()();
};

#endif