#include "file.h"
#include "globals.h"
#include "stdio.h"
#include "stdlib.h"
#include <vector>
using std::vector;
#include "dirent.h"
#include "resource.h"

string fileToString(const char *filepath){
	FILE *fp;
	fp = fopen(filepath,"r");
	if(!fp){printf("can't open file '%s'\n",filepath);exit(1);}
	string S;
	int C = fgetc(fp);
	while(C != EOF){
		S += C;
		C = fgetc(fp);
	}
	return S;
}

string locateResource(const char *type, const char *name){
#ifndef NO_SDL
	SDL_ClearError();
	const char *basepath = SDL_GetBasePath();
#else
	const char* basepath = "";
#endif
	if(!basepath){
#ifndef NO_SDL
		const char *err = SDL_GetError();
		if(err){error("locateResource: SDL error: [%s]\n",err);}
#endif
		basepath = "./";
	}
	string sname = name;
	string abspath = basepath;
	string resourcepath = "../resource/";
	string namedir = "";
	int lastSlash = sname.find_last_of("/");
	if(lastSlash != -1){namedir = sname.substr(0,lastSlash+1); sname = sname.substr(lastSlash+1,-1);}
	string T = type;
	if(T == "font"){abspath = basepath + string()+resourcepath+"fonts/" + name + ".ttf";}
	if(T == "texture"){abspath = basepath + string()+resourcepath+"textures/" + name + ".jpg";}
	if(T == "model_texture"){abspath = basepath + string() + resourcepath + "textures/" + "models/" + namedir + sname + "/"+"model_" + sname + ".jpg";}
	if(T == "model"){abspath = basepath + string()+resourcepath+"models/" + namedir + sname + "/"+sname + ".obj";}


	FILE *f = fopen(abspath.c_str(),"rb");
	if(!f && T == "texture"){abspath = basepath + string()+resourcepath+"textures/" + name + ".png"; f = fopen(abspath.c_str(),"rb");}
	if(!f && T == "model_texture"){abspath = basepath + string() + resourcepath + "textures/" + "models/" + namedir + sname + "/"+"model_" + sname + ".png";  f = fopen(abspath.c_str(),"rb");}
	if(f){
		printf("%s \"%s\" found\n",type,name);
	}else{
		error("locateResource [%s] failed!\n",abspath.c_str());
	}
	fclose(f);
	return abspath;
}

bool fileReadable(string S){
	FILE *fp = fopen(S.c_str(),"r");
	if(!fp){return false;}
	else{
		fclose(fp);
		return true;
	}
}

string getProgramDirectory(){
#ifndef NO_SDL
	return string(SDL_GetBasePath());
#else
	return string();
#endif
}
string getConfigDirectory(){
	return toCanonicalPath(getProgramDirectory()+"..\\config\\");
}
string getDataDirectory(){
	return toCanonicalPath(getProgramDirectory()+"..\\data\\");
}

string getOSdirSeparator(){
#ifdef WIN32
	return "\\";
#else
	return "/";
#endif
}

string getOSdirInitiator(){
#ifdef WIN32
	return "";
#else
	return "/";
#endif
}

//does this result in an absolute or relative path, or preserve it?
string toCanonicalPath(string dir){
	printf("toCanonicalPath(%s) = ",dir.c_str());
	vector<string> subdirs;
	string S;
	for(unsigned int I = 0; I < dir.length(); I++){
		char C = dir[I];
		if((C == '\\')||(C == '/')){
			if(S == ".."){
				subdirs.pop_back();
			}else if(S == ""){
				//do nothing
			}else{
				subdirs.push_back(S);
			}
			S = "";
		}else{
			S += C;
		}
	}
	string Sep = getOSdirSeparator();
	for(auto I = subdirs.begin(); I != subdirs.end(); I++){
		S += *I;
		if((I+1)!= subdirs.end()){S+=Sep;}
	}
	printf("%s\n",S.c_str());
	return S;
}

vector<string> getDirectoryList(string dirname){
	vector<string> res;
	printf("direntries begin\n");

	DIR *d = opendir(dirname.c_str());
	if(!d){return res;} //no directory
	while(1){
		dirent *de = readdir(d);
		if(!de){break;}
		string S = de->d_name;
		printf("directory_entry [%s]\n",S.c_str());
		res.push_back(S);
	}
	closedir(d);

	printf("direntries end\n");
	return res;
}

string getFileExtension(string filename){
	string S;
	int pos = filename.find_last_of(".");
	if(pos>0){S = filename.substr(pos+1,-1);}
	else S = "";
	printf("getFileExt(%s) = [%s]\n",filename.c_str(),S.c_str());
	return S;
}

string getFilename(string filepath){
	string S = toCanonicalPath(filepath);
	int pos = S.find_last_of("/");
	if(pos>0){S = S.substr(pos+1,-1);}
	printf("getFilename(%s) = [%s]\n",filepath.c_str(),S.c_str());
	return S;
}

string getFileNameNoExt(string filepath){
	string filename = getFilename(filepath);
	string S = filename;
	int pos = S.find_last_of(".");
	if(pos>0){S = S.substr(0,pos);}
	printf("getFileNameNoExt(%s) = [%s]\n",filepath.c_str(),S.c_str());
	return S;
}

bool isFolder(string filename){
	printf("isFolder(%s)? ",filename.c_str());
	DIR *d = opendir(filename.c_str());
	if(d){printf("yes\n"); closedir(d); return true;}
	else {printf("no\n");return false;}
}

texture *getFileIcon(string filename){
	string ext = getFileExtension(filename);
	texture *t = 0;
	if(isFolder(filename)){
		t = getTexture("gui/iconfolder");
	}else
	if(
	(ext == "txt")||
	(ext == "c")||
	(ext == "cpp")||
	(ext == "h")||
	(ext == "hpp")||
	(ext == "cfg")
	){
		t = getTexture("gui/iconfiletxt");
	}
	else
	if(
	(ext == "bmp")||
	(ext == "png")||
	(ext == "jpg")||
	(ext == "jpeg")||
	(ext == "img")
	){
		t = getTexture("gui/iconpicture");
	}
	else
	if(
	(ext == "zip")||
	(ext == "rar")||
	(ext == "gz")||
	(ext == "7z")
	){
		t = getTexture("gui/iconzipfile");
	}
	else
	if(
	(ext == "exe")
	){
		t = getTexture("gui/iconfileexe");
	}
	else
	if(
	(ext == "bat")
	){
		t = getTexture("gui/iconfilecmd");
	}
	else
	if(
	(ext == "dll")
	){
		t = getTexture("gui/icongear2");
	}
	else
	if(
	(ext == "a")
	){
		t = getTexture("gui/iconfilegear");
	}
	else{
		t = getTexture("gui/iconfile");
	}
	return t;
}

bool fileExists(string filename){
	FILE *f = fopen(filename.c_str(),"r");
	if(!f){return false;}
	else{fclose(f); return true;}
}


