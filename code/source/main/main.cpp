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



model *m;
GUIbase *GUI = 0;
octree_node *octree_root;
float frametime = 0;
float fps = 0;
bool gamePaused = false;
bool printf_enabled = true;
void *mem1 = 0;
void *mem2 = 0;

uint32_t audio_len = 0;
uint8_t *audio_pos = 0;
//Through this function, SDL requests the user to copy "len" bytes of audio data into "stream".
void my_audio_callback(void *userdata, uint8_t *stream, int len){
    if(audio_len == 0){return;}

    len = min(audio_len, len);
#ifndef NO_SDL
    SDL_memcpy(stream, audio_pos, len);
#endif
    audio_pos += len;
    audio_len -= len;
}

int main(int argc, char **argv){
	printf("Hello World!\n");
	{
		FILE* f = fopen("dirmarker.beep", "w");
		if (f) {
			printf("maked dirmaker.beep\n");
			fclose(f);
		}
		else {
			printf("can't place a dirmaker\n");
		}
	}

	debuginit();

	initUtil();
	window_init(512,512);
	initInput();
	initFreeType();
	renderInit();
	loadAssets();



	//openEditor2();


	octree_root = new octree_node();
	float worldsize = 500;
	octree_root->volume = AABB(-vec3(0.5,0.5,0.5)*worldsize,vec3(0.5,0.5,0.5)*worldsize);
	//renderTest1();
	camera.setPos({-0.5,0,0});
	
	//renderTest();
	setbuf(stdout,0);

	setupLayers();
	addKeybinds();


	//initConsole();
	console = new consoleKind();
	addConsoleCommands();
	//cprint("Hello World!\n");
	//cprint("I am uberpone\n");
	//cprint("test test test\n");
	//cprint("beep boop\n");
	//string str = fstring("Hello, %d pi = %f mcguffins, %s\n", 2, 2*3.14f, "brah");
	//cprint(str.c_str());

	openGUI();
	setFramePrinter(new frameprinter()); //it's annoying

	//initPicker();

	//openTestWindow1(); //have these registered through console?
	//openTestWindow2();
	//openTestWindow3();
	//openTestWindow4();

	//rm2 = generateBox(vec3(1,1,10));
	//rm2t = getTexture("materials/brick2");
	//uploadTexture(rm2t);
	//uploadRmodel(rm2);
	//obelisk();

	//makeScene1();
	//makeScene1(vec3(-50,-50,-50));
	//makeScene2();
	//makeSheet(vec3(0,0,-0.5));




	ecs.physics = new ecs_physics_system_kind();
	ecs.collision = new ecs_collision_system_kind();
	ecs.render = new ecs_render_system_kind();
	physicsOn = true;
	collisionOn = true;
	texturingOn = true;

	printf("initializing sound\n");
	//if(Mix_OpenAudio( 22050, /*MIX_DEFAULT_FORMAT*/0, 2, 4096) == -1){
    //    printf("SDL MIXER ERROR\n");
    //    return 0;
	//}
	//Mix_Music *music = 0;
	//Mix_Chunk *step = 0;

	//step = Mix_LoadWAV("D:\Stride\horizon\horizon\resource\sound\footstep_rock_1.wav");
	//if(step){printf(".wav loaded!\n");}else{printf("ERROR: couldn't load wav\n"); return;}
	//also call Mix_FreeChunk(step) on exit
	//and then Mix_CloseAudio()
	//if(Mix_PlayChannel(-1, step, 0);){printf("played ok\n");}else{printf("not played\n");}

#ifndef NO_SDL
	uint32_t wav_length;
	uint8_t *wav_buffer;
	SDL_AudioSpec wav_spec;
	const char *wavpath = //"D:\\Stride\\horizon\\horizon\\resource\\sound\\footstep_rock_1.wav";
		"footstep_rock_1.wav";	
	if(!SDL_LoadWAV(wavpath, &wav_spec, &wav_buffer, &wav_length)){
        	printf("SDL wav LOAD ERROR\n"); return 0;
	}
	printf("loaded ok\n");
	printf("wav_spec:\nchnnales: %d\nfreq: %d\npadding: %d\nsamples: %d\nsilence: %d\nsize: %d\n",wav_spec.channels, wav_spec.freq, wav_spec.padding, wav_spec.samples, wav_spec.silence, wav_spec.size);
	wav_spec.callback = my_audio_callback;
	wav_spec.userdata = 0;

	audio_pos = wav_buffer;
	audio_len = wav_length;


	if(SDL_OpenAudio(&wav_spec, 0) < 0){
        printf("DEVICE ERROR\n"); return 0;
	}
	SDL_PauseAudio(0);
	printf("play\n");
	/*
	for(int I = 0; I < 10; I++){
        while(audio_len > 0){
            SDL_Delay(100);
        }
        SDL_PauseAudio(1);
        audio_pos = wav_buffer;
        audio_len = wav_length;
        SDL_PauseAudio(0);
	}
	*/
	SDL_CloseAudio();
	SDL_FreeWAV(wav_buffer);
#endif
	makeScene1();
	printf("-------- frames begin ----------\n");

	printf("\n");
	mem1 = malloc(1);
	while(1){

		profile(tick(),frametime);
		if(frametime){fps = 1.0f/frametime;}
	}
	return 0;
}

//extern int num_rmodels;
//extern int num_textures;
//extern long bytes_rmodels;
//extern long bytes_textures;

float eventTime;
float renderTime;
float sysmsgTime;

float eventTimeFlt = 0;
float renderTimeFlt = 0;
float sysmsgTimeFlt = 0;
float frametimeFlt = 1.f;
void tick(){
	//printf("\n-------------FRAME-----------\n\n");
	//if(!gamePaused){
		eventKind e1; e1.type = EVENT_TICK;
		globalChannel->publishEvent(e1);
		
		eventKind e2; e2.type = EVENT_FRAME;
		profile(globalChannel->publishEvent(e2),eventTime);
	
		eventKind e3; e3.type = EVENT_CLEANUP;
		globalChannel->publishEvent(e3);
	
		frameprint(string()+"entities: "+(int)entities.size());
		//frameprint(string()+"rmodels: "+num_rmodels+", "+((double)bytes_rmodels)/1024.f/1024.f+" MB");
		//frameprint(string()+"textures: "+num_textures+", "+((double)bytes_textures)/1024.f/1024.f+" MB");
	//}
	profile(renderTick(),renderTime);
	//renderTest1Tick();
	//inputController.think();
	profile(sysMessageTick(),sysmsgTime);
	//SDL_Delay(1);
    static int i = 0;
    i++;
    if(i == 20){i = 0; eventTimeFlt = eventTime; renderTimeFlt = renderTime; sysmsgTimeFlt = sysmsgTime; frametimeFlt = frametime;}

	frameprint(fstring("time.event: %.1f%%",eventTimeFlt*100.f/frametimeFlt));
	frameprint(fstring("time.render: %.1f%%",renderTimeFlt*100.f/frametimeFlt));
	frameprint(fstring("time.sysmsg: %.1f%%",sysmsgTimeFlt*100.f/frametimeFlt));
	mem2 = malloc(1);
	uint64_t memdiff = (uint64_t)mem2 - (uint64_t)mem1;
	frameprint(fstring("total RAM use: %1.6f MB\n",((float)memdiff)/1000000.f));
	free(mem2);
	
}

renderLayer *layerGUI;
void openGUI(){
	layerGUI = new renderLayer("GUI.layerGUI");
	addLayer(layer2D,layerGUI);
	GUI = new GUIbase();
	inputChannel->addListenerFront(GUI);
	globalChannel->addListenerFront(GUI);
	//openTestWindow3();
	//openTestWindow4();
}



/* #include "globals.h"
#include "stdio.h"
#include "window.h"
#include "renderLow.h"
#include <SDL2/SDL.h>
#include "modelLoader.h"
#include "fontLoader.h"
#include "inputController.h"
#include "camera.h"
#include "fonts.h"
#include "printw.h"
#include "model.h"
#include "resource.h"
#include "gl/gl.h"
#include "paint.h"
#include "GUI.h"
#include "stringUtils.h"
#include "simplemath.h"
#include "editor2.h"
#include "timer.h"
#include "frameprinter.h"
#include "modelprimitives.h"
#include "entity.h"
#include "ecs.h"
#include "keybinds.h"
#include "octree.h"
#include "mouse.h"
#include "hook.h"
#include "console.h"
#include "picker.h"
#include "physbox.h"
#include "editmodel.h"
#include "input.h"
#include "paramList.h"
#include "operators.h" */
