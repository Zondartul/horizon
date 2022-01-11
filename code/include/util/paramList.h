#ifndef PARAMLIST_GUARD
#define PARAMLIST_GUARD

#include "vec.h"
#include <map>
using std::map;
#include <string>
using std::string;
#include "stringUtils.h"
#include <typeinfo>

#define rp(x) readParam(PL,#x,x)
#define wp(x) writeParam(PL,#x,x)

struct param{
	string name;
	string type;
	string value;
};

typedef map<string,param> ParamList;
string toString(ParamList PL);

template<> ParamList fromString<ParamList>(string S);

template<typename T> bool readParam(ParamList &PL, string pname, T& val){
	//printf("readParam(...,%s,...)\n",pname.c_str());
	if(PL.count(pname)){
		param P = PL[pname];
		string type = typeid(T).name();
		if(P.type != type){
			error("readParam error: param [%s] expects [%s] but got [%s]\n",
			pname.c_str(),type.c_str(),P.type.c_str());
		}
		val = fromString<T>(PL[pname].value); 
		return true;
	}
	else{return false;}
}
template<typename T> void writeParam(ParamList &PL, string pname, const T& val){
	//printf("writeParam(...,%s,...)\n",pname.c_str());
	param P;
	P.name = pname;
	P.type = typeid(T).name();
	P.value = toString(val);
	PL[pname] = P;
}

#endif