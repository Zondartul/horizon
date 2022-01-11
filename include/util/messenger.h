#ifndef MESSENGER_GUARD
#define MESSENGER_GUARD 
#include "../util/globals.h"
#include "assert.h"
// How to use the message system:
// 1) create a message channel
// 2) the receiver calls subscribeToMessageChannel(channel *, string type)
// 3) the sender creates a message
// 4) the sender calls channel.publish(message *)
// 5) if the message is of the type for which receiver subscribed, receiver's receiveMessage(message *) is called
// 6) the receiver may cast the message to a derived message type to get more information
// 7) if the receiver sets the message's "handled" flag, the message will not be delivered to other subscribers.
// 8) messages are cleaned up automatically (RAII)

//message filtering:
//subscribing to type "" (empty string) will let you receive all messages.
//unsubscribing from type "" (empty string) will unsubscribe you from all types of messages.

//messages may be sub-typed to send additional information.
class message{
	public:
	string type;				//receivers filter messages by type
	bool handled;				//"handled" usually means the message will stop going around.
	message();
	message(string type);
	virtual message *copy();	//constructs a copy of message in the heap, preserving derived data
};

//inherit from this to use the copy function.
// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
template<class Derived>
class message_CRTP:public message{
	public:
	virtual message *copy(){
		return new Derived(static_cast<Derived const&>(*this));
	}
};
// Nice macro which ensures correct CRTP usage (according to wiki)
#define Derive_message_CRTP(Type) class Type: public message_CRTP<Type>

/* struct message
{
	//type of message. The channel filters messages by type.
	string type;
	//message name. No effect.
	string name;
	//Set to true to indicate that the message has been dealt with and no further
	//action is required.
	bool handled;
	//typeless data is stored here by pointer.
	vector<void*> data;
	//create an empty message of type ""
	message();
	//create an empty message of given type and name
	message(string type, string name);
	//the destructor checks if erasure has properly happened.
	~message();
	//write content into the message
	template <typename T> void set(int I, T val);
	//read content from the message
	template <typename T> T get(int I);
	//erase content from the message
	template <typename T> void erase(int I); 
	//template <typename T> void push(T val);
	//template <typename T> T pop();
}; */
/* 
template <typename T> void message::set(int I, T val){
	//if I is outside of bounds, error
	if((I < 0) || (I > data.size())){assert(!"message index out of bounds");}
	//if I is the element-after-the-last index, expand the vector.
	if(I == data.size()){
		data.push_back(NULL);
	}
	//if the element already exists, error
	if(data[I]){assert(!"message content has not beeen erase()ed");}
	//assign a new occupant
	T *P = new T;
	*P = val;
	data[I] = P;
	//done
}

template <typename T> T message::get(int I){
	//if I is outside of bounds, error
	if((I < 0) || (I >= data.size())){assert(!"message index out of bounds");}
	//return a copy of the element
	return *((T*)data[I]);
}

template <typename T> void message::erase(int I){
	//if I is outside of bounds, error
	if((I < 0) || (I >= data.size())){assert(!"message index out of bounds");}
	//erase the element
	delete ((T*)data[I]);
	data[I] = NULL;
} */
struct messageChannel;
class messageReceiver //interface for accepting messages
{
	public:
	virtual void receiveMessage(message *msg);
	void subscribeToMessageChannel(messageChannel *channel, string type);
	void unsubscribeFromMessageChannel(messageChannel *channel, string type);
};

struct messageReceiverPair 
{
	messageReceiver *subscriber;
	string type;
};

class messageChannel // no message caching yet.
{
	public:
	list<messageReceiverPair> subscribers;
	list<messageReceiverPair>::iterator I,E;
	bool stopWhenHandled;		//if a message has been handled, should we keep sending it?
	messageChannel();
	void publish(message *msg);
	void addSubscriber(messageReceiver *newSub, string type);
	void removeSubscriber(messageReceiver *oldSub, string type);
};

void msgintercept(message msg);
#endif