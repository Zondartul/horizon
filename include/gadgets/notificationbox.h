#ifndef NOTIFICATIONBOX_GUARD
#define NOTIFICATIONBOX_GUARD
/*
struct notification{
	string text;
	int lifetime;		//dismissed after lifetime runs out (-1 for infinite) 
	vec3 color;
	int dismissable;	//means you can click on it to dismiss
}

notification *printNotification(string text, int lifetime = 5*60, vec3 color = {200,200,200}, int dismissable = true);
void dismissNotification(notification *N);

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
#endif