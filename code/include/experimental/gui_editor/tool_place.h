#ifndef GUI_EDITOR_TOOL_PLACE_GUARD
#define GUI_EDITOR_TOOL_PLACE_GUARD

class gui_editor_tool_place:public gui_editor_tool{
	public:
	enum {GEMT_START, GEMT_END} stage = GEMT_START;
	enum submodeKind {
			GEMS_IDLE,	
			GEMS_FRAME,		GEMS_WINDOW, 
			GEMS_BUTTON,	GEMS_CHECKBOX, 
			GEMS_IMAGE, 	GEMS_LABEL, 
			GEMS_TEXTENTRY, GEMS_SCROLLBAR, 
			GEMS_TABS,		GEMS_SELECTIONGROUP, 
			GEMS_SELECTABLE,GEMS_GRID
		 } submode = GEMS_IDLE;

	gui_editor_tool_place(gui_editor_kind *Ed, submodeKind mode);
	
	virtual void scan();	//called on mousemove events
	virtual void draw();	//called on mouse move
	virtual void lup();		//called on mouse click
	virtual void rup();
	virtual void cleanup(); //reset the tool to initial state
	
	int gridStep = 8;
	vec2 gposCursor; bool drawVertCursor = false;
	vec2 gpos1;		 bool drawVert1 = false;
};

#endif
