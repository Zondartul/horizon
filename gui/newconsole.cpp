#include "../gui/Gui2.h"
void consolecallback(void *arg);
void parse(string str);
void userparse(string str);

GUI2frame *newconsole;
GUI2textEntry *textentry;
GUI2label *text;
//GUIscrollBar *scrollbar;

map<string, double> convars;
map<string, funcptr> confuncs;

void OpenNewConsole(GUI2base *GUI){
	newconsole = new GUI2frame;
	newconsole->setPos(32,64);
	newconsole->setSize(400,300);
	newconsole->title = "Console";
	newconsole->setParent(GUI);
	
	textentry = new GUI2textEntry;
	textentry->setPos(0,300-BRDT-24);
	textentry->setSize(400-BRDB,24);
	textentry->setParent(newconsole);
	textentry->callback = consolecallback;
	textentry->text = "";
	
	/*
	scrollbar = new GUIscrollBar;
	scrollbar->setPos(0,0);
	scrollbar->setSize(400-22,300-BRDT-26);
	scrollbar->insideSize.y = 1200;
	scrollbar->setParent(newconsole);
	*/
	
	text = new GUI2label;
	text->setPos(0,0);
	text->setSize(400,300-BRDT-24);//(400-32,1200);
	text->text = "";
	text->setParent(newconsole);
	//text->setParent(scrollbar);
	
	convars["variable"] = 1;
	convars["camera_mouse_capture"] = 0;
}

void consolecallback(void *arg){
	string msg = *((string*)arg);
	if(msg == "enter"){
		string entered = textentry->text;
		textentry->text = "";
		//cout << "Entered: \"" << entered << "\"\n";
		userparse(entered);
	}
}

void cunprintfromtop(){
	int p = text->text.find_first_of('\n');
	text->text.erase(0, p != -1? p+1 : 0);
}
int countlines(string str){
	int N = 0;
	for(int I = 0; I < str.length(); I++)
		if(str[I] == '\n')
			N++;
	return N;
}
void cprint(string str){
	while(countlines(text->text) > 12)
		cunprintfromtop();
	text->text = text->text+str+'\n';
}
void cunprint(){
	int p = text->text.find_last_of('\n');
	text->text.erase(p != -1? p : 0);
	p = text->text.find_last_of('\n');
	text->text.erase(p != -1? p+1 : 0);
}
void cclear(){
	text->text = "";
}
bool echoOn = true;
void userparse(string str){
	if(echoOn)
		cprint("[user]:> "+str);
	parse(str);
}

void parse(string str){
	vector<string> words;
	explode(str, words, ' ');
	if(words[0] == "help"){
		cprint("Built-in commands: help, unprint, echo, exit, clear, vars");
		cprint("Learned commands:");
		for (std::map<string,funcptr>::iterator it=confuncs.begin(); it!=confuncs.end(); ++it){
			string first = it->first;
			funcptr second = it->second;
			cprint(first+"   "+(int)second);
		}
	}else if(words[0] == "unprint")
		cunprint();
	else if(words[0] == "echo"){
		if(words[1] == "off")
			echoOn = false;
		else if(words[1] == "on")
			echoOn = true;
		else{
			int p = str.find_first_of(' ');
			cprint(str.substr(p == -1? 0 : p+1, -1));
		}
	}else if(words[0] == "exit"){
		printf("\n\nShutting down <3\n\n");
		PostQuitMessage(0);
	}else if(words[0] == "clear"){
		cclear();
	}else if(words[0] == "vars"){
		cprint("list of console variables:");
		for (std::map<string,double>::iterator it=convars.begin(); it!=convars.end(); ++it){
			string first = it->first;
			double second = it->second;
			cprint(first+"   "+second);
		}
	}else{
		if(convars.count(words[0])){
			if(words.size()>1){
				convars[words[0]] = atof(words[1].c_str());
				cprint(words[0]+" has been set to "+atof(words[1].c_str()));
			}else
				cprint(words[0]+" is currently "+convars[words[0]]);
			return;
		}
		for (std::map<string,funcptr>::iterator it=confuncs.begin(); it!=confuncs.end(); ++it){
			string first = it->first;
			if(first == words[0]){
				funcptr func = it->second;
				func(NULL);
				return;
			}
		}
		cprint("Unknown Command \""+words[0]+"\"");
	}
}