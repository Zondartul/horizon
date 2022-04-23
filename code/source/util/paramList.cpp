//#include "debug.h"
#include "globals.h"
#include "paramList.h"

string toString(ParamList PL){
	string S;
	for(auto I = PL.begin(); I != PL.end(); I++){
		param P = I->second;
		S = S+"<"+P.name+"><"+P.type+"><"+P.value+">\n"; //needs escaping
	}
	return S;
}

template<> ParamList fromString<ParamList>(string S){
	ParamList PL;
	while(true){
		char buff1[80];
		char buff2[80];
		char buff3[80];
		int len;
		int num = sscanf(S.c_str(),"<%[^>]><%[^>]><%[^>]>\n%n",
			&buff1[0],&buff2[0],&buff3[0],&len);
		if(num!=3){break;}
		string S1(buff1),S2(buff2),S3(buff3);
		S = string(S.c_str()+len);
		PL[S1] = {S1,S2,S3};
	}
	return PL;
}


//------------------------- PARAMETER LIST EXPERIMENT --------------------------
//TODO: move "fromString" to stringUtils, and maybe disambiguate between
//"pretty printing" and "serialization" versions.

/*
class bar{
	public:
	int a;
	vec3 dir;
	string text;
	void setParamList(ParamList PL){
		rp(a); rp(dir); rp(text);
	}
	ParamList getParamList(){
		ParamList PL;
		wp(a); wp(dir); wp(text);
		return PL;
	}
};

class bar2{
	public:
	float a;
	vec3 dir;
	string text;
	void setParamList(ParamList PL){
		rp(a); rp(dir); rp(text);
	}
	ParamList getParamList(){
		ParamList PL;
		wp(a); wp(dir); wp(text);
		return PL;
	}
};

void parameterExperiment(){
	printf("------------- PARAMETER LIST EXPERIMENT --------------\n");
	bar b,c;
	b.a = 1;
	b.dir = vec3(1,2,3);
	b.text = "hello";

	c.a = 3;
	c.dir = vec3(0,0,1);
	c.text = "bai";

	ParamList PL;
	PL = c.getParamList();

	string S = toString(PL);
	printf("ParamList: \n[%s]\n",S.c_str());
	PL = fromString<ParamList>(S);

	printf("before: b.a = [%s], b.dir = [%s], b.text = [%s]\n",
		toCString(b.a),toCString(b.dir),toCString(b.text));

	b.setParamList(PL);

	printf("after: b.a = [%s], b.dir = [%s], b.text = [%s]\n",
		toCString(b.a),toCString(b.dir),toCString(b.text));

	//bar2 d;
	//d.setParamList(PL);

	printf("------------- END PARAMETER LIST EXPERIMENT --------------\n");
}
*/
//--------- END PARAMETER LIST EXPERIMENT --------

