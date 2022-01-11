// internal message broker - publish/subscribe system.
// A channel is created to store messages.
// Objects subscribe to a particular channel and type,
// or to all messages of that channel if type == "".
// Publisher objects publish a message to a channel,
// then channel delivers message to all matching subcribers.
// Subscribers need to have a PSsubscriber interface to receive messages.
#include "util/messenger.h"
#include "util/globals.h"

message::message()
{
	suspended = false;
}
message::message(string type, string name)
{
	this->type = type;
	this->name = name;
}
message::~message(){
	for(int I = 0; I < data.size(); I++){
		if(data[I]){assert(!"message content has not beeen erase()ed");}
	}
}

void messageReceiver::receiveMessage(message *msg){}
void messageReceiver::subscribeToMessageChannel(MessageChannel *channel, string type){
	channel->addSubscriber(this, type);
}
void messageReceiver::unsubscribeFromMessageChannel(MessageChannel *channel, string type){
	channel->removeSubscriber(this, type);
}


void MessageChannel::publish(message *msg){
	for(I = subscribers.begin(), E = subscribers.end(); I!=E;I++)
	{
		if((msg->type == I->type) || (I->type == ""))
		{
			I->subscriber->receiveMessage(msg);
			if(msg->suspended){return;}
		}
	}
}
void MessageChannel::addSubscriber(messageReceiver *newSub, string type){
	removeSubscriber(newSub, type);
	subscribers.push_back({newSub, type});
	cout << toString(this) + " subscribed <" + toString(newSub) + "> for type [" + type + "]\n";
}
void MessageChannel::removeSubscriber(messageReceiver *oldSub, string type){
	for(I = subscribers.begin(), E = subscribers.end(); I!=E;I++)
	{
		if(((I->type == type) || (type == "")) && (I->subscriber == oldSub))
		{
			I = subscribers.erase(I);
		}
	}
}

// debug func:
void msgintercept(message msg){
	cout << "msg.type = ["<<msg.type<<"], name = ["<<msg.name<<"]\n";
}