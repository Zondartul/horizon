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



model *g_m;
GUIbase *g_GUI = 0;
octree_node *g_octree_root;
float g_frametime = 0;
float g_fps = 0;
bool g_gamePaused = false;
bool g_printf_enabled = true;
void *g_mem1 = 0;
void *g_mem2 = 0;

uint32_t g_audio_len = 0;
uint8_t *g_audio_pos = 0;
//Through this function, SDL requests the user to copy "len" bytes of audio data into "stream".
void my_audio_callback(void *userdata, uint8_t *stream, int len){
    if(g_audio_len == 0){return;}

    len = min(g_audio_len, len);
#ifndef NO_SDL
    SDL_memcpy(stream, g_audio_pos, len);
#endif
    g_audio_pos += len;
    g_audio_len -= len;
}

int main(int argc, char **argv){
	printf("Hello World!\n");
	debuginit();

	initUtil();
	window_init(512,512);
	initInput();
	initFreeType();
	renderInit();
	loadAssets();

	g_octree_root = new octree_node();
	float worldsize = 500;
	g_octree_root->volume = AABB(-vec3(0.5,0.5,0.5)*worldsize,vec3(0.5,0.5,0.5)*worldsize);
	g_camera.setPos({-0.5,0,0});
	
	setbuf(stdout,0);

	setupLayers();
	addKeybinds();

	g_console = new consoleKind();
	addConsoleCommands();

	openGUI();
	setFramePrinter(new frameprinter()); //it's annoying

	g_ecs.physics = new ecs_physics_system_kind();
	g_ecs.collision = new ecs_collision_system_kind();
	g_ecs.render = new ecs_render_system_kind();
	g_physicsOn = true;
	g_collisionOn = true;
	g_texturingOn = true;

	printf("initializing sound\n");

#ifndef NO_SDL
	uint32_t wav_length;
	uint8_t *wav_buffer;
	SDL_AudioSpec wav_spec;
	const char *wavpath = "footstep_rock_1.wav";	
	if(!SDL_LoadWAV(wavpath, &wav_spec, &wav_buffer, &wav_length)){
        	printf("SDL wav LOAD ERROR\n"); return 0;
	}
	printf("loaded ok\n");
	printf("wav_spec:\nchnnales: %d\nfreq: %d\npadding: %d\nsamples: %d\nsilence: %d\nsize: %d\n",wav_spec.channels, wav_spec.freq, wav_spec.padding, wav_spec.samples, wav_spec.silence, wav_spec.size);
	wav_spec.callback = my_audio_callback;
	wav_spec.userdata = 0;

	g_audio_pos = wav_buffer;
	g_audio_len = wav_length;


	if(SDL_OpenAudio(&wav_spec, 0) < 0){
        printf("DEVICE ERROR\n"); return 0;
	}
	SDL_PauseAudio(0);
	printf("play\n");

	SDL_CloseAudio();
	SDL_FreeWAV(wav_buffer);
#endif
	makeScene1();
	printf("-------- frames begin ----------\n");

	printf("\n");
	g_mem1 = malloc(1);
	while(1){

		profile(tick(),g_frametime);
		if(g_frametime){g_fps = 1.0f/g_frametime;}
	}
	return 0;
}

float g_eventTime;
float g_renderTime;
float g_sysmsgTime;

float g_eventTimeFlt = 0;
float g_renderTimeFlt = 0;
float g_sysmsgTimeFlt = 0;
float g_frametimeFlt = 1.f;
void tick(){
		eventKind e1; e1.type = EVENT_TICK;
		g_globalChannel->publishEvent(e1);
		
		eventKind e2; e2.type = EVENT_FRAME;
		profile(g_globalChannel->publishEvent(e2),g_eventTime);
	
		eventKind e3; e3.type = EVENT_CLEANUP;
		g_globalChannel->publishEvent(e3);
	
		frameprint(string()+"entities: "+(int)g_entities.size());
	profile(renderTick(),g_renderTime);
	profile(sysMessageTick(),g_sysmsgTime);
    static int i = 0;
    i++;
    if(i == 20){i = 0; g_eventTimeFlt = g_eventTime; g_renderTimeFlt = g_renderTime; g_sysmsgTimeFlt = g_sysmsgTime; g_frametimeFlt = g_frametime;}

	frameprint(fstring("time.event: %.1f%%",g_eventTimeFlt*100.f/g_frametimeFlt));
	frameprint(fstring("time.render: %.1f%%",g_renderTimeFlt*100.f/g_frametimeFlt));
	frameprint(fstring("time.sysmsg: %.1f%%",g_sysmsgTimeFlt*100.f/g_frametimeFlt));
	g_mem2 = malloc(1);
	uint64_t memdiff = (uint64_t)g_mem2 - (uint64_t)g_mem1;
	frameprint(fstring("total RAM use: %1.6f MB\n",((float)memdiff)/1000000.f));
	free(g_mem2);
	
}

renderLayer *g_layerGUI;
void openGUI(){
	g_layerGUI = new renderLayer("GUI.layerGUI");
	addLayer(g_layer2D,g_layerGUI);
	g_GUI = new GUIbase();
	g_inputChannel->addListenerFront(g_GUI);
	g_globalChannel->addListenerFront(g_GUI);
}
