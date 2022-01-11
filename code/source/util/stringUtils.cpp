#include "stringUtils.h"
#include "stdio.h"
#include "globals.h"
#include "vec.h"
#include "string.h"

string itoa(int N){
	char buff[20];
	snprintf(buff,19,"%d",N);
	return string(buff);
}
string ftoa(double N){
	char buff[20];
	snprintf(buff,19,"%f",N);
	return string(buff);
}
string ftoa(double N, int prec){
	char format[20];
	snprintf(format,19,"%%.%df",prec);
	char buff[20];
	snprintf(buff,19,format,N);
	return string(buff);
}

//----------------------- vec.h --------------------------------------
#include "vec.h"
//----------------------------------------------- vec3
string toString(vec3 V){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f,%.3f)",V.x,V.y,V.z);
	return string(buff);
}

template<> vec3 fromString<vec3>(const string S){
	//printf("fromString<vec3>(%s)\n",S.c_str());
	vec3 v;
	sscanf(S.c_str(),"(%f,%f,%f)",&v.x,&v.y,&v.z);
	return v;
}

//----------------------------------------------- vec2
string toString(vec2 V){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f)",V.x,V.y);
	return string(buff);
}

template<> vec2 fromString<vec2>(const string S){
	vec2 v;
	sscanf(S.c_str(),"(%f,%f)",&v.x,&v.y);
	return v;
}
//----------------------------------------------- rect
string toString(rect R){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f + %.3f,%.3f)",R.start.x,R.start.y,R.size.x,R.size.y);
	return string(buff);
}

template<> rect fromString<rect>(const string S){
	vec2 start;
	vec2 size;
	sscanf(S.c_str(),"(%f,%f + %f,%f)",&start.x,&start.y,&size.x,&size.y);
	vec2 end = start+size;
	return rect(start,end);
}
//----------------------------------------------- AABB
string toString(AABB aabb){
	return fstring("(%.3f,%.3f,%.3f + %.3f,%.3f,%.3f)",aabb.start.x,aabb.start.y,aabb.start.y,aabb.size.x,aabb.size.y,aabb.size.z);
}

template<>  AABB fromString<AABB>(const string S){
	vec3 start;
	vec3 size;
	sscanf(S.c_str(),"(%f,%f,%f + %f,%f,%f)",&start.x,&start.y,&start.z,&size.x,&size.y,&size.z);
	vec3 end = start+size;
	return AABB(start,end);
}
//----------------------------------------------- mat4
string toString(mat4 M){
	return "<mat4>";
}

template<> mat4 fromString<mat4>(const string S){
	return mat4();
}
//-------------------- camera.h --------------------------------
#include "camera.h"
//----------------------------------------------- camprojection
string toString(camprojection cpj){return toString(cpj.MVP)+":"+toString(cpj.pos);}

template<> camprojection fromString<camprojection>(const string S){
	vector<string> VS = explode(S,':');
	mat4 MVP;
	vec3 pos;
	if(VS.size() == 2){
		MVP = fromString<mat4>(VS[0]);
		pos = fromString<vec3>(VS[1]);
	}
	camprojection proj;
	proj.MVP = MVP;
	proj.pos = pos;
	return proj;
}
//-------------------- generic ---------------------------------
//----------------------------------------------- void*
string toString(void *p){
	char buff[80];
	snprintf(buff,79,"0x%p",p);
	return string(buff);
}

template<> void *fromString<void*>(const string S){
	void *p;
	sscanf(S.c_str(),"0x%p",&p);
	return p;
}
//----------------------------------------------- bool
string toString(bool B){
	return itoa(B);
}

template<> bool fromString<bool>(const string S){
	return (bool)atoi(S.c_str());
}
//----------------------------------------------- int
string toString(int I){
	return itoa(I);
}

template<> int fromString<int>(const string S){
	//printf("fromString<int>(%s)\n",S.c_str());
	int n;
	sscanf(S.c_str(),"%d",&n);
	return n;
}
//----------------------------------------------- float
string toString(float f){
	return ftoa(f);
}

template<> float fromString<float>(const string S){
	float f;
	sscanf(S.c_str(),"%f",&f);
	return f;
}
//----------------------------------------------- string
string toString(string S){
	return S;
}

template<> string fromString<string>(const string S){
	//printf("fromString<string>(%s)\n",S.c_str());
	return S;
}
//-------------------- texture.h -------------------------------
#include "texture.h"
#include "resource.h"
//----------------------------------------------- texture*
string toString(texture *t){
	if(!t){return fstring("tex:[null]");}
	return fstring("tex:[%s]",t->name.c_str());
}

template<> texture* fromString<texture*>(const string S){
	if(S == "tex:[null]"){return 0;}
	char buff[80];
	sscanf(S.c_str(),"tex:[%[^]]]",buff);
	printf("fromString<texture*>: S = [%s]\n",S.c_str());
	printf("fromString<texture*>: buff = [%s]\n",buff);
	string name(buff);
	return getTexture(name);
}
//-------------------- font.h ----------------------------------
#include "fonts.h"
//----------------------------------------------- font*
string toString(font *f){
	if(!f){return fstring("font:[null]");}
	return fstring("font:[%s]",f->name.c_str());
}

template<> font* fromString<font*>(const string S){
	if(S == "font:[null]"){return 0;}
	char buff[80];
	sscanf(S.c_str(),"font:[%[^]]]",buff);
	printf("fromString<font*>: S = [%s]\n",S.c_str());
	printf("fromString<font*>: buff = [%s]\n",buff);
	string name(buff);
	return getFont(name);
}

//------------------- gui_internal.h
#include "GUI_internal.h"
//------------------------------------------------ alignmentKind
string toString(GUIe_alignment al){
	switch(al){
		case GUIa::None: return "ALIGN_NONE";
		case GUIa::Left: return "ALIGN_LEFT";
		case GUIa::Right: return "ALIGN_RIGHT";
		case GUIa::Top: return "ALIGN_TOP";
		case GUIa::Bottom: return "ALIGN_BOTTOM";
		case GUIa::Center: return "ALIGN_CENTER";
		default: printf("toString(GUI_alignment): unforseen switch-case\n"); return "ALIGN_NONE";
	}
}

template<> GUIe_alignment fromString<GUIe_alignment>(const string S){
	if(S == "ALIGN_NONE"){return GUIa::None;}
	if(S == "ALIGN_LEFT"){return GUIa::Left;}
	if(S == "ALIGN_RIGHT"){return GUIa::Right;}
	if(S == "ALIGN_TOP"){return GUIa::Top;}
	if(S == "ALIGN_BOTTOM"){return GUIa::Bottom;}
	if(S == "ALIGN_CENTER"){return GUIa::Center;}
	printf("fromString<alignmentKind>: unforseen case\n");
	return GUIa::None;
}
//enum alignmentKind{ALIGN_NONE,ALIGN_LEFT,ALIGN_RIGHT,ALIGN_TOP,ALIGN_BOTTOM,ALIGN_CENTER};

//======================= other stuff ====================================
//string toString(vec2 V){return toString(tovec2(V));}
//string toString(vec3 V){return toString(tovec3(V));}
string operator+(const string S, const int N){return S+itoa(N);}
string operator+(const string S, const double N){return S+ftoa(N);}

string fstring(const char *format, ...){
	string str;
	printfify(format,str);
	return str;
}



void stringUtils_error(string S){error(S.c_str());}

//---------------------------------------
vector<string> explode(string S, char del){
	vector<string> res;
	res.push_back("");
	const char *str = S.c_str();
	char C = *str++;

	while(C != 0){
		if(C != del){res.back().push_back(C);}
		else{res.push_back("");}
		C = *str++;
	}
	return res;
}


char **explode(const char *str, char del){
	char *str2 = (char *)malloc(strlen(str)+1);
	strcpy(str2,str);

	char **arr = (char **)malloc(80);
	int arrI = 0;

	//char C = *str2++;
	//int strmax = strlen(str2);
	//char *state = 0;
	//const char dels[] = {del,0};
	//char *res = strtok_s(str2,&strmax,dels,&state);
	//while(res){
	//	arr[arrI++] = res;
	//	res = strtok_s(0,&strmax,dels,&state);
	//}
	while(*str2){
		char *str3 = str2;
		while((*str3) && (*str3 == del)){str3++;}
		if(!*str3){break;}
		arr[arrI++] = str3;
		while((*str3) && (*str3 != del)){str3++;}
		if(!*str3){break;}
		*str3 = 0;
		str2 = str3+1;
		if(arrI>=80){break;}
	}
	arr[arrI] = 0;
	return arr;
}

void argcargvtest(int argc, char **argv){
	printf("argc = %d\n",argc);
	for(int i = 0; i < 10; i++){
		if(argv[i]){
			printf("argv[%d]: %p [%s]\n",i,argv[i],argv[i]);
		}else{
			printf("argv[%d]: %p (null)\n",i,argv[i]);
			break;
		}
	}
}

int countargs(char **argv){
	int I = 0;
	while(argv[I] != 0){I++;}
	return I;
}

string escapeString(string S){
	string S2;
	char C;
	for(unsigned int I = 0; I < S.length(); I++){
		C = S[I];
		switch(C){
			case '\0': S2 += "\\0"; break;
			case '\n': S2 += "\\n"; break;
			case '\t': S2 += "\\t"; break;
			case '\r': S2 += "\\r"; break;
			default:
				S2 += C;
				break;
		}
	}
	return S2;
}

string unescapeString(string S){
	string S2;
	char C;
	char C2;
	for(unsigned int I = 0; I < S.length(); I++){
		C = S[I];
		if((I+1) < S.length()){C2 = S[I+1];}else{C2 = 0;}
		if(C == '\\'){
			switch(C2){
				case '0': S2 += '\0'; break;
				case 'n': S2 += '\n'; break;
				case 't': S2 += '\t'; break;
				case 'r': S2 += '\r'; break;
				case 0:
					error("broken escape sequence\n");
					break;
				default:
					error((string("unknown escape sequence [")+C+C2+"]\n").c_str());
					break;
			}
		}else{
			S2 += C;
		}
	}
	return S2;
}


char *stralloc(const char *str){
    int len = strlen(str);
    char *ptr = (char*)malloc(len+1);
    strcpy(ptr,str);
    return ptr;
}

void printString(string S){
	for(unsigned int I = 0; I < S.length();){
		string S2 = S.substr(I,80);
		printf("%s",S2.c_str());
		I += 80;
	}
}
