#include "game.h"


//
// NEXT:
//  GUIToolkit overhaul:
//  -saving/loading of edited widgets:
//   +-XML parser
//   +-unique names for each widget
//  -resizible widgets  
//   +-layout elements
//  -widget skinning
//	 +-separate renderer
//  -tree view widget
//  -GUI scripts
//
//  maybe a shell scripting language?
//
//  get an FPS counter
//  see if GUI is fast enough
//  fix it if not
//
//  maybe implement correct input
//  with a chain-of-responsibility
//  (someone noms a message)
//
//  if/when sufficiently fast,
//  make a blocky room
//  do block-sphere collision
//	derp physics`
//	derp char controller
//
//  optimize physics using octrees
//	generateBox should return model or emodel
//  model MAY become a handle that associates textures with meshes
//  mesh then would be a condensed representation (rmodel or emodel with no non-essential data)
//
//  select sides of a box and/or texture them independently.
//  have "world" as an entity that can be referenced (contains all static mesh instances)
//  collide point (camera) with world.
//
//  yes/no collision detection vs distance collision detection
//  refine-able collision data? as continuation?
//
//  todo: visualize intersection using SAT-carving -> cut box with a plane -> editor functions -> editor GUI
//  todo: implement obj-file parsing for non-uv mapped objects and also multi-shape objects.
//  todo: nav_node path optimization & checking (remove nodes once LOS navigation is possible)
//
//  should mouseCapture be a bool, or a stack?
//  also, make multiple "views" with renderlayers active in specific sets of views.
//
//  idea: renderlayer debugger -> browse commands and display their result + current render state + last modified for every var
//  idea: smart pointers with "passing responsibility" semantics, i.e. "it's yours to delete".
//  idea: collider sleeping and/or some way to cull the collisions between objects that haven't moved relative to one another.
//  idea: use cppast (C++ parser, AST generator) to generate RTTI info.
//
// Glitches:
// AI characters experience friction vs. ground
//		possibly fixed: separation code didn't conserve parralel velocity
//						and just completely undid a move
//		still, movement is by setting velocity rather than applying force
//		(moment is not conserved)
//
// AI characters drawn at pos+vel rather than at pos.
// player character sometimes bounces off, and sometimes sticks to walls
// AI characters get broken velocity and disappear
//
// GUI bugs:
// - scrollbar needs to adjust scissor
// - crash on printscreen (F7)
// - crash on closing gui_editor
//
//
// Nitpicks:
// resetlayers are also drawn on their own.
// rmodels and textures get double-uploaded.
// camera has to recalculate it's own projection every time screenToWorld/worldToScreen is called
//	would be better if there were separate camera3D and camera2D with their own projections.
// using camera.worldToScreen() can result in seeing behind you
//

//#warning TODO: make it so collision normals are away from body2
//#warning TODO: make it so friction resolution uses actual normal force and not normal velocity

int main(int argc, char **argv){
	printf("Hello World!\n");
	G = new Globals();

	auto& camera = G->gs_camera->g_camera;
	auto& console = G->gs_console->g_console;


	initUtil();
	window_init(512,512);
	initInput();
	initFreeType();
	renderInit();
	loadAssets();

	camera.setPos({-0.5,0,0});
	
	setupLayers();
	addKeybinds();

	console = new consoleKind();
	addConsoleCommands();

	openGUI();

	printf("-------- frames begin ----------\n");

	while(1){
		tick();
	}
	return 0;
}

void tick(){
	auto& globalChannel = G->gs_event->g_globalChannel;
	//auto& entities = G->gs_entity->g_entities;
	auto& eventTime = G->gs_main->g_eventTime;
	auto& renderTime = G->gs_main->g_renderTime;
	auto& sysmsgTime = G->gs_main->g_sysmsgTime;
	auto& frametime = G->gs_main->g_frametime;
	auto& eventTimeFlt = G->gs_main->g_eventTimeFlt;
	auto& renderTimeFlt = G->gs_main->g_renderTimeFlt;
	auto& sysmsgTimeFlt = G->gs_main->g_sysmsgTimeFlt;
	auto& frametimeFlt = G->gs_main->g_frametimeFlt;
	auto& mem1 = G->gs_main->g_mem1;
	auto& mem2 = G->gs_main->g_mem2;

	eventKind e1; e1.type = EVENT_TICK;
	globalChannel->publishEvent(e1);
		
	eventKind e2; e2.type = EVENT_FRAME;
	profile(globalChannel->publishEvent(e2),eventTime);
	
	eventKind e3; e3.type = EVENT_CLEANUP;
	globalChannel->publishEvent(e3);
	
	renderTick();  
	sysMessageTick(); 
}

void openGUI(){
	auto& layerGUI = G->gs_main->g_layerGUI;
	auto& layer2D = G->gs_paint->g_layer2D;
	auto& GUI = G->gs_main->g_GUI;
	auto& inputChannel = G->gs_input->g_inputChannel;
	auto& globalChannel = G->gs_event->g_globalChannel;

	layerGUI = new renderLayer("GUI.layerGUI");
	addLayer(layer2D,layerGUI);
	GUI = new GUIbase();
	inputChannel->addListenerFront(GUI);
	globalChannel->addListenerFront(GUI);
}
