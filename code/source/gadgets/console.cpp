#include "console.h"
#include "renderLayer.h"
#include "paint.h"
#include "keybinds.h"
#include "hook.h"
#include "window.h"
#include "resource.h"
#include "inputController.h"
#include <vector>
#include <string>
using std::vector;
using std::string;
extern eventChannel *inputChannel;
extern eventChannel *globalChannel;
#include "stringUtils.h"

//------------------ dropDownTerminal ------------------------

dropDownTerminal::dropDownTerminal():terminalOn(false){
	layer = new renderLayer("console.terminal");
	layer->resetLayer = duplicateLayer(layer2D->resetLayer);
	layer->resetLayer->name = "console.terminal.reset";
	layer->resetLayer->special = true;
	addLayer(layer2D,layer);
	inputChannel->addListenerFront(this);
	globalChannel->addListenerFront(this);
}

dropDownTerminal::~dropDownTerminal(){
	removeLayer(layer);
	inputChannel->removeListener(this);
	globalChannel->removeListener(this);
}


void dropDownTerminal::print(string text){
	if(!terminalStrings.size()){terminalStrings.push_back("");}
	vector<string> strs = explode(text,'\n');
	for(int I = 0; I < strs.size(); I++){
		printf(" strs[%d] = [%s]\n",I,strs[I].c_str());
	}
	printf("\n");
	for(int I = 0; I < strs.size(); I++){
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
			if(string("Backspace") == K){	//backspace
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
			//consoleInputString = text;
		}else{
			if(string("`") == K){
				event.maskEvent();
				terminalOn = true;
				inputController->disableMouseCapture();
				return;
			}
		}
	}
	if(event.type == EVENT_FRAME){
		layer->clear();
		layer->reset();
		if(terminalOn){
			setLayer(layer);
			setTransparency(true);
			setColor(vec3(64,128,64));
			setAlpha(196.f);

			vec2 end = getScreenSize();
			end.y = end.y*1/3;
			rect R = rect(vec2(0,0),end);
			drawRect(R);

			setColor(vec3(128,255,128));
			setAlpha(255.f);
			setFont(getFont("cour 14"));
			int numstr = terminalStrings.size();
			setTextPos(vec2(0,end.y-0*14));
			printText(inputText);
			for(int I = 0; I < numstr; I++){
				setTextPos(vec2(0,end.y-(1+numstr-1-I)*14));
				printText(terminalStrings[I]);
			}
		}
	}
}

//--------------------------- consoleKind --------------------------------


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

consoleKind *console;

//--------------------- global --------------------------------------------

void cprint(const char *format, ...){
	string str;
	printfify(format,str);
	printf("cprint([%s])\n",str.c_str());
	console->print(str);
}

