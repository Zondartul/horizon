#include "elastic_ptr.h"
elastic_ptr_anchor::elastic_ptr_anchor(){
	proxy = new elastic_ptr_anchor_proxy(this);
	proxy->increment();
}
elastic_ptr_anchor::~elastic_ptr_anchor(){
	proxy->anchor = 0;
	proxy->decrement();
	proxy = 0;
}
elastic_ptr_anchor_proxy::elastic_ptr_anchor_proxy(elastic_ptr_anchor *newanchor){
	anchor = newanchor;
}
void elastic_ptr_anchor_proxy::decrement(){
	refcount--;
	if(!refcount){
		if(anchor){
			//error("elastic_ptr inc/dec mismatch (anchor still alive)\n");
			throw std::runtime_error("elastic_ptr inc/dec mismatch (anchor still alive)\n");
		}
		delete this;
	}
}
void elastic_ptr_anchor_proxy::increment(){
	refcount++;
}