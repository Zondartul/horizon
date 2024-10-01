#ifndef GUI_EDITOR_TOOL_EDIT_GUARD
#define GUI_EDITOR_TOOL_EDIT_GUARD
#include "experimental/gui_editor/tool.h"
#include "GUI/GUI_types.h"
#include "util/elastic_ptr.h"
#include "math/vec.h"

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
	void select(GUIbase *node);
	void deselect();
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