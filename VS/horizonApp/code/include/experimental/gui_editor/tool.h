#ifndef GUI_EDITOR_TOOL_GUARD
#define GUI_EDITOR_TOOL_GUARD
#include "util/elastic_ptr.h"

class gui_editor_kind;
class GUIbase;
class gui_editor_tool{
	public:
	gui_editor_kind *Ed = 0;
	elastic_ptr<GUIbase> mouseover_element = 0;
	elastic_ptr<GUIbase> subject = 0;
	gui_editor_tool(gui_editor_kind *Ed);
	virtual void scan();	
	virtual void draw();	
	virtual void lup();		
	virtual void ldown();
	virtual void rup();
	virtual void rdown();
	virtual void cleanup(); 
};
#endif