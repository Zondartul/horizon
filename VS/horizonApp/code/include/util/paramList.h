#ifndef PARAMLIST_GUARD
#define PARAMLIST_GUARD
#include "math/vec.h"
#include "util/stringUtils.h"
#include "util/debug.h" //21.01.2023: for error()
#include <string>
#include <typeinfo>
#include <map>
using std::map;
using std::string;

#define rp(x) readParam(PL,#x,x)
#define wp(x) writeParam(PL,#x,x)

struct param{
	string name;
	string type;
	string value;
};

typedef map<string,param> ParamList;

string toString(ParamList PL);

template<> Result<ParamList,zError> fromString<ParamList>(string S);

template<typename T> 
bool readParam(ParamList &PL, string pname, T& val)
{
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

template<typename T> 
void writeParam(ParamList &PL, string pname, const T& val)
{
	param P;
	P.name = pname;
	P.type = typeid(T).name();
	P.value = toString(val);
	PL[pname] = P;
}

#endif