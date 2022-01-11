#ifndef GUI_DROPDOWNMENU_GUARD
#define GUI_DROPDOWNMENU_GUARD

class GUIdropdownMenu:public GUIframe{
	public:
	vector<GUIdropdownMenu*> submenus;
	GUIbutton *btnMenu = 0; //the button that opened this menu
	int hideCounter = 0; //distance*time away from menu
	GUIdropdownMenu();
	bool constructed = false;
	GUIbutton *addItem(string text);
	GUIbutton *addItem(string text, function<void()> func);
	GUIdropdownMenu *addSubmenu(string text);
	GUIgrid *grid;
	virtual void invalidate();
	//void close();
	void onEvent(eventKind event);
	float getMouseDist();
	virtual string getType();
};

#endif