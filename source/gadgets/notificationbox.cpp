#include "notificationbox.h"
/*
notificationBox *currentNB;

struct notification{
	string text;
	int lifetime;		//dismissed after lifetime runs out (-1 for infinite) 
	vec3 color;
	int dismissable;	//means you can click on it to dismiss
};

struct notificationFrame{
	notification N;
	bool 
}

notification *printNotification(string text, int lifetime, vec3 color, int dismissable){
	notification *N = new notification();
	N->text = text;
	N->lifetime = lifetime;
	N->color = color;
	N->dismissable = dismissable;
	currentNB->notifications.push_back(N);
}

void dismissNotification(notification *N){
	
}

class notificationBox::public eventListener(){
	public:
	vector<string> strings;
	vec3 pos;
	vec3 size;
	notificationBox();
	~notificationBox();
	virtual notification *print(notification N); 	//adds the notification to cache
	virtual void render(); 							//draws the notifications
	virtual void onEvent(eventKind event);			//checks for frame and mouse events
}

void setNotificationBox(notificationBox *NB);
*/