#ifndef GUI_EDITOR_GUARD
#define GUI_EDITOR_GUARD
#include "game.h"

class gui_editor_kind{
	public:
	GUIwindow *mainWindow;
	GUIbase *subject;
	
	gui_editor_kind();
};

extern gui_editor_kind *gui_editor;

void openGuiEditor();

#endif