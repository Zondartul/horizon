#include "gadgets/console.h"
#include "render/renderLayer.h"
#include "render/paint.h"
#include "input/keybinds.h"
#include "util/hook.h"
#include "program/window.h"
#include "resource/resource.h"
#include "util/stringUtils.h"
#include "input/input.h"
#include "util/global_vars_app.h"
#include "util/global_vars_gui.h"
#include "util/global_vars_render.h"
#include "util/global_vars_program.h"
#include "util/global_vars_util.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

dropDownTerminal::dropDownTerminal():terminalOn(false){
	auto& layer2D = Gr->gs_paint->g_layer2D;
	auto& inputChannel = Gp->sysInput->inputChannel;//Gr->gs_input->g_inputChannel;
	auto& globalChannel = Gu->sysEvent->globalChannel;//Gr->gs_event->g_globalChannel;
	layer = new renderLayer("console.terminal");
	layer->resetLayer = duplicateLayer(layer2D->resetLayer);
	layer->resetLayer->name = "console.terminal.reset";
	layer->resetLayer->special = true;
	addLayer(layer2D,layer);
	inputChannel.addListenerFront(this);
	globalChannel.addListenerFront(this);

	auto make_rt = [&](font* F, float ypos) {
		auto rT = new renderableText();
		rT->color = vec3(128, 255, 128);
		rT->pos = vec3(0, ypos, 0);
		rT->F = F;
		rT->upload();
		return rT;
	};

	auto* F = getFont("cour 14");
	float font_height = F->maxrect.size.y;

	for (unsigned int i = 0; i < 11; i++) {
		rTs.push_back(make_rt(F, font_height*i));
	}
	rT_input = make_rt(F, font_height * 11);
}

dropDownTerminal::~dropDownTerminal(){
	auto& inputChannel = Gp->sysInput->inputChannel;
	auto& globalChannel = Gu->sysEvent->globalChannel;
	removeLayer(layer);
	inputChannel.removeListener(this);
	globalChannel.removeListener(this);
}

void dropDownTerminal::print(string text){
	if(!terminalStrings.size()){terminalStrings.push_back("");}
	vector<string> strs = explode(text,'\n');
	for(unsigned int I = 0; I < strs.size(); I++){
		printf(" strs[%d] = [%s]\n",I,strs[I].c_str());
	}
	printf("\n");
	for(unsigned int I = 0; I < strs.size(); I++){
		if(I != 0){terminalStrings.push_back("");}
		string &S = terminalStrings.back();
		S += strs[I];
	}
}

void dropDownTerminal::onEvent(eventKind event){
	if(event.type == EVENT_KEY_DOWN){
		const char *K = event.keyboard.key;
		if(terminalOn){
			if(string("`") == K){
				event.maskEvent();
				terminalOn = false;
				return;
			}
			if(string("Backspace") == K){	
				event.maskEvent();
				if(inputText != ""){inputText.pop_back();}
			}
			if(string("Return") == K){
				event.maskEvent();
				onEnter(inputText);
				inputText = "";
			}
			if(event.keyboard.printchar){
				event.maskEvent();
				inputText += event.keyboard.printchar;
			}
		}else{
			if(string("`") == K){
				event.maskEvent();
				terminalOn = true;
				return;
			}
		}
	}
	if(event.type == EVENT_FRAME){
		layer->clear();
		layer->reset();
		if(terminalOn){
			setLayer(layer);

			/// Debug: can we see if the window's opengl context borks on resize?
			/// can we fix that?
			//setScissoring(false);

			/// end debug

			setTransparency(true);
			setColor(vec3(64,128,64));
			setAlpha(196.f);
			vec2 end = getScreenSize();
			end.y = end.y*1/3;
			rect R = rect(vec2(0,0),end);
			drawRect(R);
			setColor(vec3(128,255,128));
			setAlpha(255.f);
			
			//bool render_direct_text = false;
			bool render_rTs = true;

			//if (render_direct_text) {
				//setFont(getFont("cour 14"));
				//int numstr = terminalStrings.size();
				//setTextPos(vec2(0,end.y-0*14));
				/// echo user typing
				//printText(inputText);
				/// print everything else
				//for(int I = 0; I < numstr; I++){
					//setTextPos(vec2(0,end.y-(1+numstr-1-I)*14));
					//printText(terminalStrings[I]);
				//}
			//}
			
			if (render_rTs) {
				rT_input->text = inputText;
				rT_input->render(nullptr);

				for (unsigned int i = 0; i < rTs.size(); i++) {
					int i_str = terminalStrings.size() - rTs.size() + i;
					string S;
					if ((i_str >= 0) && (i_str < terminalStrings.size())) {
						S = terminalStrings[i_str];
					}
					rTs[i]->text = S;
					rTs[i]->render(nullptr);
				}
			}
		}
	}
}

consoleKind::consoleKind(){
	term = new dropDownTerminal();
	term->onEnter = [=](string text){this->run(text);};
}

consoleKind::~consoleKind(){
	delete term;
}

void consoleKind::addCommand(consoleCommand cmd){commands.push_back(cmd);}

consoleCommand *consoleKind::getCommand(string cmd){
	for(auto I = commands.begin(); I != commands.end(); I++){
		if(I->name == cmd){
			return &*I;
		}
	}
	return 0;
}

void consoleKind::run(string text){
	cprint("user:> %s\n",text.c_str());
	printf("parsing [%s]\n",text.c_str());
	char **argv = explode(text.c_str(),' ');
	int argc = countargs(argv);
	if(!argc){return;}
	auto cmd = getCommand(argv[0]);
	if(cmd){
		cmd->f(argc-1,argv+1);
	}else{
		cprint("unknown command: [%s]\n",argv[0]);
	}
}

void consoleKind::print(string text){
	if(term){term->print(text);}
}

void cprint(const char *format, ...){
	auto& console = Ga->gs_console->g_console;
	string str;
	printfify(format,str);
	printf("cprint([%s])\n",str.c_str());
	console->print(str);
}

