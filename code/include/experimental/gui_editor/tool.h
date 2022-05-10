#ifndef GUI_EDITOR_TOOL_GUARD
#define GUI_EDITOR_TOOL_GUARD
class gui_editor_kind;
class GUIbase;

class gui_editor_tool{
	public:
	gui_editor_kind *Ed = 0;
	GUIbase *mouseover_element = 0;
	GUIbase *subject = 0;
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