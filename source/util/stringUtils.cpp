#include "stringutils.h"
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

#include "vec.h"
string toString(vec3 V){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f,%.3f)",V.x,V.y,V.z);
	return string(buff);
}

// string toString(vec3 V){
	// char buff[80];
	// snprintf(buff,79,"(%d,%.3d,%.3d)",V.r,V.g,V.b);
	// return string(buff);
// }

// string toString(vec2 V){
	// char buff[80];
	// snprintf(buff,79,"(%d,%d)",V.x,V.y);
	// return string(buff);
// }

string toString(vec2 V){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f)",V.x,V.y);
	return string(buff);
}

// string toString(rect R){
	// char buff[80];
	// snprintf(buff,79,"(%d,%d + %d,%d)",R.start.x,R.start.y,R.size.x,R.size.y);
	// return string(buff);
// }

string toString(rect R){
	char buff[80];
	snprintf(buff,79,"(%.3f,%.3f + %.3f,%.3f)",R.start.x,R.start.y,R.size.x,R.size.y);
	return string(buff);
}

string toString(void *p){
	char buff[80];
	snprintf(buff,79,"0x%p",p);
	return string(buff);
}

string toString(AABB aabb){
	return fstring("(%.3f,%.3f,%.3f + %.3f,%.3f,%.3f)",aabb.start.x,aabb.start.y,aabb.start.y,aabb.size.x,aabb.size.y,aabb.size.z);
}
string toString(bool B){return itoa(B);}
string toString(string S){return S;}
string toString(int I){return itoa(I);}
string toString(float f){return ftoa(f);}

//string toString(vec2 V){return toString(tovec2(V));}
//string toString(vec3 V){return toString(tovec3(V));}
string toString(mat4 M){return "<mat4>";}
string operator+(const string S, const int N){return S+itoa(N);}
string operator+(const string S, const double N){return S+ftoa(N);}

string fstring(const char *format, ...){
	string str;
	printfify(format,str);
	return str;
}

#include "camera.h"
string toString(camprojection cpj){return toString(cpj.MVP)+":"+toString(cpj.pos);}

template<> int fromString<int>(const string S){
	//printf("fromString<int>(%s)\n",S.c_str());
	int n;
	sscanf(S.c_str(),"%d",&n);
	return n;
}

template<> vec3 fromString<vec3>(const string S){
	//printf("fromString<vec3>(%s)\n",S.c_str());
	vec3 v;
	sscanf(S.c_str(),"(%f,%f,%f)",&v.x,&v.y,&v.z);
	return v;
}

template<> string fromString<string>(const string S){
	//printf("fromString<string>(%s)\n",S.c_str());
	return S;
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
	for(int I = 0; I < S.length(); I++){
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
	for(int I = 0; I < S.length(); I++){
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
