// internal message broker - publish/subscribe system.
// A channel is created to store messages.
// Objects subscribe to a particular channel and type,
// or to all messages of that channel if type == "".
// Publisher objects publish a message to a channel,
// then channel delivers message to all matching subcribers.
// Subscribers need to have a PSsubscriber interface to receive messages.

struct message
{
	string type;
	list<void*> data;
	message()
	{
	}
	message(string t, string d)
	{
		type = t;
		push<string>(d);
	}
	void pushString(string newStr)
	{
		//printf("s1\n");
		string* strPtr = new string;
		//printf("s2\n");
		*strPtr = newStr;
		//printf("s3\n");
		data.push_back((void*)strPtr);
		//printf("s4\n");
	}
	string popString()
	{
		//printf("r1\n");
		void *dat = data.back();
		string* strPtr = (string*)dat;
		//printf("r2\n");
		//printf("dat = %p",dat);
		string oldStr = *strPtr;
		//printf("r3\n");
		data.pop_back();
		//printf("r4\n");
		delete strPtr;
		//printf("r5\n");
		return oldStr;
	}
	template <typename T> void push(T val) //always explicitly give <type> for: "string".
	{
		//cout << "pushing " + toString(&val);
		T* ptr = new T;
		*ptr = val;
		data.push_back((void*)ptr);
	}
	template <typename T> T pop()
	{
		T *ptr = (T*)(data.back());
		T oldT = *ptr;
		data.pop_back();
		delete ptr;
		return oldT;
	}
};

class PSsubscriber //interface for accepting messages
{
	public:
	virtual void PSreceive(message newMsg){}
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
	
	
	void publish(message newMsg)
	{
		//printf("%s received message %s, type [%s]\n",toString(this),toString(newMsg), newMsg.type);
		//cout << toString(this) + " received message " + toString(&newMsg) + ", type [" + newMsg.type + "]\n";
		//cout << "num subscribers: " << subscribers.size() << "\n";
		for(I = subscribers.begin(), E = subscribers.end(); I!=E;I++)
		{
			if((newMsg.type == I->type) || (I->type == ""))
			{
				I->subscriber->PSreceive(newMsg);
				//printf("%s dispatched message %s to %s\n",toString(this),toString(newMsg),toString(I->subscriber));
				//cout << toString(this) + " dispatched message " + toString(&newMsg) + " to " + toString(I->subscriber);
			}
		}
	}
	void subscribe(string type, PSsubscriber *newSub)
	{
		subscribers.push_back({type, newSub});
		//printf("%s subscribed %s for type [%s]\n",toString(this),toString(newSub),type);
		cout << toString(this) + " subscribed <" + toString(newSub) + "> for type [" + type + "]\n";
	}
	void unsubscribe(string type, PSsubscriber *oldSub)
	{
		for(I = subscribers.begin(), E = subscribers.end(); I!=E;I++)
		{
			if(((I->type == type) || (type == "")) && (I->subscriber = oldSub))
			{
				I = subscribers.erase(I);
				//printf("%s unsubscribed %s from type [%s]\n",toString(this),toString(oldSub),type);
				//cout << toString(this) + " unsubscribed " + toString(oldSub) + " fro type [" + type + "]\n";
			}
		}
	}
};