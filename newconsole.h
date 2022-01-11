void consolecallback(void *arg);

GUIframe *newconsole;
GUItextEntry *textentry;

void OpenNewConsole(GUIbase *GUI){
	newconsole = new GUIframe;
	newconsole->setPos(128,128);
	newconsole->setSize(400,300);
	newconsole->title = "Console";
	newconsole->setParent(GUI);
	
	textentry = new GUItextEntry;
	textentry->setPos(0,0);
	textentry->setSize(400-BRDB,300-BRDT);
	textentry->setParent(newconsole);
	textentry->callback = consolecallback;
	textentry->multiline = true;
}

void consolecallback(void *arg){
	string msg = *((string*)arg);
	if(msg == "enter"){
		string entered = textentry->text;
		//textentry->text = "";
		cout << "Entered: \"" << entered << "\"\n";
	}
}