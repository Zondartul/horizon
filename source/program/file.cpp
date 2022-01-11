#include "file.h"
#include "globals.h"
#include "stdio.h"
#include "stdlib.h"
#include <vector>
using std::vector;
//#include <filesystem>
//#include "boost/filesystem.hpp"
//using namespace boost::filesystem;
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
	SDL_ClearError();
	const char *basepath = SDL_GetBasePath();
	if(!basepath){
		const char *err = SDL_GetError();
		if(err){error("locateResource: SDL error: [%s]\n",err);}
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
	if(T == "model"){abspath = basepath + string()+resourcepath+"models/" /*+ name + "/" + name +*/ + namedir + sname + "/"+sname + ".obj";}


	//printf("locateResource: attempting [%s]\n",abspath.c_str());
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


string getProgramDirectory(){
	return string(SDL_GetBasePath());
}
string getConfigDirectory(){
	return toCanonicalPath(getProgramDirectory()+"..\\config\\");
}

string toCanonicalPath(string dir){
	printf("toCanonicalPath(%s) = ",dir.c_str());
	vector<string> subdirs;
	string S;
	for(int I = 0; I < dir.length(); I++){
		char C = dir[I];
		if((C == '\\')||(C == '/')){
			if(S == ".."){
				//S = subdirs.back();
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
	S = "";
	for(auto I = subdirs.begin(); I != subdirs.end(); I++){
		S += *I;
		if((I+1)!= subdirs.end()){S+="\\";}
	}
	printf("%s\n",S.c_str());
	return S;
}

vector<string> getDirectoryList(string dirname){
	vector<string> res;
	//path dirpath(dir);
	printf("direntries begin\n");

	DIR *d = opendir(dirname.c_str());
	while(1){
		dirent *de = readdir(d);
		if(!de){break;}
		string S = de->d_name;
		printf("directory_entry [%s]\n",S.c_str());
		res.push_back(S);
	}
	closedir(d);
	//directory_iterator I(dir);
	//for(directory_entry &E : I){
	//	string S = E.path().string();
	//	printf("directory_entry [%s]\n",S.c_str());
	//	res.push_back(S);
	//}


	//for(directory_iterator I(dirpath); I != end(I); I++){
		//directory_entry E = *I;
		//string S = E.path.string();
	//	printf("directory_entry [%s]\n",S.c_str());
	//	res.push_back(S);
	//}
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



