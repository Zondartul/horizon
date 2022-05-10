#ifndef GUI_EDITOR_TOOL_EDIT_GUARD
#define GUI_EDITOR_TOOL_EDIT_GUARD
#include "tool.h"
#include "GUI_types.h"
#include "elastic_ptr.h"
#include "vec.h"
class GUIbase;
class gui_editor_tool_edit:public gui_editor_tool{
	public:
	enum {GEMT_START, GEMT_SUBJECT, GEMT_END} stage = GEMT_START;
	virtual void scan();	
	virtual void draw();	
	virtual void lup();		
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
struct gs_tool_editKind {
	int g_numDDMs = 0;
};
#endif