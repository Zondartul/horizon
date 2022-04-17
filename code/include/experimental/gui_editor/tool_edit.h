#ifndef GUI_EDITOR_TOOL_EDIT_GUARD
#define GUI_EDITOR_TOOL_EDIT_GUARD

class gui_editor_tool_edit:public gui_editor_tool{
	public:
	enum {GEMT_START, GEMT_SUBJECT, GEMT_END} stage = GEMT_START;
	virtual void scan();	//called on mousemove events
	virtual void draw();	//called on mouse move
	virtual void lup();		//called on mouse click
	virtual void ldown();
	virtual void rup();
	virtual void rdown();
	gui_editor_tool_edit(gui_editor_kind *Ed);
	
	GUIbase *copyBuffer = 0;
	elastic_ptr<GUIbase> elDDM = 0;
	
	bool isValidSubject(GUIbase *el);
	
	int gridStep = 8;
	GUIe_border grabState;
	rect oldArea;
	vec2 grabPos;
};

#endif