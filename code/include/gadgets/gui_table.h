#ifndef GUI_TABLE_GUARD
#define GUI_TABLE_GUARD
#include "GUI.h"
/*
	Gui table - a widget that shows a table of labels and value entries.
*/	

struct GUItableInfo{
	GUIpropertyTable entries;			
	map<string, GUIbase*> entry_widgets; 
};
	

class GUItable: public GUIwindow{
	public:
	
	function<void(string, string)> f_entryChanged;
	
	GUItableInfo table;
	
	GUIscrollbar *bar;
	GUIgrid *grid;
	
	GUItable();
	GUItable(GUIpropertyTable table);
	GUItable &setTable(GUIpropertyTable table);
	void invalidate();
	GUItable &setFunction(function<void(string, string)> f);
};
	
	
#endif
