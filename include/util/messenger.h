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
// 6) the sender erases the message

//message filtering:
//subscribing to type "" (empty string) will let you receive all messages.
//unsubscribing from type "" (empty string) will unsubscribe you from all types of messages.

struct message
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
};

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
}
struct MessageChannel;
class messageReceiver //interface for accepting messages
{
	public:
	virtual void receiveMessage(message *msg);
	void subscribeToMessageChannel(MessageChannel *channel, string type);
	void unsubscribeFromMessageChannel(MessageChannel *channel, string type);
};

struct messageReceiverPair 
{
	messageReceiver *subscriber;
	string type;
};

struct MessageChannel // no message caching yet.
{
	list<messageReceiverPair> subscribers;
	list<messageReceiverPair>::iterator I,E;
	void publish(message *msg);
	void addSubscriber(messageReceiver *newSub, string type);
	void removeSubscriber(messageReceiver *oldSub, string type);
};

void msgintercept(message msg);
#endif