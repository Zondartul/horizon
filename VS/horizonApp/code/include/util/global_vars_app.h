#pragma once

class GlobalsApp {
public:
	GlobalsApp();
	struct gs_map_editorKind* gs_map_editor;
	struct gs_consoleKind* gs_console;
	//struct gs_commandsKind* gs_commands;
	struct gs_editor2Kind* gs_editor2;
	struct gs_tool_editKind* gs_tool_edit;
	struct gs_gui_editorKind* gs_gui_editor;
	struct gs_pickerKind* gs_picker;
	struct gs_commandsKind* gs_commands;
};

extern GlobalsApp *Ga;
