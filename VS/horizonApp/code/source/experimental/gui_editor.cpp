#include "experimental/gui_editor.h"
#include "gadgets/gui_filedialog.h"
//#include "Ext/tinyXML2/tinyxml2.h"
#include "GUI/GUI_property.h"
#include "program/file.h"
#include "input/input.h"
#include "render/paint.h"
#include "resource/resource.h"
#include "resource/bitmap.h"
#include "util/global_vars_app.h"
#include "util/global_vars_gui.h"
#include "util/global_vars_render.h"
#include "util/global_vars_program.h"
#include "util/global_vars_util.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
using namespace tinyxml2;
using namespace std;

gui_editor_kind::gui_editor_kind(){
	add_windows();
	setup_layers();
	helper.editor = this;
	setup_channels();
	tool_default();
}

GUIbase* add_window(std::string title, vec2 size, vec2 pos) {
	auto& GUI = Gg->gs_GUI->g_GUI;
	GUIwindow* win = new GUIwindow();
	win->setTitle(title);
	win->setSize(size);
	win->moveTo(pos);
	GUI->addChild(win);
	return win;
}

void gui_editor_kind::add_windows() {
	elMainWindow = add_window("GUI editor", vec2(100, 400), vec2(0, 0));
	add_tool_buttons();
	elWorkWindow = add_window("GUI workspace", vec2(500, 500), vec2(300, 100));
	elNodegraph = add_window("GUI graph", vec2(150, 400), vec2(150, 100));
}

void gui_editor_kind::setup_channels() {
	auto& inputChannel = Gp->sysInput->inputChannel;
	auto& globalChannel = Gu->sysEvent->globalChannel;
	inputChannel.addListener(this);
	inputChannel.addListenerFront(&helper);
	globalChannel.addListener(this);
}

void gui_editor_kind::setup_layers() {
	auto& layerGUI = Gg->gs_GUI->g_layerGUI;
	auto& layer2D = Gr->gs_paint->g_layer2D;
	setLayer(layer2D);
	ge_layer_back = addNewLayer("gui_editor_layer_back", false);
	setLayer(layerGUI);
	ge_layer_front = addNewLayer("gui_editor_layer_front", false);
	ge_layer_front->resetLayer = addNewLayer("gui_editor_layer_front.reset", true, true);
	setLayer(ge_layer_front->resetLayer);
	setScissor(rect(vec2(0, 0), vec2(1024, 1024)));
	setScissoring(false);
	setAlpha(255.0f);
	setColor(vec3(0, 0, 0));
}


GUIbutton* add_button(GUIbase& parent, std::string text, std::string texname, vec2 size, vec2 pos, std::function<void()> func) {
	GUIbutton* btn = new GUIbutton();
	btn->setText(text);
	auto tex = getTexture(texname);
	if(!tex.ok()){push(*tex.err);}
	btn->setImage(tex.val());
	btn->setSize(size);
	btn->setFunction(func);
	btn->moveTo(pos);
	parent.addChild(btn);
	return btn;
}

void not_implemented() { std::cout << "- not implemented -" << std::endl; }

void gui_editor_kind::add_tool_buttons() {
	btnFile = add_button(*elMainWindow, "File", "", vec2(40, 20), vec2(0, 0), std::bind(&gui_editor_kind::on_btn_File_pressed, this));
	add_button(*elMainWindow, "", "gui/iconframe", vec2(32, 32), vec2(0, 32 * 1), std::bind(&gui_editor_kind::on_btn_Frame_pressed, this));
	add_button(*elMainWindow, "", "gui/iconwindow", vec2(32, 32), vec2(0, 32 * 2), std::bind(&gui_editor_kind::on_btn_Window_pressed, this));
	add_button(*elMainWindow, "", "gui/iconbutton", vec2(32, 32), vec2(0, 32 * 3), std::bind(&gui_editor_kind::on_btn_Btn_pressed, this));
	add_button(*elMainWindow, "", "gui/iconcheckbox", vec2(32, 32), vec2(0, 32 * 4), not_implemented);
	add_button(*elMainWindow, "", "gui/iconlabel", vec2(32, 32), vec2(0, 32 * 5), std::bind(&gui_editor_kind::on_btn_Label_pressed, this));
	add_button(*elMainWindow, "", "gui/icontextentry", vec2(32, 32), vec2(0, 32 * 6), std::bind(&gui_editor_kind::on_btn_TextEntry_pressed, this));
	add_button(*elMainWindow, "", "gui/iconimage", vec2(32, 32), vec2(0, 32 * 7), std::bind(&gui_editor_kind::on_btn_Image_pressed, this));
	add_button(*elMainWindow, "", "gui/iconscrollbar", vec2(32, 32), vec2(0, 32 * 8), std::bind(&gui_editor_kind::on_btn_ScrollBar_pressed, this));
	add_button(*elMainWindow, "", "gui/icontabs", vec2(32, 32), vec2(0, 32 * 9), std::bind(&gui_editor_kind::on_btn_Tabs_pressed, this));
	add_button(*elMainWindow, "", "gui/cursor", vec2(32, 32), vec2(32, 32 * 1), std::bind(&gui_editor_kind::on_btn_Cursor_pressed, this));
	add_button(*elMainWindow, "", "gui/iconsave", vec2(32, 32), vec2(32, 32 * 2), std::bind(&gui_editor_kind::on_btn_Save_pressed, this));
	add_button(*elMainWindow, "", "gui/iconvertex", vec2(32, 32), vec2(32, 32 * 3), std::bind(&gui_editor_kind::on_btn_Paste_pressed, this));
	add_button(*elMainWindow, "", "gui/iconselectiongroup", vec2(32, 32), vec2(32, 32 * 4), std::bind(&gui_editor_kind::on_btn_SelectionGroup_pressed, this));
	add_button(*elMainWindow, "", "gui/iconselectable2", vec2(32, 32), vec2(32, 32 * 5), std::bind(&gui_editor_kind::on_btn_Selectable_pressed, this));
	add_button(*elMainWindow, "", "gui/icongrid", vec2(32, 32), vec2(32, 32 * 6), std::bind(&gui_editor_kind::on_btn_Grid_pressed, this));
}


void gui_editor_kind::on_btn_File_pressed() {
	auto& GUI = Gg->gs_GUI->g_GUI;

	auto *ddm = new GUIdropdownMenu();
	ddm->addItem("New", std::bind(&gui_editor_kind::on_btn_New_pressed, this));
	ddm->addItem("Open...", std::bind(&gui_editor_kind::on_btn_Paste_pressed, this));
	ddm->addItem("Save...", std::bind(&gui_editor_kind::on_btn_Save_pressed,this));
	ddm->btnMenu = btnFile;
	elMainWindow->addChild(ddm);
}

void gui_editor_kind::on_btn_New_pressed(){
	GUIwindow* workWindow;
	EPCAST(elWorkWindow, workWindow) else return;
	for(auto ch:workWindow->children){
		if(ch->isClient){
			ch->close();
		}
	}
	update_nodegraph();
}

void gui_editor_kind::on_btn_Save_pressed() {
	auto& GUI = Gg->gs_GUI->g_GUI;
	GUIfileDialog* dialog = new GUIfileDialog();
	dialog->setDirectory(getDataDirectory());
	dialog->setMode(GUIf::Save);
	dialog->setFunction([=](string filepath) {
		save(filepath);
		});
	GUI->addChild(dialog);
}
void gui_editor_kind::on_btn_Paste_pressed() {
	auto& GUI = Gg->gs_GUI->g_GUI;
	GUIfileDialog* dialog = new GUIfileDialog();
	dialog->setDirectory(getDataDirectory());
	dialog->setMode(GUIf::Open);
	dialog->setFunction([=](string filepath) {
		paste(filepath);
		});
	GUI->addChild(dialog);
}

void gui_editor_kind::on_btn_Frame_pressed() { tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_FRAME)); }
void gui_editor_kind::on_btn_Window_pressed() { tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_WINDOW)); }
void gui_editor_kind::on_btn_Btn_pressed() { tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_BUTTON)); }
void gui_editor_kind::on_btn_Label_pressed() { tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_LABEL)); }
void gui_editor_kind::on_btn_TextEntry_pressed() { tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_TEXTENTRY)); }
void gui_editor_kind::on_btn_Image_pressed() { tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_IMAGE)); }
void gui_editor_kind::on_btn_ScrollBar_pressed() { tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_SCROLLBAR)); }
void gui_editor_kind::on_btn_Tabs_pressed() { tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_TABS)); }
void gui_editor_kind::on_btn_Cursor_pressed() { tool_none(); }
void gui_editor_kind::on_btn_SelectionGroup_pressed() { tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_SELECTIONGROUP)); }
void gui_editor_kind::on_btn_Selectable_pressed() { tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_SELECTABLE)); }
void gui_editor_kind::on_btn_Grid_pressed() { tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_GRID)); }




void gui_editor_kind::close(){
	printf("closing gui editor\n");
	if(elWorkWindow){
		printf("closing workWindow\n");
		//GUIbase *B = elWorkWindow;
		//delete B;
		elWorkWindow->close();
	}
	if(elMainWindow){
		printf("closing mainWindow\n");
		//GUIbase *B = elMainWindow;
		//delete B;
		elMainWindow->close();
	}
	if(elNodegraph){
		printf("closing nodeGraph\n");
		elNodegraph->close();
	}
}
void openGuiEditor(){
	auto& gui_editor = Ga->gs_gui_editor->g_gui_editor;
	if(gui_editor){
		printf("closing gui editor\n");
		gui_editor->close();
		//printf("deleting gui editor\n");
		//delete gui_editor;
	}
	gui_editor = new gui_editor_kind();
	printf("opened gui editor\n");
}
void gui_editor_kind::save(string filepath){
	printf("gui_editor_kind::save()\n");
	XMLDocument doc;
	GUIwindow *workWindow = 0;
	if(elWorkWindow){
		workWindow = dynamic_cast<GUIwindow*>(&*elWorkWindow);
	}else{return;}
	auto P = workWindow->getCompoundProperty();
	XMLElement *el = P.toXML(&doc);
	doc.InsertEndChild(el);
	/*XMLError eResult = */ doc.SaveFile(filepath.c_str());
}
void gui_editor_kind::paste(string filepath){
	try {
		GUIwindow* workWindow;
		EPCAST(elWorkWindow, workWindow) else return;
		printf("gui_editor_kind::paste()\n");
		XMLDocument doc;
		if (!fileReadable(filepath)) { 
			//error("can't read file [%s]\n", filepath.c_str()); 
			stringstream ss;
			ss << "can't read file [" << filepath << "]\n";
			throw std::runtime_error(ss.str());
		}
		doc.LoadFile(filepath.c_str());
		XMLElement* el = doc.RootElement();
		GUIcompoundProperty* P = new GUIcompoundProperty(el);
		for (auto I = P->children.begin(); I != P->children.end(); I++) {
			const GUIcompoundProperty& P2 = *I;
			if (P2.table.table.at("isClient") == "1") {
				GUIbase* B = P2.instantiate();
				workWindow->addChild(B);
			}
		}
		delete P;
		update_nodegraph();
	}
	catch (...) { 
		cout << "gui_editor: could not paste" << endl;
	}
}
void gui_editor_kind::tool_cleanup(){  
	if(tool){tool->cleanup();}
}
void gui_editor_kind::tool_start(gui_editor_tool *newtool){   
	if(lastTool){delete lastTool; lastTool = 0;}
	lastTool = tool;
	tool = newtool;
	elWorkWindow->blockChildInput = true;
}
void gui_editor_kind::tool_cancel(){   
	tool_cleanup();
	tool_default();
}
void gui_editor_kind::tool_restart(){  
	tool_cleanup();
}
void gui_editor_kind::tool_finished(){ 
	tool_restart();
}
void gui_editor_kind::tool_default(){
	tool_start(new gui_editor_tool_edit(this));
}
void gui_editor_kind::tool_none(){
	tool_cancel();
	if(tool){delete tool; tool = 0;}
	elWorkWindow->blockChildInput =  false;
}
void gui_editor_kind::onEvent(eventKind event){
	if((!elWorkWindow) || (!elMainWindow) || (!elNodegraph)){
		close();
		delete this;
		return;
	}
	GUIwindow *workWindow = 0;
	EPCAST(elWorkWindow, workWindow);
	if(event.type == EVENT_FRAME){
		resetLayer(ge_layer_front);
		rect R = workWindow->thisToWorld(workWindow->clientArea);
		setLayer(ge_layer_back);
		setColor(vec3(255,255,255));
		drawRect(R);
		setLayer(ge_layer_front);
	}
	switch(event.type){
		case EVENT_MOUSE_MOVE: if(tool){tool->scan();} break;
		case EVENT_MOUSE_BUTTON_DOWN:
			if(!isFrontEvent){break;} 
			if(event.mousebutton.button == MOUSE_LEFT) {if(tool){tool->ldown();}}
			if(event.mousebutton.button == MOUSE_RIGHT){if(tool){tool->rdown();}}
		break;
		case EVENT_MOUSE_BUTTON_UP:
			if(!isFrontEvent){break;} 
			if(event.mousebutton.button == MOUSE_LEFT) {if(tool){tool->lup();}}
			if(event.mousebutton.button == MOUSE_RIGHT){if(tool){tool->rup();}}
		break;
		case EVENT_FRAME: if(tool){tool->draw();} break;
	}
}
void gui_editor_event_helper::onEvent(eventKind event){
	switch(event.type){
		case EVENT_MOUSE_BUTTON_DOWN:
		case EVENT_MOUSE_BUTTON_UP:
			editor->isFrontEvent = true;
			editor->onEvent(event);
			editor->isFrontEvent = false;
		break;
	}
}
void drawGrid(GUIbase *element, float gridStep){
	rect R = element->thisToWorld(element->clientArea);
	setPointSize(1.f);
	vector<vec3> points;
	for(float y = R.start.y+gridStep; y < R.end.y; y += gridStep){
		for(float x = R.start.x+gridStep; x < R.end.x; x += gridStep){
			points.push_back(vec3(x,y,0));
		}
	}
	setColor(vec3(0,0,0));
	drawPoints(points);
}
vec2 snapToGrid(GUIbase *element, vec2 pos, float gridStep){
	vec2 corner = element->worldClientArea().start;
	pos -= corner;
	pos += vec2(0.5f*gridStep,0.5f*gridStep);
	pos.x = pos.x - fmodf(pos.x, gridStep);
	pos.y = pos.y - fmodf(pos.y, gridStep);
	return pos;
}
rect snapToGrid(GUIbase *element, rect R, float gridStep){
	return rect(snapToGrid(element, R.start, gridStep),
				snapToGrid(element, R.end, gridStep));
}
vec2 snapToGridToWorld(GUIbase *element, vec2 pos, float gridStep){
	vec2 corner = element->worldClientArea().start;
	return snapToGrid(element,pos,gridStep)+corner;
}
rect snapToGridToWorld(GUIbase *element, rect R, float gridStep){
	return rect(snapToGridToWorld(element, R.start, gridStep),
				snapToGridToWorld(element, R.end, gridStep));
}

void gui_editor_kind::nodegraph_add_children_rec(GUIbase *node){
	for(auto ch:node->getChildren()){
		std::cout << "nodegraph: add node [" << ch->getType() << "][" << ch->name << "]" << std::endl;
		GUIbutton *node = new GUIbutton();
		node->setText(ch->getType());
		elNodegraph->addChild(node);

		nodegraph_add_children_rec(ch);
	}
}

void gui_editor_kind::update_nodegraph(){
	std::cout << "update_nodegraph(): hi" << std::endl;
	for(auto ch:elNodegraph->getChildren()){
		ch->close();
	}

	nodegraph_add_children_rec((GUIbase*)elWorkWindow);
}