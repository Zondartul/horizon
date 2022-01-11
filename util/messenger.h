#ifndef MESSENGER_GUARD
#define MESSENGER_GUARD 
#include "../util/globals.h"

struct message
{
	//message problem: every message carries pointer to non-primitive type
	//messages can not be copied content must be deleted immediately.
	string type;
	list<void*> vdata; //one-use only!
	string str;
	union data_t{
		char c;
		int i;
		double d;
		float f;
		vec2i v2i;
		vec3i v3i;
	}data;
	message();
	message(string t, string d);
	template <typename T> void push(T val);
	template <typename T> T pop();
	
};

template <typename T> void message::push(T val) //always explicitly give <type> for: "string".
{
	cout << "[msg]pushing " + toString(&val);
	T* ptr = new T;
	*ptr = val;
	vdata.push_back((void*)ptr);
}
template <typename T> T message::pop()
{
	T *ptr = (T*)(vdata.back());
	T oldT = *ptr;
	vdata.pop_back();
	cout << "[msg]popping " << toString(ptr); 
	delete ptr;
	return oldT;
}

class PSsubscriber //interface for accepting messages
{
	public:
	virtual void PSreceive(message newMsg);
};

struct PSsubscriberPair 
{
	string type;
	PSsubscriber *subscriber;
};

struct PSchannel // no message caching yet.
{
	list<PSsubscriberPair> subscribers;
	list<PSsubscriberPair>::iterator I,E;
	void publish(message newMsg);
	void subscribe(string type, PSsubscriber *newSub);
	void unsubscribe(string type, PSsubscriber *oldSub);
};

void msgintercept(message msg);
#endif