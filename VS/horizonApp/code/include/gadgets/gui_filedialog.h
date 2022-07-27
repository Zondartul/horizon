#ifndef GUI_FILEDIALOG_H
#define GUI_FILEDIALOG_H
#include "GUI/GUI.h"

class GUIfileDialog: public GUIwindow{
	public:
	GUIbutton *btn_up;
	GUItextEntry *address_field;
	GUIscrollbar *scroll;
	GUItextEntry *select_field;
	GUIbutton *btn_select;
	GUIe_fileop mode = GUIf::Open;
	function<void(string)> func;
	GUIfileDialog();
	GUIfileDialog &setDirectory(string dir);
	GUIfileDialog &setMode(GUIe_fileop newmode);
	GUIfileDialog &setFunction(function<void(string)> f);
	private:
	vector<GUIbase*> elements;
};

texture* getFileIcon(string filename);

#endif