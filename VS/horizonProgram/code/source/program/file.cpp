//#include "stdio.h"
//#include "stdlib.h"
#ifdef USE_DIRENT
	#ifdef WIN32
		#include "Ext/dirent/dirent.h"
	#endif
	#ifdef LINUX
		#include <dirent.h>
	#endif
#endif
#ifdef USE_SDL
	#include "Ext/SDL2/SDL.h"
#endif
#include "program/file.h"
#include "util/globals_program.h"
//#include "resource/resource.h"
#include "util/res_or_err.hpp"
#include "util/zerror.hpp"
#include "util/stringUtils.h" //for SS()
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <iostream>
using std::vector;
using std::runtime_error;
using std::stringstream;

std::string getBasePath();

string fileToString(const char *filepath){
	FILE *fp;
	fp = fopen(filepath,"r");
	if(!fp){
		std::string basePath = getBasePath();
		//printf("can't open file '%s'\n",filepath);exit(1);
		stringstream ss;
		ss << "from [" << basePath << "]: ";
		ss << "can't open file " << filepath;
		throw runtime_error(ss.str());
	}
	string S;
	int C = fgetc(fp);
	while(C != EOF){
		S += C;
		C = fgetc(fp);
	}
	return S;
}

std::string getBasePath(){
#ifdef USE_SDL
	SDL_ClearError();
	const char *basepath = SDL_GetBasePath();
#else
	const char* basepath = "";
#endif
	if(!basepath){
#ifdef USE_SDL
		const char *err = SDL_GetError();
		if(err){push(zError(SS("locateResource: SDL error: [" << err << "]\n"), zError::ERROR, zError::INTERNAL));}
#endif
		basepath = "./";
	}
	return string(basepath);
}

std::string getResPath(std::string type){
	string resourcepath = "../resource/";
	std::string res = getBasePath() + resourcepath;
	if(type == "font"){res += "fonts/";}// + name + ".ttf";}
	if(type == "texture"){res += "textures/";}// + name + ".jpg";}
	if(type == "model_texture"){res += "textures/models/";}// + namedir + sname + "/"+"model_" + sname + ".jpg";}
	if(type == "model"){res += "models/";}// /*+ name + "/" + name +*/ + namedir + sname + "/"+sname + ".obj";}
	return res;
}

std::vector<std::string> get_resource_extensions(std::string type){
	if(type == "font"){return {".ttf"};}
	if(type == "texture"){return {".jpg", ".png"};}
	if(type == "model_texture"){return {".jpg", ".png"};}
	if(type == "model"){return {".obj"};}
	assert(!"false");
	return {};
}

Result<string,zError> locateResource(const char *type, const char *name){
	SplitPath split = split_path(name);

	//printf("locateResource: namedir [%s], sname [%s], ext [%s], type [%s]", namedir, sname, type);
	//if(T == "font"){abspath = basepath + string()+resourcepath+"fonts/" + name + ".ttf";}
	//if(T == "texture"){abspath = basepath + string()+resourcepath+"textures/" + name + ".jpg";}
	//if(T == "model_texture"){abspath = basepath + string() + resourcepath + "textures/" + "models/" + namedir + sname + "/"+"model_" + sname + ".jpg";}
	//if(T == "model"){abspath = basepath + string()+resourcepath+"models/" /*+ name + "/" + name +*/ + namedir + sname + "/"+sname + ".obj";}

	std::string respath = getResPath(type);
	auto extensions = get_resource_extensions(type);
	for(auto ext:extensions){
		std::string path = respath + split.dir + split.basename + ext;
		std::cout << "locateResource: try [" << path << "]" << std::endl;
		if(fileExists(path)){
			return path;
		} 
	}
	return zError(SS("Can't locate resource: " << type << " " << name));
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
	string S = getOSdirInitiator(); 
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


bool fileExists(string filename){
	FILE *f = fopen(filename.c_str(),"r");
	if(!f){return false;}
	else{fclose(f); return true;}
}

SplitPath split_path(string path){
	//std::string canon = toCanonicalPath(path);

	int lastSlash = path.find_last_of("/");

	SplitPath res;
	res.fullpath = path;
	if(lastSlash != -1){
		res.dir = path.substr(0,lastSlash+1);
		res.name = path.substr(lastSlash+1,-1);	
	}else{
		res.name = path;
	}
	int lastDot = res.name.find_last_of(".");
	if(lastDot != 0){
		res.basename = res.name.substr(0, lastDot);
		res.ext = res.name.substr(lastDot+1,-1);
	}else{
		res.basename = res.name;
	}

	return res;
}
