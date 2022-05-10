#ifndef FANCYMATH_GUARD
#define FANCYMATH_GUARD
#include "simplemath.h"
#include <vector>
#include <utility>
using std::vector;
using std::pair;
#include "vec.h"
#include "globals.h"
#include "math.h"
void pack2D(vector<vec2> sizes,
			vec2 binsize,
			bool sort,
			vector<vec2> *results,
			vector<int> *packed,
			int *heightNeeded);
vec2 pack2DfindClosestPOT(vector<vec2> sizes, bool sort);
template<typename VT> VT interpolate_nearest(vector<pair<VT,float>> scatter, float pos){
	if (scatter.size() == 0){error("nothing to interpolate");}
	if (scatter.size() == 1){return scatter.front().first;}
	if (pos < scatter.front().second){return scatter.front().first;}
	if (pos > scatter.back().second){return scatter.back().first;}
	for(auto I = scatter.begin(); I != scatter.end()-1; I++){
		if(pos > (I+1)->second){continue;}
		if(std::abs(pos-I->second) > std::abs(pos-(I+1)->second)){return I->first;}else{return (I+1)->first;}
	}
	error("unreachable code");
	return scatter.front().first;
}
template<typename VT> VT interpolate_linear(vector<pair<VT,float>> scatter, float pos){
	if (scatter.size() == 0){error("nothing to interpolate");}
	if (scatter.size() == 1){return scatter.front().first;}
	if (pos < scatter.front().second){return scatter.front().first;}
	if (pos > scatter.back().second){return scatter.back().first;}
	for(auto I = scatter.begin(); I != scatter.end()-1; I++){
		if(pos > (I+1)->second){continue;}	
		VT val1 = I->first;					
		VT val2 = (I+1)->first;
		float pos1 = I->second;
		float pos2 = (I+1)->second;
		float k = (pos - pos2)/(pos2-pos1);
		return val1*(1.f-k)+val2*k;
	}
	error("unreachable code");
	return scatter.front().first;
}
template<typename VT> VT interpolate_cubic(vector<pair<VT,float>> scatter, float pos){
	if (scatter.size() == 0){error("nothing to interpolate");}
	if (scatter.size() == 1){return scatter.front().first;}
	if (pos < scatter.front().second){return scatter.front().first;}
	if (pos > scatter.back().second){return scatter.back().first;}
	for(auto I = scatter.begin(); I != scatter.end()-1; I++){
		if(pos > (I+1)->second){continue;}
		VT  p0,p1,m0,m1,m01;
		float x0,x1,t;
		p0 = I->first;
		x0 = I->second;
		p1 = (I+1)->first;
		x1 = (I+1)->second;
		m01 = (p1-p0)/(x1-x0);
		m0 = m1 = m01;
		if(!(I == scatter.begin())){m0 = (p0-(I-1)->first)/(x0-(I-1)->second);}
		if(!((I+1) == scatter.end()-1)){m1 = ((I+2)->first-p1)/((I+2)->second-x1);}
		t = (pos-x0)/(x1-x0);
		float h00 = 2*t*t*t-3*t*t+1;
		float h10 = t*t*t-2*t*t+t;
		float h01 = -2*t*t*t+3*t*t;
		float h11 = t*t*t-t*t;
		return h00*p0+h10*m0+h01*p1+h11*m1;
	}
	error("unreachable code");
	return scatter.front().first;
}
#endif
