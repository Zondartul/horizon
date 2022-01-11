#include "util/functor.h"

//functor::functor(){}
void functor::operator()(){printf("empty functor %p called\n",this);}

functor_from_funcptr::functor_from_funcptr(funcptr F, void *arg){
	this->arg = arg;
	this->F = F;
}

void functor_from_funcptr::operator ()(){
	if(F){
		printf("firing funcptr %p with argument %p\n",F,arg);
		F(arg);
	}else{printf("functor %p called with no funcptr\n",this);}
}