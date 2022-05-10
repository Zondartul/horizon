#include "picker.h"
#include "GUI.h"
#include "stringUtils.h"
#include "renderLayer.h"
#include "paint.h"
#include "keybinds.h"
#include "mouse.h"
#include "camera.h"
#include "rmodel.h"
#include "window.h"
#include "console.h"
#include "main.h" 
#include "global_vars.h"
void closeEntityOptionsWindow(){
	auto& entWindow = Gt->gs_picker->g_entWindow;
	if(entWindow){delete entWindow; entWindow = 0;}
}
int cmd_pick(int argc, char **argv){
	auto& pickerLayer = Gt->gs_picker->g_pickerLayer;
	auto& camera = Gb->gs_camera->g_camera;
	setLayer(pickerLayer);
	setColor(vec3(0,255,0));
	setPointSize(3);
	vec2 mousepos = getMousePos();
	vec3 mousedv = camera.screenToWorld(vec3(mousepos.x,mousepos.y,1));
	vec3 dv = normalizeSafe(mousedv - camera.pos);
	printf("LMB: %s, dv: %s\n",toString(mousepos).c_str(),toString(dv).c_str());
		printf("no hit\n");
		closeEntityOptionsWindow();
	return 0;
}
int cmd_pick2(int argc, char **argv){
	auto& loadLayer = Gb->gs_paint->g_loadLayer;
	auto& pickerLayer = Gt->gs_picker->g_pickerLayer;
	auto& camera = Gb->gs_camera->g_camera;
	setLayer(pickerLayer);
	setPointSize(1);
	rmodel *rm1 = new rmodel();
	vec2 scr = getScreenSize();
	int div = 50;
	for(int I = 0; div*I < scr.x; I++){
		for(int J = 0; div*J < scr.y; J++){
			vec3 dv = camera.screenToWorld(vec3(div*I,div*J,1));
			dv = normalizeSafe(dv - camera.pos);
		}
	}
	rm1->finalize();
	setLayer(loadLayer);
	uploadRmodel(rm1);
	setLayer(pickerLayer);
	setRenderMode(1);
	setColor(vec3(255,0,0));
	drawRmodelStd(rm1);
	return 0;
}
void initPicker(){
	auto& pickerLayer = Gt->gs_picker->g_pickerLayer;
	auto& layer3D = Gb->gs_paint->g_layer3D;
	auto& console = Gt->gs_console->g_console;
	auto& keybinds = Gb->gs_keybinds->g_keybinds;
	pickerLayer = new renderLayer("picker");
	addLayer(layer3D,pickerLayer);
	setLayer(pickerLayer);
	setPosition(vec3(0,0,0));
	setScale(vec3(1,1,1));
	setColoring(false);
	setTexturing(false);
	setLighting(false);
	setPointSize(3);
	console->addCommand({"pick","picker function 1\n",cmd_pick});
	console->addCommand({"pick2","picker function 2\n",cmd_pick2});
	keybinds->binds["+LMB"].cmd = "pick";
	keybinds->binds["+RMB"].cmd = "pick2";
}
