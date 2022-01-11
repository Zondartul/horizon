#include "../gui/Gui2.h"
#include "../main/control.h" //bad
#include "../resource/textureloader.h"
#include "../game/physics.h"
GUIframe *Console;

int ConsoleNumLines = 0;
struct concommand
{
	string cmd;
	string desc;
	funcptr func;
};

vector<concommand> cmdlist;



void ConsoleParse(string text)
{	
	
	if(Console!=NULL)
	{
		GUIlabel *L = (GUIlabel*)(Console->findByTag("Scroll")->findByTag("Text"));
		if(L==NULL){printf("can't findByTag(Text)\n");}
		//printf("adding text <%s> to <%s>\n",text.c_str(), L->text.c_str());	
		L->text += '\n';
		ConsoleNumLines++;
		L->text += text;
		if(ConsoleNumLines>60){L->text.erase(0,L->text.find("\n")+1);ConsoleNumLines--;}
	}
	else{printf("Console missing\n");}
	
}
void ParseCommand(string inputString)
{
		vector<string> args;
		int nargs = explode(inputString, args, ' ');
		
		bool found = false;
		for(int i=0;i<cmdlist.size();i++)
		{
			if(cmdlist[i].cmd==args[0])
			{
				//ConsoleParse(cmdlist[i].desc);
				if(cmdlist[i].func){cmdlist[i].func((void*)&args);}
				found = true;
				break;
			}
		}
		if(!found){ConsoleParse("[color=red]Unknown command.[/color]");}
}
void MenuConsoleConEntryCallback(void *arg)
{
	GUItextEntry *E = (GUItextEntry*)arg;
	if(E->text[E->text.length()-1]=='\n')
	{
		E->text.erase(E->text.length()-1);
		ConsoleParse(E->text);
		
		// Console Command search happens here
		ParseCommand(E->text);
		
		E->text.erase();
	}
}

void makeInvisible(void *arg)
{
	Console->visible = false;
	printf("ninja\n");
}

void OpenMenuConsole()
{
	if(Console==NULL)
	{
		Console = new GUIframe;
		Console->setSize(256,512);
		Console->setParent(GUI);
		Console->setPos(64,64);
		Console->color_panel = {32,32,64};
		((GUIbutton*)Console->findByTag("btnClose"))->func = &makeInvisible;
		
		GUIscrollBar *Scroll = new GUIscrollBar;
		Scroll->setSize(Console->size.x-22,Console->size.y-64-3);
		Scroll->insideSize.y = 1024;
		Scroll->setParent(Console);
		Scroll->tag = "Scroll";
		
		GUIlabel *ConText = new GUIlabel;
		ConText->setSize(Scroll->size.x-4,1024);
		ConText->setPos(0,0);
		ConText->setParent(Scroll);
		ConText->tag = "Text";
		ConText->setdock(false,false,false,true);
		
		GUItextEntry *ConEntry = new GUItextEntry;
		ConEntry->setSize(256-4,32-4);
		ConEntry->setPos(0,512-64);
		ConEntry->multiline = true;
		ConEntry->setParent(Console);
		ConEntry->callback = MenuConsoleConEntryCallback;
		ConEntry->arg = (void *)ConEntry;
		ConEntry->setdock(true,true,false,true);
		ConEntry->scissor = false;
		ConEntry->text = "";
		
		ConText->text = "[color=ff0000]Red[/color], [color=0x00FF00]Green[/color], [color=blue]Blue[/color].";
		stringstream k;
		for(int i = 1;i<200;i++){k.str("");k<<i;ConsoleParse(k.str());}
	}
}

void cmd_help(void *arg)
{
	for(int i=0;i<cmdlist.size();i++)
	{
		ConsoleParse("[color=orange]"+cmdlist[i].cmd+"[/color]");
		ConsoleParse("[color=yellow] -"+cmdlist[i].desc+"[/color]");
	}
}

void cmd_hi(void *arg){ConsoleParse("[color=green]hello![/color]");}
void cmd_exit(void *arg){exit(0);}
void cmd_resetcam(void *arg)
{
	ConsoleParse("[color=white]Camera reset.[/color]");
	CamAngle = quat::fromAngleAxis(0,{0,0,1});
	SomeVec1 = {0,0,0};
}
void cmd_cls(void *arg)
{
	GUIlabel *L = (GUIlabel*)(Console->findByTag("Scroll")->findByTag("Text"));
	L->text.erase();
	ConsoleNumLines = 0;
}
void cmd_color(void *arg)
{
	vector<string>* args = (vector<string>*)arg;
	if(args->size()>3)
	{
		int r = atoi((*args)[1].c_str());
		int g = atoi((*args)[2].c_str());
		int b = atoi((*args)[3].c_str());
		ConsoleParse("Color set to "+itoa(r)+","+itoa(g)+","+itoa(b));
		Console->color_panel = {r,g,b};
	}
}
void cmd_textureLoad(void *arg)
{
	vector<string>* args = (vector<string>*)arg;
	if(args->size()>1)
	{
		texture n = GenTextureBMP((*args)[1].c_str());//temp copy
		if(n.t!=0)
		{
			AllTextures.push_back(n); //permanent copy
		}
	}
}
void cmd_textureStore(void *arg)
{
	for(int i = 0;i<AllTextures.size();i++)
	{
		texture t = AllTextures[i];
		ConsoleParse(itoa(i)+" ["+AllTextures[i].name+"] "+itoa(t.width));
		ConsoleParse("[img]"+AllTextures[i].name+"[/img]");
		ConsoleParse(" ");
		ConsoleParse(" ");
		ConsoleParse(" ");
	}
}
void cmd_texture(void *arg)
{
	//this really should go into textureloader.h
	vector<string>* args = (vector<string>*)arg;
	if(args->size()<2){ConsoleParse("Usage: texture C:/Stride/textures/cube.bmp");return;}
	//string path = "C:/Stride/textures/cube.bmp";
	string path = (*args)[1];
	string ext = path; ext.erase(0,ext.find_last_of(".")+1);
	if((ext=="bmp")|(ext=="jpg")|(ext=="png")|(ext=="gif")){ConsoleParse("it's a "+ext);}else
	{ConsoleParse("[color=red]Unknown extension: "+ext+"[/color]");}
	FILE * img;
	img = fopen(path.c_str(), "rb");
	if(img == NULL){ConsoleParse("[color=red]Texture file not found: "+path+"[/color]");return;}
	
	if(ext=="bmp")
	{
		char dword[5];
		unsigned int filesize;
		fread(dword,2,1,img);dword[2]=0;dword[3]=0;dword[4]=0;
		fread(&filesize,4,1,img);
		printf("\nId: [%s]/%X", dword,*((unsigned long *)dword));
		printf("\nFilesize: [%f]kb",(double)filesize/1024);
		fread(dword,4,1,img);dword[4]=0;
		printf("\nReserved: [%s]/0x%X",dword,*((unsigned long *)dword));
		fread(dword,4,1,img);dword[4]=0;
		printf("\nData offset: [%s]/0x%X/%u",dword,*((unsigned long *)dword),*((unsigned long *)dword));
		fread(dword,4,1,img);dword[4]=0;
		printf("\nHeader size: [%s]/0x%X/%u",dword,*((unsigned long *)dword),*((unsigned long *)dword));
		fread(dword,4,1,img);dword[4]=0;
		printf("\nwidth: [%s]/0x%X/%u",dword,*((unsigned long *)dword),*((unsigned long *)dword));
		fread(dword,4,1,img);dword[4]=0;
		printf("\nheight: [%s]/0x%X/%u",dword,*((unsigned long *)dword),*((unsigned long *)dword));
		fread(dword,2,1,img);dword[2]=0;dword[3]=0;dword[4]=0;
		printf("\nplanes: [%s]/0x%X/%u",dword,*((unsigned long *)dword),*((unsigned long *)dword));
		fread(dword,2,1,img);dword[2]=0;dword[3]=0;dword[4]=0;
		printf("\nbits per pixel: [%s]/0x%X/%u",dword,*((unsigned long *)dword),*((unsigned long *)dword));
		fread(dword,4,1,img);dword[4]=0;
		printf("\ncompression: [%s]/0x%X/%u",dword,*((unsigned long *)dword),*((unsigned long *)dword));
		fread(dword,4,1,img);dword[4]=0;
		printf("\nbitmap data size: [%s]/0x%X/%u",dword,*((unsigned long *)dword),*((unsigned long *)dword));
		fread(dword,4,1,img);dword[4]=0;
		printf("\nhresolution: [%s]/0x%X/%u",dword,*((unsigned long *)dword),*((unsigned long *)dword));
		fread(dword,4,1,img);dword[4]=0;
		printf("\nvresolution: [%s]/0x%X/%u",dword,*((unsigned long *)dword),*((unsigned long *)dword));
		fread(dword,4,1,img);dword[4]=0;
		int colors = *((unsigned long *)dword);
		printf("\ncolors: [%s]/0x%X/%u",dword,*((unsigned long *)dword),*((unsigned long *)dword));
		fread(dword,4,1,img);dword[4]=0;
		printf("\nimportant colors: [%s]/0x%X/%u",dword,*((unsigned long *)dword),*((unsigned long *)dword));
		
		printf("\ndone\n");
		//ConsoleParse("Id: "+id+", size: "+itoa(((double)filesize)/1024)+"kb");
		fclose(img);
	}
	
	ConsoleParse("[color=green]OK[/color]");
}

void cmd_quat(void *arg)
{
	vector<string>* args = (vector<string>*)arg;
	string argstr = "[color=green]args:";
	for(int i = 0;i<args->size();i++)
	{
		argstr += " [[color=yellow]"+(*args)[i]+"[color=green]]";
	}
	argstr+="[/color]";
	ConsoleParse(argstr);
	bool found = false;
	
	if(args->size()>1)
	{
	if((*args)[1]=="form")
	{
		found = true;
		ConsoleParse("create quaternion from angle in degrees and axis");
		if(args->size()>5)
		{
			double w = atof((*args)[2].c_str());
			double x = atof((*args)[3].c_str());
			double y = atof((*args)[4].c_str());
			double z = atof((*args)[5].c_str());
			quat q = quat::fromAngleAxis(w,x,y,z);
			ConsoleParse("result: "+itoa(q.w)+","+itoa(q.v.x)+","+itoa(q.v.y)+","+itoa(q.v.z));
			//{cos(w*deg2rad/2),((vec){x,y,z}).norm()*sin(w*deg2rad/2)};
			//ConsoleParse
		}
	}
	if((*args)[1]=="rot")
	{
		found = true;
		ConsoleParse("rotate vector A by quat B");
		if(args->size()>4)
		{
			double x = atof((*args)[2].c_str());
			double y = atof((*args)[3].c_str());
			double z = atof((*args)[4].c_str());
			vec v = {x,y,z};
			ConsoleParse("vec: "+itoa(x)+","+itoa(y)+","+itoa(z));
			if(args->size()>8)
			{
				double w = atof((*args)[5].c_str());
				x = atof((*args)[6].c_str());
				y = atof((*args)[7].c_str());
				z = atof((*args)[8].c_str());
				quat q = {w,{x,y,z}};
				ConsoleParse("quat: "+itoa(x)+","+itoa(y)+","+itoa(z)+","+itoa(w));
				
				v = q.rotateVector(v);
				ConsoleParse("Result: "+itoa(v.x)+","+itoa(v.y)+","+itoa(v.z));
			}
		}
	}
	if((*args)[1]=="read")
	{
		found = true;
		ConsoleParse("get angle/axis from quaternion");
		if(args->size()>5)
		{
			double w = atof((*args)[2].c_str());
			double x = atof((*args)[3].c_str());
			double y = atof((*args)[4].c_str());
			double z = atof((*args)[5].c_str());
			quat q = {w,{x,y,z}};
			w = q.getRotationAngle();
			vec v = q.getRotationAxis();
			ConsoleParse("Angle: "+itoa(w)+", Axis:"+itoa(v.x)+","+itoa(v.y)+","+itoa(v.z));
		}
	}
	if((*args)[1]=="add")
	{
		found = true;
		ConsoleParse("add rot.q. A to rot.q. B");
		if(args->size()>9)
		{
			double w = atof((*args)[2].c_str());
			double x = atof((*args)[3].c_str());
			double y = atof((*args)[4].c_str());
			double z = atof((*args)[5].c_str());
			quat q1 = {w,{x,y,z}};
				
			w = atof((*args)[6].c_str());
			x = atof((*args)[7].c_str());
			y = atof((*args)[8].c_str());
			z = atof((*args)[9].c_str());
			quat q2 = {w,{x,y,z}};
			
			q1 = q1*q2;
			ConsoleParse("result: "+itoa(q1.w)+","+itoa(q1.v.x)+","+itoa(q1.v.y)+","+itoa(q1.v.z));
		}
	}
	}
	
	if(!found)
	{
		ConsoleParse("unknown operation, try [rot][add][form][read]");
	}
}

void cmd_test(void *arg)
{
	vector<string>* args = (vector<string>*)arg;
	string argstr = "[color=green]args:";
	for(int i = 0;i<args->size();i++)
	{
		argstr += " [[color=yellow]"+(*args)[i]+"[color=green]]";
	}
	argstr+="[/color]";
	ConsoleParse(argstr);
	/*
	vector<string> v;
	int n = explode("test of text",v,' ');
	stringstream k;
	k.str(""); k<<n;
	ConsoleParse(k.str());
	string argstr = "[color=green]args:";
	for(int i = 0;i<n;i++)
	{
		argstr += " [[color=yellow]"+v[i]+"[color=green]]";
	}
	argstr+="[/color]";
	if(v[1]=="of"){ConsoleParse("okay");}
	if(v[1]=="of "){ConsoleParse("not okay");}
	ConsoleParse(argstr);
	*/
}

void initConCommands()
{
	printf("assigning 1\n");
	cmdlist.push_back({"hi","prints hello",&cmd_hi});
	cmdlist.push_back({"poni","my little pony",NULL});
	cmdlist.push_back({"patapon","pata pata pata pon",NULL});
	cmdlist.push_back({"help","console command documentation",&cmd_help});
	cmdlist.push_back({"exit","exit the program",&cmd_exit});
	cmdlist.push_back({"test","does things", &cmd_test});
	cmdlist.push_back({"quat","quaternion operations", &cmd_quat});
	cmdlist.push_back({"camreset","reset camera position/orientation",&cmd_resetcam});
	cmdlist.push_back({"texture","image file info",&cmd_texture});
	cmdlist.push_back({"color","set color",&cmd_color});
	cmdlist.push_back({"textureload","load texture",&cmd_textureLoad});
	cmdlist.push_back({"texturestore","repo of all loaded textures",&cmd_textureStore});
	cmdlist.push_back({"cls","clear screen",&cmd_cls});
	printf("assigning 2\n");
	printf("\nsize of physBody = %d, * = %d\n",sizeof(physBody),sizeof(physBody*));
	printf("{0,0,1}.angle({1,0,0}) = %f\n", ((vec){0,0,1}).angle({1,0,0}));
	printf("glGet(GL_EXTENSIONS) = %s",glGetString(GL_EXTENSIONS));
}