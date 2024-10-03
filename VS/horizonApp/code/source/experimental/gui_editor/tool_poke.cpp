#include "experimental/gui_editor.h"
#include "util/timer.h"
gui_editor_tool_poke::gui_editor_tool_poke(gui_editor_kind *Ed):gui_editor_tool(Ed){}

void gui_editor_tool_poke::lup(){
    ON_NEXT_FRAME(Ed->update_nodegraph());
}