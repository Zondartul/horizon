#ifndef GUI_EDITOR_GUARD
#define GUI_EDITOR_GUARD
#include "game.h"
#include "gadgets/gui_table.h"
#include "gui_editor/tool.h"
#include "gui_editor/tool_place.h"
#include "gui_editor/tool_edit.h"

class gui_editor_kind;
class gui_editor_event_helper:public eventListener{
	public:
	gui_editor_kind *editor;
	void onEvent(eventKind event);
};

class gui_editor_kind:public eventListener, public elastic_ptr_anchor{
	public:
	elastic_ptr<GUIbase> elMainWindow = 0;
	elastic_ptr<GUIbase> elWorkWindow = 0;
	//GUIbase *mouseover_element = 0;
	//GUIbase *subject = 0;
	renderLayer *ge_layer_back = 0;
	renderLayer *ge_layer_front = 0;
	
	gui_editor_tool *lastTool = 0;
	gui_editor_tool *tool = 0;
	gui_editor_kind();
	void close();
	
	void onEvent(eventKind event);
	
	//----- tool selection -----------
	
	void tool_start(gui_editor_tool *newtool); //select a tool
	void tool_cleanup();  //to be called after every application of a tool
	void tool_cancel();   //de-select the tool
	void tool_restart();  //re-select the previous tool
	void tool_finished(); //tool signals that it is done
	void tool_default();  //select the default tool
	void tool_none();	  //de-select all tools
	// ---------------
	//returns true for client children of workWindow, or their children. false otherwise.
	bool isValidSubject(GUIbase *el);
	//----place widget tool --------
	void place_tool_scan(); //on mousemove
	void place_tool_draw(); //on frame
	void place_tool_lclick(); //left click = place
	void place_tool_rclick(); //right click = cancel
	//----edit widget tool ---------
	void edit_tool_mousemove();
	void edit_tool_draw();
	void edit_tool_lup();
	void edit_tool_ldown();
	void edit_tool_rclick();
	
	//------------------------------
	void save(string filepath); //saves the currently drawn widgets to file
	void paste(string filepath); //instantiates the current saved widget
	//------------------------------
	bool isFrontEvent = false;
	gui_editor_event_helper helper;
	
};

struct gs_gui_editorKind {
	elastic_ptr<gui_editor_kind> g_gui_editor;
};

//extern elastic_ptr<gui_editor_kind> g_gui_editor;

void openGuiEditor();

void drawGrid(GUIbase *element, float gridStep);
//returns snapped position relative to the start of the grid
//takes world, outputs client coords
vec2 snapToGrid(GUIbase *element, vec2 pos, float gridStep);
//same but for rects
rect snapToGrid(GUIbase *element, rect R, float gridStep);
//returns snapped position in absolute coordinates
//takes world, outputs world coords
vec2 snapToGridToWorld(GUIbase *element, vec2 pos, float gridStep);
rect snapToGridToWorld(GUIbase *element, rect R, float gridStep);

#endif