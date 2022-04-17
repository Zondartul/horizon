#include "game.h"
#include "string.h"
#include "gui_editor.h"
#include "file.h"
#include "map_editor.h"
#include "node_graph.h"
#include "gui_texturebrowser.h"
#include <fstream>
using std::ofstream;
#include <sstream>
using std::stringstream;
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
	float size = 4.f;
	vector<physprim> treecombo = {
		{PRIM_CYL,	"materials/log_wall",
			size*vec3(0,0,0),
			{size*0.075f,size*0.375f,16,2.0f}},
		{PRIM_CONE,	"materials/leaves_fir",
			size*vec3(0,0,0.375),
			{size*0.25f,size*0.75f,16,-4.0f}},
		{PRIM_CONE,	"materials/leaves_fir",
			size*vec3(0,0,0.75),
			{size*0.2f,size*0.75f,16,-4.0f}},
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
int cmd_printf(int argc, char **argv){printf_enabled = !printf_enabled; return 0;}

map<const char*, struct_alloc_file> prev_allocation_map;
int memreport_last_frame = 0;
int prev_total_size = 0;
bool has_prev_alloc_map = 0;

int cmd_memreport(int argc, char **argv){
	
	
	if(argc && (strcmp(argv[0], "-i")==0)){
		//INCREMENTAL REPORT. only reports used memory ADDED since last report.
		if(has_prev_alloc_map){
			string filename = "logs/memreport_inc";
			filename = filename + "_" + getCalendarDateStr()+"_"+getCalendarTimeStr()+".txt";
			ofstream fs(filename);
			if(!fs.is_open()){error((string("can't open file [")+filename+"] for writing\n").c_str());}
			stringstream ss;
			int J = 0;
			int tf1 = memreport_last_frame;
			int tf2 = getGameTicks();
			int dtf = tf2-tf1;
			ss << fstring("Incremental memory report for frames %d .. %d (%d frames)\n", tf1, tf2, dtf);
			
			int supertotal = 0;
			for(auto F2 = allocation_map.begin(); F2 != allocation_map.end(); F2++){
				stringstream ss_file;
				const char *key = F2->first;
				string alloc_file = F2->first;
				ss_file << alloc_file + ":\n";//":\t\t";
				auto &r_file2 = F2->second;
				auto &r_file1 = prev_allocation_map[key];
				int filetotal = 0;
				
				//we want all the lines of current version that also exist in the previous version
				for(auto L2 = r_file2.begin(); L2 != r_file2.end(); L2++){
					int alloc_line = L2->first;
					if(!r_file1.count(alloc_line)){continue;}
					auto &r_line2 = L2->second;
					auto &r_line1 = r_file1[alloc_line];
					int count2 = r_line2.size();
					int count1 = r_line1.size();
					int size = 0;
					if(count2){size = r_line2[0].size;}
					int total = size*(count2-count1);
					if(total){ss_file << fstring(" @%d: %d x %d b = %d b\n",alloc_line,count2-count1,size,total);}
					supertotal += total;
					filetotal += total;
				}
				ss_file << fstring("file: %.3f kb\n",(float)filetotal/1024.f);
				if(filetotal){ss << ss_file.str();}
			}
			ss << fstring("user total: %.3f kb\n",(float)supertotal/1024.f);
			ss << fstring("sys  total: %.3f kb\n",(float)(total_size-prev_total_size)/1024.f);
			
			fs << ss.str();
			fs.close();
			printf("frame report saved to %s\n",filename.c_str());
		}
	}else{
		string filename = "logs/memreport";
		filename = filename + "_" + getCalendarDateStr()+"_"+getCalendarTimeStr()+".txt";
		ofstream fs(filename);
		if(!fs.is_open()){error((string("can't open file [")+filename+"] for writing\n").c_str());}
		stringstream ss;
		int J = 0;
		ss << fstring("Memory report for frame %d\n",getGameTicks());
		
		int supertotal = 0;
		for(auto F = allocation_map.begin(); F != allocation_map.end(); F++){
			string alloc_file = F->first;
			ss << alloc_file + ":\n";//":\t\t";
			auto &r_file = F->second;
			int filetotal = 0;
			for(auto L = r_file.begin(); L != r_file.end(); L++){
				int alloc_line = L->first;
				auto &r_line = L->second;
				int count = r_line.size();
				int size = 0;
				if(count){size = r_line[0].size;}
				int total = size*count;
				ss << fstring(" @%d: %d x %d b = %d b\n",alloc_line,count,size,total);
				supertotal += total;
				filetotal += total;
			}
			ss << fstring("file: %.3f kb\n",(float)filetotal/1024.f);
		}
		ss << fstring("user total: %.3f kb\n",(float)supertotal/1024.f);
		ss << fstring("sys  total: %.3f kb\n",(float)total_size/1024.f);
		
		fs << ss.str();
		fs.close();
		printf("frame report saved to %s\n",filename.c_str());
	}
	
	//save the current allocation map
	prev_allocation_map = allocation_map;
	has_prev_alloc_map = true;
	memreport_last_frame = getGameTicks();
	prev_total_size = total_size;
	printf("allocation map saved\n");
	return 0;
}

float noiseHelper(float x){
	return sin(sin(x)+sin(x*x));
}

float noiseFunc(vec2 pt){
	float x = pt.x;
	float y = pt.y;
	float hx = 1.f * noiseHelper(x)+
			   0.5f * noiseHelper(2*x) +
				0.25f * noiseHelper(4*x);
				
	float hy = 1.f * noiseHelper(y)+
			   0.5f * noiseHelper(2*y) +
				0.25f * noiseHelper(4*y);
	return hx+hy;
}

int cmd_genterrain(int argc, char **argv){
	int ix, iy;
	int numx, numy;
	float size = 10.f;
	int slices = 10;
	numx = numy = 10;
	vec3 center = vec3(0,0,10); //dafuq? why vec3, why 0,0,10?
	for(ix = -numx/2; ix < numx/2; ix++){
		for(iy = -numy/2; iy < numy/2; iy++){
			vec2 corner = vec2(ix,iy)*size + vec2(center.x,center.y);
			
			gridKind g(slices, slices, size, size);
			for(int jx = 0; jx < slices; jx++){
				for(int jy = 0; jy < slices; jy++){
					int tilewidth = size/slices;
					vec2 pt = corner+vec2(jx,jy)*tilewidth;
					float height = noiseFunc(pt);
					g[jx][jy] = height;
				}
			}
			
		}
	}
	return 0;
}

int cmd_texture_browser(int argc, char** argv){
	openTextureBrowser();
	return 0;
}

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
	keybinds->binds["+F9"].cmd = "memreport";
	keybinds->binds["+F10"].cmd = "memreport -i";
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
	console->addCommand({"printf","toggle debug message printing\n",cmd_printf});
	console->addCommand({"memreport","report the memory usage to a file\n"
		"args: -i -- incremental, reports difference from previous report\n",cmd_memreport});
	console->addCommand({"genterrain","generate some land\n",cmd_genterrain});
	console->addCommand({"texbrowser","browse for textures\n",cmd_texture_browser});
}








