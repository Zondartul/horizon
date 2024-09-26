#include "util/stringUtils.h"
#include "util/globals_util.h"
#include "math/vec.h"
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <sstream>
using std::runtime_error;
using std::stringstream;

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
#include "math/vec.h"
//----------------------------------------------- vec3
string toString(vec3 V){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f,%.3f)",V.x,V.y,V.z);
	return string(buff);
}
template<> Result<vec3,zError> fromString<vec3>(const string S){
	vec3 v;
	//std::ignore = sscanf(S.c_str(),"(%f,%f,%f)",&v.x,&v.y,&v.z);
	char c;
	stringstream ss(S);
	if(ss >> c >> v.x >> c >> v.y >> c >> v.z >> c){
		return v;
	}else{
		return zError("can't parse vec3 from string");
	}
}
string toString(vec2 V){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f)",V.x,V.y);
	return string(buff);
}
template<> Result<vec2,zError> fromString<vec2>(const string S){
	vec2 v;
	//std::ignore = sscanf(S.c_str(),"(%f,%f)",&v.x,&v.y);
	char c;
	stringstream ss(S);
	if(ss >> c >> v.x >> c >> v.y >> c){
		return v;
	}else{
		return zError("can't parse vec2 from string");
	}
}
string toString(rect R){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f + %.3f,%.3f)",R.start.x,R.start.y,R.size.x,R.size.y);
	return string(buff);
}
template<> Result<rect,zError> fromString<rect>(const string S){
	vec2 start;
	vec2 size;
	//std::ignore = sscanf(S.c_str(),"(%f,%f + %f,%f)",&start.x,&start.y,&size.x,&size.y);
	char c;
	stringstream ss(S);
	/// so something funny about this... seems to eat the first two characters of the size.x
	/// maybe it's because stream extraction operator ignores spaces by default.
	//ss >> c >> start.x >> c >> start.y >> c >> c >> c >> size.x >> c >> size.y >> c;
	if(ss >> c >> start.x >> c >> start.y >> c >> size.x >> c >> size.y >> c){
		vec2 end = start+size;
		return rect(start,end);
	}else{
		return zError("can't parse rect from string");
	}
}
string toString(AABB aabb){
	return fstring("(%.3f,%.3f,%.3f + %.3f,%.3f,%.3f)",aabb.start.x,aabb.start.y,aabb.start.y,aabb.size.x,aabb.size.y,aabb.size.z);
}
template<>  Result<AABB,zError> fromString<AABB>(const string S){
	vec3 start;
	vec3 size;
	//std::ignore = sscanf(S.c_str(),"(%f,%f,%f + %f,%f,%f)",&start.x,&start.y,&start.z,&size.x,&size.y,&size.z);
	char c;
	stringstream ss(S);
	if(
	ss >> c >> start.x >> c >> start.y >> c >> start.z
		>> c >> c >> c
		>> size.x >> c >> size.y >> c >> size.z >> c
	){
		vec3 end = start+size;
		return AABB(start,end);
	}else{
		return zError("can't parse AABB from string");
	}
}
string toString(mat4 M){
	return "<mat4>";
}
template<> Result<mat4,zError> fromString<mat4>(const string S){
	return zError("unimplemented");
}



string toString(void *p){
	char buff[80];
	snprintf(buff,79,"0x%p",p);
	return string(buff);
}
template<> Result<void*,zError> fromString<void*>(const string S){
	void *p;
	//std::ignore = sscanf(S.c_str(),"0x%p",&p);
	stringstream ss(S);
	if(ss >> p){
		return p;
	}else{
		zError("can't parse void* from string");
	}
}
string toString(bool B){
	return itoa(B);
}
template<> Result<bool,zError> fromString<bool>(const string S){
	return (bool)atoi(S.c_str());
}
string toString(int I){
	return itoa(I);
}
template<> Result<int,zError> fromString<int>(const string S){
	int n;
	//std::ignore = sscanf(S.c_str(),"%d",&n);
	stringstream ss(S);
	if(ss >> n){
		return n;
	}else{
		return zError("can't parse int from string");
	}
}
string toString(float f){
	return ftoa(f);
}
template<> Result<float,zError> fromString<float>(const string S){
	float f;
	//std::ignore = sscanf(S.c_str(),"%f",&f);
	stringstream ss(S);
	if(ss >> f){
		return f;
	}else{
		return zError("can't parse float from string");
	}
}
string toString(string S){
	return S;
}
template<> Result<string,zError> fromString<string>(const string S){
	return S;
}

//#include "resource/texture.h"
//#include "resource/resource.h"

//#include "resource/fonts.h"
//#include "GUI_internal.h"
//string toString(GUIe_alignment al){
//	switch(al){
//		case GUIa::None: return "ALIGN_NONE";
//		case GUIa::Left: return "ALIGN_LEFT";
//		case GUIa::Right: return "ALIGN_RIGHT";
//		case GUIa::Top: return "ALIGN_TOP";
//		case GUIa::Bottom: return "ALIGN_BOTTOM";
//		case GUIa::Center: return "ALIGN_CENTER";
//		default: printf("toString(GUI_alignment): unforseen switch-case\n"); return "ALIGN_NONE";
//	}
//}
//template<> GUIe_alignment fromString<GUIe_alignment>(const string S){
//	if(S == "ALIGN_NONE"){return GUIa::None;}
//	if(S == "ALIGN_LEFT"){return GUIa::Left;}
//	if(S == "ALIGN_RIGHT"){return GUIa::Right;}
//	if(S == "ALIGN_TOP"){return GUIa::Top;}
//	if(S == "ALIGN_BOTTOM"){return GUIa::Bottom;}
//	if(S == "ALIGN_CENTER"){return GUIa::Center;}
//	printf("fromString<alignmentKind>: unforseen case\n");
//	return GUIa::None;
//}
string operator+(const string S, const int N){return S+itoa(N);}
string operator+(const string S, const double N){return S+ftoa(N);}

string fstring(const char *format, ...){
	string str;
	printfify(format,str);
	return str;
}



void stringUtils_error(string S){
	throw runtime_error(S);
	//error(S.c_str());
}

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
	if (!str2) {return 0;}
	strcpy(str2,str);

	char **arr = (char **)malloc(80);
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
					//error("broken escape sequence\n");
					throw runtime_error("broken escape sequence");
					break;
				default:
					//error((string("unknown escape sequence [")+C+C2+"]\n").c_str());
					stringstream ss;
					ss << "unknown escape sequence [" << C << C2 << "]";
					throw runtime_error(ss.str());
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
	if (!ptr) {return 0;}
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
