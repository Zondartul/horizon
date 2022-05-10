
#include "globals.h"
#include "paramList.h"

string toString(ParamList PL){
	string S;
	for(auto I = PL.begin(); I != PL.end(); I++){
		param P = I->second;
		S = S+"<"+P.name+"><"+P.type+"><"+P.value+">\n"; 
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









