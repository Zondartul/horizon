#include "game/game.h"
#include "program/file.h"
#include "experimental/map_editor.h"
#include "experimental/gui_editor.h"
#include "experimental/editor2.h"
#include "gadgets/gui_texturebrowser.h"
#include "gadgets/console.h"
#include "util/global_vars_app.h"
#include "render/paint.h"
#include <cstring>
#include <fstream>
#include <sstream>
using std::ofstream;
using std::stringstream;

int cmd_pauseGame(int argc, char **argv){
    return 0;
}
int cmd_listKeybinds(int argc, char **argv){
	return 0;
}
int cmd_help(int argc, char **argv){
	auto& console = Ga->gs_console->g_console;
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
		case 7:openTestWindow7();break;
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
int cmd_printf(int argc, char **argv){
	//auto& printf_enabled = Gb->gs_main->g_printf_enabled;
	//printf_enabled = !printf_enabled; 
	return 0;
}
//int cmd_memreport(int argc, char **argv){
//	auto& allocation_map = Gb->gs_debug->g_allocation_map;
//	auto& total_size = Gb->gs_debug->g_total_size;
//	auto& has_prev_alloc_map = Gt->gs_commands->g_has_prev_alloc_map;
//	auto& prev_allocation_map = Gt->gs_commands->g_prev_allocation_map;
//	auto& prev_total_size = Gt->gs_commands->g_prev_total_size;
//	auto& memreport_last_frame = Gt->gs_commands->g_memreport_last_frame;
//	if(argc && (strcmp(argv[0], "-i")==0)){
//		if(has_prev_alloc_map){
//			string filename = "logs/memreport_inc";
//			filename = filename + "_" + getCalendarDateStr()+"_"+getCalendarTimeStr()+".txt";
//			ofstream fs(filename);
//			if(!fs.is_open()){error((string("can't open file [")+filename+"] for writing\n").c_str());}
//			stringstream ss;
//			//int J = 0;
//			int tf1 = memreport_last_frame;
//			int tf2 = getGameTicks();
//			int dtf = tf2-tf1;
//			ss << fstring("Incremental memory report for frames %d .. %d (%d frames)\n", tf1, tf2, dtf);
//			int supertotal = 0;
//			for(auto F2 = allocation_map.begin(); F2 != allocation_map.end(); F2++){
//				stringstream ss_file;
//				const char *key = F2->first;
//				string alloc_file = F2->first;
//				ss_file << alloc_file + ":\n";
//				auto &r_file2 = F2->second;
//				auto &r_file1 = prev_allocation_map[key];
//				int filetotal = 0;
//				for(auto L2 = r_file2.begin(); L2 != r_file2.end(); L2++){
//					int alloc_line = L2->first;
//					if(!r_file1.count(alloc_line)){continue;}
//					auto &r_line2 = L2->second;
//					auto &r_line1 = r_file1[alloc_line];
//					int count2 = r_line2.size();
//					int count1 = r_line1.size();
//					int size = 0;
//					if(count2){size = r_line2[0].size;}
//					int total = size*(count2-count1);
//					if(total){ss_file << fstring(" @%d: %d x %d b = %d b\n",alloc_line,count2-count1,size,total);}
//					supertotal += total;
//					filetotal += total;
//				}
//				ss_file << fstring("file: %.3f kb\n",(float)filetotal/1024.f);
//				if(filetotal){ss << ss_file.str();}
//			}
//			ss << fstring("user total: %.3f kb\n",(float)supertotal/1024.f);
//			ss << fstring("sys  total: %.3f kb\n",(float)(total_size-prev_total_size)/1024.f);
//			fs << ss.str();
//			fs.close();
//			printf("frame report saved to %s\n",filename.c_str());
//		}
//	}else{
//		string filename = "logs/memreport";
//		filename = filename + "_" + getCalendarDateStr()+"_"+getCalendarTimeStr()+".txt";
//		ofstream fs(filename);
//		if(!fs.is_open()){error((string("can't open file [")+filename+"] for writing\n").c_str());}
//		stringstream ss;
//		//int J = 0;
//		ss << fstring("Memory report for frame %d\n",getGameTicks());
//		int supertotal = 0;
//		for(auto F = allocation_map.begin(); F != allocation_map.end(); F++){
//			string alloc_file = F->first;
//			ss << alloc_file + ":\n";
//			auto &r_file = F->second;
//			int filetotal = 0;
//			for(auto L = r_file.begin(); L != r_file.end(); L++){
//				int alloc_line = L->first;
//				auto &r_line = L->second;
//				int count = r_line.size();
//				int size = 0;
//				if(count){size = r_line[0].size;}
//				int total = size*count;
//				ss << fstring(" @%d: %d x %d b = %d b\n",alloc_line,count,size,total);
//				supertotal += total;
//				filetotal += total;
//			}
//			ss << fstring("file: %.3f kb\n",(float)filetotal/1024.f);
//		}
//		ss << fstring("user total: %.3f kb\n",(float)supertotal/1024.f);
//		ss << fstring("sys  total: %.3f kb\n",(float)total_size/1024.f);
//		fs << ss.str();
//		fs.close();
//		printf("frame report saved to %s\n",filename.c_str());
//	}
//	prev_allocation_map = allocation_map;
//	has_prev_alloc_map = true;
//	memreport_last_frame = getGameTicks();
//	prev_total_size = total_size;
//	printf("allocation map saved\n");
//	return 0;
//}

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
int cmd_texture_browser(int argc, char** argv){
	openTextureBrowser();
	return 0;
}
void addKeybinds(){
	//auto& keybinds = Gb->sysInput->keybinds;//Gb->gs_keybinds->g_keybinds;
	//keybinds.binds["+R"].cmd = "reset";
	//keybinds.binds["+F"].cmd = "physbox";
	//keybinds.binds["+G"].cmd = "physbox 1";
	//keybinds.binds["+H"].cmd = "physbox 2";
	//keybinds.binds["+J"].cmd = "physbox 3";
	//keybinds.binds["+C"].cmd = "character";
	//keybinds.binds["+V"].cmd = "npc";
	//keybinds.binds["+Pause"].cmd = "pause";
	//keybinds.binds["+F8"].cmd = "framereport";
	//keybinds.binds["+F3"].cmd = "printf";
	//keybinds.binds["+F9"].cmd = "memreport";
	//keybinds.binds["+F10"].cmd = "memreport -i";
}
void addConsoleCommands(){
	auto& console = Ga->gs_console->g_console;
	console->addCommand({"help","prints information about installed commands\n",cmd_help});
	console->addCommand({"pause","pause the game\n",cmd_pauseGame});
	console->addCommand({"keybinds","print the current keybinds\n",cmd_listKeybinds});
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
	console->addCommand({"printf","toggle debug message printing\n",cmd_printf});
	//console->addCommand({"memreport","report the memory usage to a file\n"
	//	"args: -i -- incremental, reports difference from previous report\n",cmd_memreport});
	console->addCommand({"texbrowser","browse for textures\n",cmd_texture_browser});
}
