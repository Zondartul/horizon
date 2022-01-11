#include "file.h"
#include "globals.h"
#include "stdio.h"
#include "stdlib.h"

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
	
	string abspath = basepath;
	string resourcepath = "../resource/";
	string T = type;
	if(T == "font"){abspath += string()+resourcepath+"fonts/" + name + ".ttf";}
	if(T == "texture"){abspath += string()+resourcepath+"textures/" + name + ".jpg";}
	if(T == "model"){abspath += string()+resourcepath+"models/" + name + "/" + name + ".obj";}
	
	//printf("locateResource: attempting [%s]\n",abspath.c_str());
	FILE *f = fopen(abspath.c_str(),"rb");
	if(f){
		printf("%s \"%s\" found\n",type,name);
	}else{
		error("locateResource [%s] failed!\n",abspath.c_str());
	}
	fclose(f);
	return abspath;
}