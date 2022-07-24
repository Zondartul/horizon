#include <tuple> 
#include <sstream>
#include "stringUtils.h"
#include "stdio.h"
#include "globals.h"
#include "vec.h"
#include "string.h"
using namespace std;

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
#include "vec.h"
string toString(vec3 V){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f,%.3f)",V.x,V.y,V.z);
	return string(buff);
}
template<> vec3 fromString<vec3>(const string S){
	vec3 v;
	//std::ignore = sscanf(S.c_str(),"(%f,%f,%f)",&v.x,&v.y,&v.z);
	char c;
	stringstream ss(S);
	ss >> c >> v.x >> c >> v.y >> c >> v.z >> c;
	return v;
}
string toString(vec2 V){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f)",V.x,V.y);
	return string(buff);
}
template<> vec2 fromString<vec2>(const string S){
	vec2 v;
	//std::ignore = sscanf(S.c_str(),"(%f,%f)",&v.x,&v.y);
	char c;
	stringstream ss(S);
	ss >> c >> v.x >> c >> v.y >> c;
	return v;
}
string toString(rect R){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f + %.3f,%.3f)",R.start.x,R.start.y,R.size.x,R.size.y);
	return string(buff);
}
template<> rect fromString<rect>(const string S){
	vec2 start;
	vec2 size;
	//std::ignore = sscanf(S.c_str(),"(%f,%f + %f,%f)",&start.x,&start.y,&size.x,&size.y);
	char c;
	stringstream ss(S);
	ss >> c >> start.x >> c >> start.y >> c >> c >> c >> size.x >> c >> size.y >> c;
	vec2 end = start+size;
	return rect(start,end);
}
string toString(AABB aabb){
	return fstring("(%.3f,%.3f,%.3f + %.3f,%.3f,%.3f)",aabb.start.x,aabb.start.y,aabb.start.y,aabb.size.x,aabb.size.y,aabb.size.z);
}
template<>  AABB fromString<AABB>(const string S){
	vec3 start;
	vec3 size;
	//std::ignore = sscanf(S.c_str(),"(%f,%f,%f + %f,%f,%f)",&start.x,&start.y,&start.z,&size.x,&size.y,&size.z);
	char c;
	stringstream ss(S);
	ss >> c >> start.x >> c >> start.y >> c >> start.z
		>> c >> c >> c
		>> size.x >> c >> size.y >> c >> size.z >> c;
	vec3 end = start+size;
	return AABB(start,end);
}
string toString(mat4 M){
	return "<mat4>";
}
template<> mat4 fromString<mat4>(const string S){
	return mat4();
}
#include "camera.h"
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
string toString(void *p){
	char buff[80];
	snprintf(buff,79,"0x%p",p);
	return string(buff);
}
template<> void *fromString<void*>(const string S){
	void *p;
	//std::ignore = sscanf(S.c_str(),"0x%p",&p);
	stringstream ss(S);
	ss >> p;
	return p;
}
string toString(bool B){
	return itoa(B);
}
template<> bool fromString<bool>(const string S){
	return (bool)atoi(S.c_str());
}
string toString(int I){
	return itoa(I);
}
template<> int fromString<int>(const string S){
	int n;
	//std::ignore = sscanf(S.c_str(),"%d",&n);
	stringstream ss(S);
	ss >> n;
	return n;
}
string toString(float f){
	return ftoa(f);
}
template<> float fromString<float>(const string S){
	float f;
	//std::ignore = sscanf(S.c_str(),"%f",&f);
	stringstream ss(S);
	ss >> f;
	return f;
}
string toString(string S){
	return S;
}
template<> string fromString<string>(const string S){
	return S;
}
#include "texture.h"
#include "resource.h"
string toString(texture *t){
	if(!t){return fstring("tex:[null]");}
	return fstring("tex:[%s]",t->name.c_str());
}
template<> texture* fromString<texture*>(const string S){
	if(S == "tex:[null]"){return 0;}
	//char buff[80];
	//std::ignore = sscanf(S.c_str(),"tex:[%[^]]]",buff);
	char c;
	string name;
	stringstream ss(S);
	ss >> c >> c >> c >> c >> c >> name >> c;
	printf("fromString<texture*>: S = [%s]\n",S.c_str());
	printf("fromString<texture*>: buff = [%s]\n", name.c_str());//buff);
	//buff[79] = 0;
	//string name(buff);
	return getTexture(name);
}
#include "fonts.h"
string toString(font *f){
	if(!f){return fstring("font:[null]");}
	return fstring("font:[%s]",f->name.c_str());
}
template<> font* fromString<font*>(const string S){
	if(S == "font:[null]"){return 0;}
	//char buff[80];
	//std::ignore = sscanf(S.c_str(),"font:[%[^]]]",buff);
	string name;
	char c;
	stringstream ss(S);
	ss >> c >> c >> c >> c >> c >> c >> name >> c;
	printf("fromString<font*>: S = [%s]\n",S.c_str());
	printf("fromString<font*>: buff = [%s]\n", name.c_str());//buff);
	//buff[79] = 0;
	//string name(buff);
	return getFont(name);
}
#include "GUI_internal.h"
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
string operator+(const string S, const int N){return S+itoa(N);}
string operator+(const string S, const double N){return S+ftoa(N);}
string fstring(const char *format, ...){
	string str;
	printfify(format,str);
	return str;
}
void stringUtils_error(string S){
	//error(S.c_str());
	throw std::runtime_error(S);
}
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
	if (str2) { strcpy(str2, str); }
	if (!str2) {return 0;}
	char **arr = (char **)malloc(80);
	if (!arr) {return 0;}
	int arrI = 0;
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
					throw std::runtime_error("broken escape sequence\n"); //error("broken escape sequence\n");
					break;
				default:
					//error((string("unknown escape sequence [")+C+C2+"]\n").c_str());
				{
					stringstream ss;
					ss << "unknown escape sequence [" << C << C2 << "]\n";
					throw std::runtime_error(ss.str());
				}
					
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
	if (ptr) {strcpy(ptr, str);}
	return ptr;
}
void printString(string S){
	for(unsigned int I = 0; I < S.length();){
		string S2 = S.substr(I,80);
		printf("%s",S2.c_str());
		I += 80;
	}
}
