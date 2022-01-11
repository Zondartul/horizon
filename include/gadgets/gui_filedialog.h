#ifndef GUI_FILEDIALOG_H
#define GUI_FILEDIALOG_H
#include "gui.h"

class GUIfileDialog: public GUIwindow{
	public:
	
	GUIbutton *btn_up;
	GUItextEntry *address_field;
	GUIscrollbar *scroll;
	GUItextEntry *select_field;
	GUIbutton *btn_select;
	
	GUIfileDialog();
	GUIfileDialog &setDirectory(string dir);
	private:
	vector<GUIbase*> elements;
};

#endif