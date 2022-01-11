#include "util/functor.h"

//functor::functor(){}
void functor::operator()(){}

functor_from_funcptr::functor_from_funcptr(funcptr F, void *arg){
	this->arg = arg;
	this->F = F;
}

void functor_from_funcptr::operator ()(){
	if(F){F(arg);}
}