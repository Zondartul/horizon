#include "gui_editor.h"
gui_editor_tool::gui_editor_tool(gui_editor_kind *Ed):Ed(Ed){}
void gui_editor_tool::scan(){
	GUIwindow *workWindow = 0;
	EPCAST(Ed->elWorkWindow, workWindow) else return;
	mouseover_element = workWindow->getMouseoverElement();
}
void gui_editor_tool::draw(){}
void gui_editor_tool::lup(){}
void gui_editor_tool::ldown(){}
void gui_editor_tool::rup(){}
void gui_editor_tool::rdown(){}
void gui_editor_tool::cleanup(){
	mouseover_element = 0;
	subject = 0;
}