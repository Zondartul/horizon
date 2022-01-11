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
	type = "";
	handled = false;
}
message::message(string type)
{
	this->type = type;
	handled = false;
}
/* message::~message(){
	for(int I = 0; I < data.size(); I++){
		if(data[I]){assert(!"message content has not beeen erase()ed");}
	}
} */
message *message::copy(){
	return new message(*this);
}
void messageReceiver::receiveMessage(message *msg){}
void messageReceiver::subscribeToMessageChannel(messageChannel *channel, string type){
	channel->addSubscriber(this, type);
}
void messageReceiver::unsubscribeFromMessageChannel(messageChannel *channel, string type){
	channel->removeSubscriber(this, type);
}

messageChannel::messageChannel(){
	stopWhenHandled = true;
}

void messageChannel::publish(message *msg){
	for(I = subscribers.begin(), E = subscribers.end(); I!=E;I++)
	{
		if((msg->type == I->type) || (I->type == ""))
		{
			//printf("msg to from %p to %p\n",this,I->subscriber);
			I->subscriber->receiveMessage(msg);
			if(msg->handled && stopWhenHandled){return;}
		}
	}
}
void messageChannel::addSubscriber(messageReceiver *newSub, string type){
	//removeSubscriber(newSub, type);
	subscribers.push_back({newSub, type});
	printf("MessageChannel %p now publishes to %p\n", this, newSub);
	//cout << toString(this) + " subscribed <" + toString(newSub) + "> for type [" + type + "]\n";
}
void messageChannel::removeSubscriber(messageReceiver *oldSub, string type){
	//cout << toString(this) + " unsubscribed <" + toString(oldSub) + "> for type [" + type + "]\n";
	printf("MessageChannel %p no longer publishes to %p\n", this, oldSub);
	bool found = false;
	for(I = subscribers.begin(), E = subscribers.end(); I!=E;I++)
	{
		if(((I->type == type) || (type == "")) && (I->subscriber == oldSub))
		{
			I = subscribers.erase(I);
			found = true;
		}
	}
	if(!found){printf("Warning: %p already doesn't publish to %p\n", this, oldSub);}
}

// debug func:
void msgintercept(message msg){
	cout << "msg.type = ["<<msg.type<<"]\n";//", name = ["<<msg.name<<"]\n";
}