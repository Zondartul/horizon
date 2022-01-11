#include "game.h"
#include "string.h"
#include "gui_editor.h"
#include "file.h"
#include "map_editor.h"
#include "node_graph.h"

extern bool gamePaused;

int cmd_pauseGame(int argc, char **argv){
	gamePaused = !gamePaused;
    return 0;
}

int cmd_spawnCharacter(int argc, char **argv){
	inputController->character = new characterController();
    return 0;
}

int cmd_spawnNPC(int argc, char **argv){
	new AIcontroller(new characterController());
    return 0;
}

void deleteDynamicBodies(){
	for(auto I = entities.begin(); I != entities.end();){
		if((*I)->body && (*I)->body->type == BODY_DYNAMIC){
			delete *I;
			I = entities.erase(I);
		}else{
			I++;
		}
	}
}

void spawnPhysbox1(){
	physbox("materials/brick2",1.f,1.f,0.2f,0.5f);//0.007f);
}

void spawnFancyBox(){
	vector<physprim> combo = {
		{PRIM_BOX, "materials/wood5",
			vec3(0.5,0.5,0.5),
			{1.1f,1.1f,1.1f,1.f}},
		{PRIM_BOX, "materials/steel",
			vec3(0,0,0),
			{0.3f,0.3f,0.3f,1.f}},
		{PRIM_BOX, "materials/steel",
			vec3(1,0,0),
			{0.3f,0.3f,0.3f,1.f}},
		{PRIM_BOX, "materials/steel",
			vec3(0,1,0),
			{0.3f,0.3f,0.3f,1.f}},
		{PRIM_BOX, "materials/steel",
			vec3(1,1,0),
			{0.3f,0.3f,0.3f,1.f}},
		{PRIM_BOX, "materials/steel",
			vec3(0,0,1),
			{0.3f,0.3f,0.3f,1.f}},
		{PRIM_BOX, "materials/steel",
			vec3(1,0,1),
			{0.3f,0.3f,0.3f,1.f}},
		{PRIM_BOX, "materials/steel",
			vec3(0,1,1),
			{0.3f,0.3f,0.3f,1.f}},
		{PRIM_BOX, "materials/steel",
			vec3(1,1,1),
			{0.3f,0.3f,0.3f,1.f}}
	};
	physcombo(combo);
}

void spawnPhysbox2(){
	physbox("materials/steel",0.5f,0.125f,1.f,0.f);
}

void spawnTree(){
	vector<physprim> treecombo = {
		{PRIM_CYL,	"materials/log_wall",
			vec3(0,0,0),
			{0.075f,0.375,16,2.0f}},
		{PRIM_CONE,	"materials/leaves_fir",
			vec3(0,0,0.375),
			{0.25f,0.75,16,-4.0f}},
		{PRIM_CONE,	"materials/leaves_fir",
			vec3(0,0,0.75),
			{0.2f,0.75,16,-4.0f}},
	};

	physcombo(treecombo);
}

int cmd_listKeybinds(int argc, char **argv){
	for(auto I = keybinds->binds.begin(); I != keybinds->binds.end(); I++){
		string key = I->first;
		bind b = I->second;
		console->print(fstring("%s: %s\n",key.c_str(),b.cmd.c_str()));
	}
	return 0;
}

void test1render(){
	setLayer(layer3D);
	resetLayer(layer3D);
	setLighting(false);
	setPosition(vec3(0,0,0));
	setPointSize(3);

	//draw 3 points in xy plane
	setColor(vec3(0,0,0));	//black for origin
	drawPoint(vec3(0,0,-10));

	setColor(vec3(255,0,0)); //red for x
	drawPoint(vec3(1,0,-10));

	setColor(vec3(0,255,0)); //green for y
	drawPoint(vec3(0,1,-10));

	setColor(vec3(0,0,255)); //blue for z
	drawPoint(vec3(0,0,-1-10));

	//make a 3x3x3 point-grid next to it
	setColor(vec3(255,255,255));
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			for(int k = 0; k < 4; k++){
				drawPoint(vec3(i,j,k-5));
			}
		}
	}
	//draw a 3x3x2 box in it
	vec3 start = vec3(0,0,-5);
	vec3 rot = vec3(0,0,0);

	vec3 scale = vec3(3,3,2);
	vec3 pos = start+scale/2.f;

	setColor(vec3(128,128,128));
	drawBoxWireframe(pos,rot,scale);

	//make a 2x2x2 textured cube
	scale = vec3(2,2,2);
	pos = start+scale/2.f;
	static rmodel *rm = 0;
	setLayer(loadLayer);
	if(!rm){
		e_model *em = generateBox(scale,1.f);
		rm = em->getRmodel();
		uploadRmodel(rm);
	}
	static texture *t = 0;
	if(!t){
		t = getTexture("materials/brick3");
		uploadTexture(t);
	}
	setLayer(layer3D);
	resetLayer(layer3D);
	setTexture(t);
	setRenderMode(3);
	setColoring(false);
	setTexturing(true);
	setPosition(pos);
	setScale(vec3(1,1,1));
	drawRmodel(rm);
}


int cmd_help(int argc, char **argv){
	if(argc==0){
		cprint("commands: ");
		for(auto I = console->commands.begin(); I != console->commands.end(); I++){
			cprint("%s ",I->name.c_str());
		}
		return 0;
	}
	if(argc==1){
		auto cmd = console->getCommand(argv[0]);
		if(cmd){
			if(cmd->help != ""){
				cprint("%s",cmd->help.c_str());
			}else{
				cprint("no help is available for this command\n");
			}
		}else{
			cprint("command [%s] is unknown\n",argv[0]);
		}
		return 0;
	}
	cprint("usage: help <command>");
	return 1;
}

int cmd_whereami(int argc, char **argv){
	auto C = inputController->character;
	entity *E = 0;
	if(!C || !C->E){
		vec3 pos = camera.pos;
		cprint("%s\n",toCString(pos));
	}else{
		E = C->E;
		vec3 pos = E->body->pos;
		cprint("%s\n",toCString(pos));
	}
	return 0;
}

int cmd_teleport(int argc, char **argv){
	string word1,word2,word3;
	if(argc>0){word1 = argv[0];}
	if(argc>1){word2 = argv[1];}
	if(argc>2){word3 = argv[2];}
	vec3 pos = vec3(atoi(word1.c_str()),atoi(word2.c_str()),atoi(word3.c_str()));
	cprint("teleporting to %s\n",toCString(pos));
	auto C = inputController->character;
	if(!C || !C->E){
		camera.setPos(pos);
	}else{
		C->E->body->pos = pos;
	}
	return 0;
}

int cmd_reset(int argc, char **argv){deleteDynamicBodies(); return 0;}
int cmd_physbox(int argc,char**argv){
	int num = 0;
	if(argc>0){
		num = atoi(argv[0]);
	}
	switch(num){
		default:
		case 0:
			spawnPhysbox1();
			break;
		case 1:
			spawnFancyBox();
			break;
		case 2:
			spawnPhysbox2();
			break;
		case 3:
			spawnTree();
			break;
	}
	return 0;
}


//int cmd_character(int argc, char **argv){
//	spawnCharacter(argc,argv);
//	return 0;
//}

//int cmd_npc(int argc, char **argv){
//	spawnNPC(argc,argv);
//	return 0;
//}

//int cmd_pause(int argc, char **argv){
//	pauseGame(argc,argv);
//}

int cmd_camera(int argc, char **argv){
	if(argc!=1){printf("invalid syntax\n"); return 1;}
	if(!strcmp(argv[0],"ortho")){
		camera.perspective = false;
		camera.scaleOrtho = 1.f/10.f;
	}else if(!strcmp(argv[0],"perspective")){
		camera.perspective = true;
		camera.scaleOrtho = 1.f;
	}
	return 0;
}

int cmd_scene(int argc, char **argv){
	if(argc!=1){printf("invalid syntax\n"); return 1;}
	int num = atoi(argv[0]);
	switch(num){
		default:
		case 1:
			makeScene1();
			break;
		case 2:
			makeScene2();
			break;
        case 3:
            makeScene3();
            break;
		case -1:
			for(auto I = entities.begin(); I != entities.end();){
				if((*I)->group == "scene1"){delete *I; I = entities.erase(I);}else{I++;}
			}
			break;
		case -2:
			for(auto I = entities.begin(); I != entities.end();){
				if((*I)->group == "scene2"){delete *I; I = entities.erase(I);}else{I++;}
			}
			break;
        case -3:
			for(auto I = entities.begin(); I != entities.end();){
				if((*I)->group == "scene3"){delete *I; I = entities.erase(I);}else{I++;}
			}
			break;

	}
	return 0;
}

int cmd_opengui(int argc, char **argv){
	if(argc!=1){printf("invalid syntax\n"); return 1;}
	int num = atoi(argv[0]);
	switch(num){
		default:
		case 1:openTestWindow1();break;
		case 2:openTestWindow2();break;
		case 3:openTestWindow3();break;
		case 4:openTestWindow4();break;
		case 5:openTestWindow5();break;
		case 6:openTestWindow6();break;
	}
	return 0;
}

int cmd_editor(int argc, char **argv){openEditor2(); return 0;}
int cmd_guieditor(int argc, char **argv){openGuiEditor(); return 0;}
int cmd_dirs(int argc, char **argv){
	cprint("directories:\n");
	cprint((string()+"program dir: "+getProgramDirectory()+"\n").c_str());
	cprint((string()+"config dir: "+getConfigDirectory()+"\n").c_str());
    return 0;
}
int cmd_mapeditor(int argc, char **argv){openMapEditor(); return 0;}
int cmd_framereport(int argc, char **argv){frameReportNextRender(); return 0;}
int cmd_nodegraph(int argc, char **argv){generateNodegraph(); return 0;}
int cmd_printf(int argc, char **argv){printf_enabled = !printf_enabled;}

void addKeybinds(){
	keybinds->binds["+R"].cmd = "reset";
	keybinds->binds["+F"].cmd = "physbox";
	keybinds->binds["+G"].cmd = "physbox 1";
	keybinds->binds["+H"].cmd = "physbox 2";
	keybinds->binds["+J"].cmd = "physbox 3";
	keybinds->binds["+C"].cmd = "character";
	keybinds->binds["+V"].cmd = "npc";
	keybinds->binds["+Pause"].cmd = "pause";
	keybinds->binds["+F8"].cmd = "framereport";
	keybinds->binds["+F3"].cmd = "printf";
}



void addConsoleCommands(){
	console->addCommand({"help","prints information about installed commands\n",cmd_help});
	console->addCommand({"whereami","tells your current coords\n",cmd_whereami});
	console->addCommand({"teleport","args: <x> <y> <z>\nteleports you to given coords\n",cmd_teleport});
	console->addCommand({"reset","reset scene (delete all dynamic bodies)\n",cmd_reset});
	console->addCommand({"physbox",
		"spawn a physbox\n"
		"args: <n>\n"
		"n: 1 - plain physbox\n"
		"n: 2 - fancy physbox\n"
		"n: 3 - bouncy physbox\n"
		"n: 4 - tree\n",
		cmd_physbox});
	console->addCommand({"character","spawn a player-controlled character\n",cmd_spawnCharacter});
	console->addCommand({"npc","spawn an NPC\n",cmd_spawnNPC});
	console->addCommand({"pause","pause the game\n",cmd_pauseGame});
	console->addCommand({"camera","args:ortho|perspective\nsets camera mode\n",cmd_camera});
	console->addCommand({"keybinds","print the current keybinds\n",cmd_listKeybinds});
	console->addCommand({"makescene",
		"make a scene:\n"
		"1 - small walls\n"
		"2 - city\n"
        "3 - maze;\n"
		"-number to remove a scene\n",
		cmd_scene});
	console->addCommand({"opengui",
		"open a GUI window. args: 1-5\n",
		cmd_opengui});
	console->addCommand({"editor",
		"open a 3D model editor\n",
		cmd_editor});
	console->addCommand({"guieditor","open a graphical editor of the GUI\n", cmd_guieditor});
	console->addCommand({"dirs","show path settings\n",cmd_dirs});
	console->addCommand({"mapeditor","open a map editor\n",cmd_mapeditor});
	console->addCommand({"framereport","save the render commands for the next frame to disk",cmd_framereport});
    console->addCommand({"nodegraph","generate a navigation node graph",cmd_nodegraph});
	//console->addCommand({"deleteDynamicBodies","",cmd_deleteDynamicBodies});
	//console->addCommand({"spawnPhysbox1","",cmd_spawnPhysbox1});
	//console->addCommand({"spawnFancyPhysbox","",cmd_spawnFancyPhysbox});
	//console->addCommand({"spawnPhysbox2","",cmd_spawnPhysbox2});
	//console->addCommand({"spawnTree","",cmd_spawnTree});
	//console->addCommand({"spawnCharacter","",cmd_spawnCharacter});
	//console->addCommand({"spawnNPC","",cmd_spawnNPC});
	//console->addCommand({"pause","",cmd_pause});
	console->addCommand({"printf","toggle debug message printing\n",cmd_printf});

}








