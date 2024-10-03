#ifndef GUI_EDITOR_TOOL_POKE_GUARD
#define GUI_EDITOR_TOOL_POKE_GUARD
#include "util/elastic_ptr.h"

// this whole tool exists as a hack to capture "close button" and
// update the nodegraph, cause we don't yet have signals.

class gui_editor_kind;
class GUIbase;
class gui_editor_tool_poke: public gui_editor_tool{
	public:
	gui_editor_tool_poke(gui_editor_kind *Ed);
    void lup();
};
#endif