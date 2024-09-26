#ifndef GUI_EDITOR_GUARD
#define GUI_EDITOR_GUARD
#include "game/game.h"
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
	elastic_ptr<GUIbase> elNodegraph = 0;
	renderLayer *ge_layer_back = 0;
	renderLayer *ge_layer_front = 0;
	gui_editor_tool *lastTool = 0;
	gui_editor_tool *tool = 0;
	GUIbutton *btnFile = 0;
	gui_editor_kind();
	void close();
	void onEvent(eventKind event);
	void tool_start(gui_editor_tool *newtool); 
	void tool_cleanup();  
	void tool_cancel();   
	void tool_restart();  
	void tool_finished(); 
	void tool_default();  
	void tool_none();	  
	bool isValidSubject(GUIbase *el);
	void place_tool_scan(); 
	void place_tool_draw(); 
	void place_tool_lclick(); 
	void place_tool_rclick(); 
	void edit_tool_mousemove();
	void edit_tool_draw();
	void edit_tool_lup();
	void edit_tool_ldown();
	void edit_tool_rclick();
	void save(string filepath); 
	void paste(string filepath); 
	bool isFrontEvent = false;
	gui_editor_event_helper helper;

	void on_btn_File_pressed();
	void on_btn_New_pressed();
	void on_btn_Save_pressed();
	void on_btn_Paste_pressed();
	void on_btn_Frame_pressed();
	void on_btn_Window_pressed();
	void on_btn_Btn_pressed();
	void on_btn_Label_pressed();
	void on_btn_TextEntry_pressed();
	void on_btn_Image_pressed();
	void on_btn_ScrollBar_pressed();
	void on_btn_Tabs_pressed();
	void on_btn_Cursor_pressed();
	void on_btn_SelectionGroup_pressed();
	void on_btn_Selectable_pressed();
	void on_btn_Grid_pressed();

	void setup_layers();
	void add_tool_buttons();
	void add_windows();
	void setup_channels();
};

struct gs_gui_editorKind {
	elastic_ptr<gui_editor_kind> g_gui_editor;
};
void openGuiEditor();
void drawGrid(GUIbase *element, float gridStep);
vec2 snapToGrid(GUIbase *element, vec2 pos, float gridStep);
rect snapToGrid(GUIbase *element, rect R, float gridStep);
vec2 snapToGridToWorld(GUIbase *element, vec2 pos, float gridStep);
rect snapToGridToWorld(GUIbase *element, rect R, float gridStep);
#endif