#include "gui_editor.h"
#include "gui_filedialog.h"
#include "file.h"
#include "resource/bitmap.h"
#include "tinyxml2.h"
#include <iostream>
using namespace tinyxml2;
using namespace std;

// V-SWEET --0. make gui editor not crash when closing
// v - 1. object selection
//	v - 1.1 make a click-screen/gate widget OR an object property that stops clickthrough.
// v - 2. object deletion
// v - 3. fix selectable selection
// v - 4. fix second object placement
// v - 5. make a "poke" tool

//elastic_ptr<gui_editor_kind> g_gui_editor;
//extern renderLayer *g_layerGUI;

gui_editor_kind::gui_editor_kind(){
	auto& GUI = G->gs_main->g_GUI;
	auto& layerGUI = G->gs_main->g_layerGUI;
	auto& layer2D = G->gs_paint->g_layer2D;
	auto& inputChannel = G->gs_input->g_inputChannel;
	auto& globalChannel = G->gs_event->g_globalChannel;

	GUIwindow *mainWindow = new GUIwindow();
	elMainWindow = mainWindow;
	
	mainWindow->setTitle("GUI editor");
	mainWindow->setSize(vec2(100,400));
	
	GUIbutton *btn1 = new GUIbutton();
	btn1->setText("File");
	btn1->setSize(vec2(40,20));
	btn1->setFunction([=]() {
		GUIfileDialog* dialog = new GUIfileDialog();
		dialog->setDirectory(getDataDirectory());
		dialog->setMode(GUIf::Open);
		dialog->setFunction([=](string filepath) {
			paste(filepath);
			});
		GUI->addChild(dialog);
		}
	);
	mainWindow->addChild(btn1);
	
	GUIbutton *btnFrame = new GUIbutton();
	btnFrame->setText("");
	btnFrame->setSize(vec2(32,32));
	btnFrame->setImage(getTexture("gui/iconframe"));
	btnFrame->moveTo(vec2(0,32*1));
	btnFrame->setFunction([=](){
		tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_FRAME));
	});
	mainWindow->addChild(btnFrame);
	
	GUIbutton *btnWindow = new GUIbutton();
	btnWindow->setText("");
	btnWindow->setSize(vec2(32,32));
	btnWindow->setImage(getTexture("gui/iconwindow"));
	btnWindow->moveTo(vec2(0,32*2));
	btnWindow->setFunction([=](){
		tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_WINDOW));
	});
	mainWindow->addChild(btnWindow);
	
	GUIbutton *btnBtn = new GUIbutton();
	btnBtn->setText("");
	btnBtn->setSize(vec2(32,32));
	btnBtn->setImage(getTexture("gui/iconbutton"));
	btnBtn->moveTo(vec2(0,32*3));
	btnBtn->setFunction([=](){
		tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_BUTTON));
	});
	mainWindow->addChild(btnBtn);
	
	GUIbutton *btnCheckbox = new GUIbutton();
	btnCheckbox->setText("");
	btnCheckbox->setSize(vec2(32,32));
	btnCheckbox->setImage(getTexture("gui/iconcheckbox"));
	btnCheckbox->moveTo(vec2(0,32*4));
	btnCheckbox->setFunction([=](){
	});
	mainWindow->addChild(btnCheckbox);
	
	GUIbutton *btnLabel = new GUIbutton();
	btnLabel->setText("");
	btnLabel->setSize(vec2(32,32));
	btnLabel->setImage(getTexture("gui/iconlabel"));
	btnLabel->moveTo(vec2(0,32*5));
	btnLabel->setFunction([=](){
		tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_LABEL));
	});
	mainWindow->addChild(btnLabel);
	
	GUIbutton *btnTextEntry = new GUIbutton();
	btnTextEntry->setText("");
	btnTextEntry->setSize(vec2(32,32));
	btnTextEntry->setImage(getTexture("gui/icontextentry"));
	btnTextEntry->moveTo(vec2(0,32*6));
	btnTextEntry->setFunction([=](){
		tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_TEXTENTRY));
	});
	mainWindow->addChild(btnTextEntry);
	
	GUIbutton *btnImage = new GUIbutton();
	btnImage->setText("");
	btnImage->setSize(vec2(32,32));
	btnImage->setImage(getTexture("gui/iconimage"));
	btnImage->moveTo(vec2(0,32*7));
	btnImage->setFunction([=](){
		tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_IMAGE));
	});
	mainWindow->addChild(btnImage);
	
	GUIbutton *btnScrollbar = new GUIbutton();
	btnScrollbar->setText("");
	btnScrollbar->setSize(vec2(32,32));
	btnScrollbar->setImage(getTexture("gui/iconscrollbar"));
	btnScrollbar->moveTo(vec2(0,32*8));
	btnScrollbar->setFunction([=](){
		tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_SCROLLBAR));
	});
	mainWindow->addChild(btnScrollbar);
	
	GUIbutton *btnTabs = new GUIbutton();
	btnTabs->setText("");
	btnTabs->setSize(vec2(32,32));
	btnTabs->setImage(getTexture("gui/icontabs"));
	btnTabs->moveTo(vec2(0,32*9));
	btnTabs->setFunction([=](){
		tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_TABS));
	});
	mainWindow->addChild(btnTabs);
	//------------------------
	//edit tool buttons
	GUIbutton *btnCursor = new GUIbutton();
	btnCursor->setText("");
	btnCursor->setSize(vec2(32,32));
	btnCursor->setImage(getTexture("gui/cursor"));
	btnCursor->moveTo(vec2(32,32*1));
	btnCursor->setFunction([=](){
		tool_none();
	});
	mainWindow->addChild(btnCursor);
	
	GUI->addChild(mainWindow);
	
	GUIbutton *btnSave = new GUIbutton();
	btnSave->setText("");
	btnSave->setSize(vec2(32,32));
	btnSave->setImage(getTexture("gui/iconsave"));
	btnSave->moveTo(vec2(32,32*2));
	btnSave->setFunction([=](){
		GUIfileDialog *dialog = new GUIfileDialog();
		dialog->setDirectory(getDataDirectory());
		dialog->setMode(GUIf::Save);
		dialog->setFunction([=](string filepath){
			save(filepath);
		});
		GUI->addChild(dialog);
		//save();
	});
	mainWindow->addChild(btnSave);
	
	GUIbutton *btnPaste = new GUIbutton();
	btnPaste->setText("");
	btnPaste->setSize(vec2(32,32));
	btnPaste->setImage(getTexture("gui/iconvertex"));
	btnPaste->moveTo(vec2(32,32*3));
	btnPaste->setFunction([=](){
		GUIfileDialog *dialog = new GUIfileDialog();
		dialog->setDirectory(getDataDirectory());
		dialog->setMode(GUIf::Open);
		dialog->setFunction([=](string filepath){
			paste(filepath);
		});
		GUI->addChild(dialog);	
	});
	mainWindow->addChild(btnPaste);
	
	GUIbutton *btnSG = new GUIbutton();
	btnSG->setText("");
	btnSG->setSize(vec2(32,32));
	btnSG->setImage(getTexture("gui/iconselectiongroup"));
	btnSG->moveTo(vec2(32,32*4));
	btnSG->setFunction([=](){
		tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_SELECTIONGROUP));
	});
	mainWindow->addChild(btnSG);
	
	GUIbutton *btnSel = new GUIbutton();
	btnSel->setText("");
	btnSel->setSize(vec2(32,32));
	btnSel->setImage(getTexture("gui/iconselectable2"));
	btnSel->moveTo(vec2(32,32*5));
	btnSel->setFunction([=](){
		tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_SELECTABLE));
	});
	mainWindow->addChild(btnSel);
	
	GUIbutton *btnGrid = new GUIbutton();
	btnGrid->setText("");
	btnGrid->setSize(vec2(32,32));
	btnGrid->setImage(getTexture("gui/icongrid"));
	btnGrid->moveTo(vec2(32,32*6));
	btnGrid->setFunction([=](){
		tool_start(new gui_editor_tool_place(this, gui_editor_tool_place::GEMS_GRID));
	});
	mainWindow->addChild(btnGrid);
	
	//----------------------------
	GUIwindow *workWindow = new GUIwindow();
	elWorkWindow = workWindow;
	workWindow->setTitle("GUI workspace");
	workWindow->setSize(vec2(500,500));
	workWindow->moveTo(vec2(150,100));
	
	GUI->addChild(workWindow);
	//---- events
	setLayer(layer2D);
	ge_layer_back = addNewLayer("gui_editor_layer_back",false);
	setLayer(layerGUI);
	ge_layer_front = addNewLayer("gui_editor_layer_front",false);
	ge_layer_front->resetLayer = addNewLayer("gui_editor_layer_front.reset",true,true);
	
	setLayer(ge_layer_front->resetLayer);
	setScissor(rect(vec2(0,0),vec2(1024,1024)));
	setScissoring(false);
	setAlpha(255.0f);
	setColor(vec3(0,0,0));
	
	helper.editor = this;
	
	inputChannel->addListener(this);
	inputChannel->addListenerFront(&helper);
	globalChannel->addListener(this);
	
	tool_default();
}

void gui_editor_kind::close(){
	printf("closing gui editor\n");
	if(elWorkWindow){
		printf("deleting workWindow\n");
		GUIbase *B = elWorkWindow;
		delete B;
	}
	if(elMainWindow){
		printf("deleting mainWindow\n");
		GUIbase *B = elMainWindow;
		delete B;
	}
}

void openGuiEditor(){
	auto& gui_editor = G->gs_gui_editor->g_gui_editor;

	if(gui_editor){
		printf("closing gui editor\n");
		gui_editor->close();
		printf("deleting gui editor\n");
		delete gui_editor;
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

	XMLError eResult = doc.SaveFile(filepath.c_str());
	
}

void gui_editor_kind::paste(string filepath){
	try {
		GUIwindow* workWindow;
		EPCAST(elWorkWindow, workWindow) else return;
		printf("gui_editor_kind::paste()\n");
		XMLDocument doc;
		if (!fileReadable(filepath)) { error("can't read file [%s]\n", filepath.c_str()); }
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
	}
	catch (...) { 
		//error("gui_editor::paste: some error"); 
		cout << "gui_editor: could not paste" << endl;
	}
}


void gui_editor_kind::tool_cleanup(){  //to be called after every application of a tool
	if(tool){tool->cleanup();}
}

void gui_editor_kind::tool_start(gui_editor_tool *newtool){   //select a tool
	if(lastTool){delete lastTool; lastTool = 0;}
	lastTool = tool;
	tool = newtool;
	elWorkWindow->blockChildInput = true;
}
void gui_editor_kind::tool_cancel(){   //de-select the tool
	tool_cleanup();
	tool_default();
}
void gui_editor_kind::tool_restart(){  //re-select the previous tool
	tool_cleanup();
}
void gui_editor_kind::tool_finished(){ //tool signals that it is done.
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

//=============== edit tool ============================



//=================== main gui_editor =======================
void gui_editor_kind::onEvent(eventKind event){
	if((!elWorkWindow) || (!elMainWindow)){
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
			if(!isFrontEvent){break;} //a "front event" is the one we receive before everyone else
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

//================================ random stuff ==================================================================
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

//returns snapped position relative to the start of the grid
//takes world, outputs client coords
vec2 snapToGrid(GUIbase *element, vec2 pos, float gridStep){
	vec2 corner = element->worldClientArea().start;//element->thisToWorld(element->clientArea.start);
	pos -= corner;
	pos += vec2(0.5f*gridStep,0.5f*gridStep);
	pos.x = pos.x - fmodf(pos.x, gridStep);
	pos.y = pos.y - fmodf(pos.y, gridStep);
	return pos;
}

//same but for rects
rect snapToGrid(GUIbase *element, rect R, float gridStep){
	return rect(snapToGrid(element, R.start, gridStep),
				snapToGrid(element, R.end, gridStep));
}

//returns snapped position in absolute coordinates
//takes world, outputs world coords
vec2 snapToGridToWorld(GUIbase *element, vec2 pos, float gridStep){
	vec2 corner = element->worldClientArea().start;
	return snapToGrid(element,pos,gridStep)+corner;
}

rect snapToGridToWorld(GUIbase *element, rect R, float gridStep){
	return rect(snapToGridToWorld(element, R.start, gridStep),
				snapToGridToWorld(element, R.end, gridStep));
}


/*
void magnifyingGlass(vec2 pos, vec2 offset, float sizeIn, float sizeOut){
	//
	//              sizeOut
	//             ___________
	//            |           |
	//            |           |
	//            |           |
	//            |           |
	//            B___________|
	//           /
	//   ______A/ offset
	//  |      |
	//	|  pos |
	//	|______|
	//	 sizeIn
	//
	
	//MAGNIFYING GLASS
	vec2 vSi = vec2(1,1)*sizeIn/2.f;
	vec2 vSiO = vSi + vec2(1,1);
	rect RglassIn = 	   rect(-vSi,vSi).moveBy(pos);
	rect RglassInOutline = rect(-vSiO,vSiO).moveBy(pos);
	setColor(vec3(0,0,0));
	drawRectOutline(RglassInOutline);
	
	bitmap *bmp = new bitmap();
	camera.screenRead(RglassIn, bmp);
	
	texture *t = new texture();
	t->bmp = bmp;
	t->UV = rect(vec2(0,0),vec2(1,1));
	t->name = "magnifier";
	
	vec2 vSo = vec2(1,1)*sizeOut/2.f;
	vec2 vSoO = vSo + vec2(1,1);
	//add the radius of the circumscribed circles of both outlines
	//to the length of the offset vector
	//to guarantee that they do not touch
	vec2 voffs = offset+normalizeSafe(offset)*((sizeIn+sizeOut)*sqrt(2.f)/2.f);
	voffs.x = round(voffs.x);
	voffs.y = round(voffs.y);
	rect RglassOut			= rect(-vSo,vSo).moveBy(pos+voffs);
	rect RglassOutOutline	= rect(-vSoO,vSoO).moveBy(pos+voffs);
	drawRectOutline(RglassOutOutline);
	uploadTexture(t);
	setTexturing(true);
	setColor(vec3(255,255,255));
	setTexture(t);
	drawRect(RglassOut);
	setTexturing(false);
	printf("done\n");
	//deleteTexture(t);
}

#define COL_RED		vec3(255,	0,		0	)
#define COL_ORANGE	vec3(255,	128,	0	)
#define COL_BROWN	vec3(128,	64,		0	)
#define COL_YELLOW	vec3(255,	255,	0	)
#define COL_DYELLOW	vec3(128,	128,	0	)
#define COL_LIME	vec3(128,	255,	0	)
#define COL_GREEN	vec3(0,		255,	0	)
#define COL_DGREEN	vec3(0,		128,	0	)
#define COL_MINT	vec3(0,		255,	128 )
#define COL_CYAN	vec3(0,		255,	255 )
#define COL_SKY		vec3(0,		128,	255 )
#define	COL_BLUE	vec3(0,		0,		255 )
#define COL_DBLUE	vec3(0,		0,		128	)
#define COL_PURPLE	vec3(128,	0,		255 )
#define COL_PINK	vec3(255,	0,		255 )
#define COL_DRED	vec3(128,	0,		0	)
#define COL_WHITE	vec3(255,	255,	255 )
#define COL_GRAY	vec3(128,	128,	128 )
#define COL_BLACK	vec3(0,		0,		0	)

void rainbowIndicator(vec2 pos, vector<bool> state){
	vector<vec3> colors = {
		COL_RED,
		COL_YELLOW,
		COL_GREEN,
		COL_BLUE,
		COL_PINK,
		COL_DRED,
		COL_ORANGE,
		COL_LIME,
		COL_CYAN,
		COL_PURPLE,
		COL_BROWN,
		COL_DGREEN,
		COL_SKY,
		COL_GRAY,
		COL_WHITE
	};
	int size = colors.size();
	if(state.size()-1 < size){size = state.size()-1;}
	
	//black rect
	rect R = rect(pos,pos+vec2(size+2,3));
	setColor(vec3(0,0,0));
	
	//red outline
	if(state[0]){
		drawRect(R);
		setColor(vec3(128,0,0));
		drawRectOutline(R);
		setPointSize(1.f);
	}
	
	//indicators
	for(int I = 0; I < size; I++){
		if(state[I+1]){setColor(colors[I]); drawPoint(toVec3(pos + vec2(1+I,1)));}
	}
}

void drawClock(vec2 pos){
	vec2 v;
	switch((getGameTicks()/30)%8){
		case 0: v = vec2(0,-8); break;
		case 1: v = vec2(8,-8); break;
		case 2: v = vec2(8,0); break;
		case 3: v = vec2(8,8); break;
		case 4: v = vec2(0,8); break;
		case 5: v = vec2(-8,8); break;
		case 6: v = vec2(-8,0); break;
		case 7: v = vec2(-8,-8); break;
	}
	setColor(vec3(64,64,64));
	drawLine(toVec3(pos), toVec3(pos+v));
}
*/

