#ifndef ELASTIC_PTR_GUARD
#define ELASTIC_PTR_GUARD
#include "util/globals_util.h"
#include <typeinfo>
#include <string>
#include <sstream>
#include <stdexcept>
using std::string;
using std::stringstream;

class elastic_ptr_anchor_proxy;

class DLLAPI_UTIL elastic_ptr_anchor{
	private:
	elastic_ptr_anchor_proxy *proxy=0;
	friend class elastic_ptr_anchor_proxy;
	template<typename T> friend class elastic_ptr;
	public:
	elastic_ptr_anchor();
	elastic_ptr_anchor(const elastic_ptr_anchor &) = delete;
	~elastic_ptr_anchor();
};

class DLLAPI_UTIL elastic_ptr_anchor_proxy{
	private:
	friend class elastic_ptr_anchor;
	template<typename T> friend class elastic_ptr;
	elastic_ptr_anchor *anchor;
	int refcount = 0;
	elastic_ptr_anchor_proxy(elastic_ptr_anchor *newanchor);
	void decrement();
	void increment();
};

template<typename T> class elastic_ptr{
	private:
	const static bool debug = false;
	elastic_ptr_anchor_proxy *proxy=0;
	void badDerefError(){
		string type = typeid(T).name();
		if(debug){
			//error("attempt to deref empty elastic_ptr to %s\n",type.c_str());
			stringstream ss;
			ss << "attempt to deref empty elastic_ptr to " << type << "\n";
			throw std::runtime_error(ss.str());
		}
	}
	string toString() const{
		void *ptrProxy = 0;
		void *ptrAnchor = 0;
		string static_type = typeid(T).name();
		string dynamic_type = "(null)";
		if(proxy){
			ptrProxy = (void*)proxy;
			if(proxy->anchor){
				ptrAnchor = (void*)(proxy->anchor);
				dynamic_type = typeid(proxy->anchor).name();
			}
		}
		return fstring("[%p:elptr<%s> to %s, p %p, a %p]",this,static_type.c_str(),dynamic_type.c_str(),ptrProxy,ptrAnchor);
	}
	void retarget(elastic_ptr_anchor_proxy *newproxy){
		if(proxy == newproxy){return;}
		if(proxy){proxy->decrement();}
		proxy = newproxy;
		if(proxy){proxy->increment();}
	}
	public:
	elastic_ptr(){
		if(debug){printf("constructed %s\n",toString().c_str());}
	}
	elastic_ptr(T *obj){
		if(obj){
			proxy = obj->elastic_ptr_anchor::proxy;
			if(!proxy){
				//error("zombie elastic_anchor at %p\n",obj);
				stringstream ss;
				ss << "zombie elastic_anchro at " << (void*)obj << "\n";
				throw std::runtime_error(ss.str());
			}
			proxy->increment();
		}
		if(debug){printf("constructed %s from %p\n",toString().c_str(),obj);}
	}
	elastic_ptr(const elastic_ptr<T> &other){
		retarget(other.proxy);
		if(debug){printf("copy-constructed %s\n",toString().c_str());}
	}
	~elastic_ptr(){if(proxy){proxy->decrement();}}
	T& operator*(){
		T* result = 0;
		if(!proxy || !proxy->anchor){badDerefError();}
		result = static_cast<T*>(proxy->anchor);
		if(debug){printf("*%s = %p\n",toString().c_str(),result);}
		return *result;
	}
	T *operator->(){
		T* result = 0;
		if(!proxy || !proxy->anchor){badDerefError();}
		result = static_cast<T*>(proxy->anchor);
		if(debug){printf("->%s = %p\n",toString().c_str(),result);}
		return result;
	}
	operator bool(){
		bool result = false;
		if(!proxy || !proxy->anchor){result = false;}
		else{result = true;}
		if(debug){printf("toBool(%s) = %d\n",toString().c_str(),result);}
		return result;
	}
	explicit operator T*(){
		T* result = 0;
		if(!proxy || !proxy->anchor){badDerefError();}
		result = static_cast<T*>(proxy->anchor);
		if(debug){printf("toRaw(%s) = %p\n",toString().c_str(),result);}
		return result;
	}
	bool operator==(const elastic_ptr<T>& other){
		return proxy == other.proxy;
	}
	/*bool operator==(T *obj){
		bool result = false;
		if(!proxy || !proxy->anchor){result = (obj==0);}
		else{result = (obj==static_cast<T*>(proxy->anchor));}
		if(debug){printf("(%s == %p) == %d\n",toString().c_str(),result);}
		return result;
	}*/
	elastic_ptr &operator=(const elastic_ptr &other){
		if(debug){printf("assign %s = %s\n",toString().c_str(), other.toString().c_str());}
		retarget(other.proxy);
		return *this;
	}
	elastic_ptr &operator=(const T *obj){
		if(debug){printf("assign %s = %p\n",toString().c_str(),obj);}
		if(obj){
			retarget(obj->elastic_ptr_anchor::proxy);
		}else{
			retarget(0);
		}
		return *this;
	}
};

#define EPCAST(ptrsrc,ptrdest) if(ptrsrc){ptrdest = dynamic_cast<decltype(ptrdest)>(&*ptrsrc);}

#endif