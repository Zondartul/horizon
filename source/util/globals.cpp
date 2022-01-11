#define EXCEPT_SUPPRESS //lol, have to suppress the macro in this particular
//translation unit, because that's where the REAL except is actually defined
#include "util/globals.h"
#include "assert.h"

using namespace std;

int clamp(int A, int B, int C)
{
	if(A<B)
	{
		return B;
	}
	else
	{
		if(A>C){return C;}
		else{return A;}
	}
}

vec2i clamp(vec2i A, vec2i B, vec2i C){
	return {clamp(A.x,B.x,C.x),clamp(A.y,B.y,C.y)};
}
vec2i operator + (vec2i A, vec2i B)
{
	return {A.x+B.x, A.y+B.y};
}
vec2i operator - (vec2i A, vec2i B)
{
	return {A.x-B.x, A.y-B.y};
}
vec2i operator - (vec2i A){
	return {-A.x,-A.y};
}
bool operator == (vec2i A, vec2i B)
{
	return ((A.x==B.x)&&(A.y==B.y));
}
bool operator != (vec2i A, vec2i B)
{
	return !(A==B);
}
vec2i operator / (vec2i A, int B)
{
	return {A.x/B,A.y/B};
}
vec2i operator * (vec2i A, int B)
{
	return {A.x*B,A.y*B};
}


color3i operator + (color3i A, color3i B)
{
	return {clamp(A.r+B.r,0,255),clamp(A.g+B.g,0,255),clamp(A.b+B.b,0,255)};
}


vec3i operator + (vec3i A, vec3i B)
{
	return {A.x+B.x, A.y+B.y, A.z+B.z};
}
vec3i operator * (vec3i A, int B)
{
	return {A.x*B,A.y*B,A.z*B};
}
vec3i operator / (vec3i A, int B)
{
	return {A.x/B,A.y/B,A.z/B};
}

bool vec4i::contains(vec2i A)
{
	return  (A.x >= x1) && (A.x <= x2) &&
			(A.y >= y1) && (A.y <= y2);
}
bool operator == (vec4i A, vec4i B)
{
	return ((A.x1==B.x1)&&(A.y1==B.y1)&&(A.x2==B.x2)&&(A.y2==B.y2));
}

rect &rect::setStart(vec2i A){setx(A.x);sety(A.y);}
rect &rect::setEnd(vec2i A){setx2(A.x);sety2(A.y);}
rect &rect::setSize(vec2i A){setw(A.x);seth(A.y);}
rect &rect::move(vec2i A){setStart(start+A); setEnd(end+A);}
rect &rect::setx(int x){start.x = x; size.x = end.x-start.x;}// return *this;}
rect &rect::sety(int y){start.y = y; size.y = end.y-start.y;}
rect &rect::setw(int w){size.x = w; end.x = start.x+size.x;}
rect &rect::seth(int h){size.y = h; end.y = start.y+size.y;}
rect &rect::setx2(int x2){end.x = x2; size.x = end.x-start.x;}
rect &rect::sety2(int y2){end.y = y2; size.y = end.y-start.y;}
bool rect::contains(vec2i A){return (A.x >= start.x) && (A.x <= end.x) && (A.y >= start.y) && (A.y <= end.y);}
bool rect::contains(vec4i A){return contains((vec2i){A.x1,A.y1}) && contains((vec2i){A.x2,A.y2});}
bool rect::contains(rect A){return contains(A.start) && contains(A.end);}
vec2i rect::clamp(vec2i A){return {::clamp(A.x, start.x, end.x),::clamp(A.y,start.y,end.y)};} //nothing::something means "find something in global scope"
rect rect::clamp(rect A){
	return A.setStart(clamp(A.start)).setEnd(clamp(A.end));
}
rect rect::insert(rect A){
	vec2i diff = {0,0};
	if(A.start.x < start.x){diff.x = start.x - A.start.x;}
	else if(end.x < A.end.x){diff.x = end.x - A.end.x;}
	if(A.start.y < start.y){diff.y = start.y - A.start.y;}
	else if(end.y < A.end.y){diff.y = end.y - A.end.y;}
	return A.move(diff);
}
rect rect::toParent(rect A){	//A is in local, need it in parent's coords.
	return A.setStart(toParent(A.start)).setEnd(toParent(A.end));
}
rect rect::toParent(){			//need this in parent's coords.
	//rect A;
	//A.setStart({0,0}).setSize(size);
	//return toParent(A);
	return *this; //... it's already in parent's coords.
}
rect rect::toWorld(rect A){		//A is in local, need it in world coords.
	A.setStart(toWorld(A.start)).setEnd(toWorld(A.end));
	return A;
}
rect rect::toWorld(){			//need this rect in world coords.
	//rect A;
	//A.setStart({0,0}).setSize(size);
	//return toWorld(A);
	if(parent){
		return parent->toWorld(*this); //if has a parent, then we're in parent's coords.
	}else{
		return *this;	//else we're in world coords.
	}
}
rect rect::fromParent(rect A){	//A is in parent's coords, need in local coords.
	return A.setStart(fromParent(A.start)).setEnd(fromParent(A.end));
}
rect rect::fromParent(){	//need this rect in it's own coords?...
	rect A;
	A.setStart({0,0}).setSize(size);
	//return fromParent(A);
	return A;
}
rect rect::fromWorld(rect A){	//A is in world coords, need it in local.
	A.setStart(fromWorld(A.start)).setEnd(fromWorld(A.end));
}
rect rect::fromWorld(){	//uh
	//rect A;
	//A.setStart({0,0}).setSize(size);
	//return fromWorld(A);
	assert(!"dafuq does this even mean");
}
vec2i rect::toParent(vec2i A){
	return {A.x+start.x, A.y+start.y};
}
vec2i rect::toWorld(vec2i A){
	if(parent){
		return parent->toWorld(toParent(A));
	}else{
		return toParent(A);
	}
}
vec2i rect::fromParent(vec2i A){
	return {A.x-start.x, A.y-start.y};
}
vec2i rect::fromWorld(vec2i A){
	if(parent){
		return fromParent(parent->fromWorld(A));
	}else{
		return fromParent(A);
	}
}

vec3i::operator color3i()
{
	return {clamp(x,0,255),clamp(y,0,255),clamp(z,0,255)};
}




int ms = 0;
int freq = 66;
float theta;
int counter = 0;
float width = 1024.0f;
float height = 640.0f;
color4i bground;
vec2i mousePos;
rect screen;
double d2r(double x){return x*M_PI/180.0;}
double r2d(double x){return x*180.0/M_PI;}


unsigned int explode(const string &txt, vector<string> &strs, char ch)
{
    unsigned int pos = txt.find( ch );
    unsigned int initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}

string itoa(double I)
{
	stringstream str;
	str.str("");
	str<<I;
	return str.str();
}

//atoi already there



double random(double min, double max)
{
	return min+((double)rand()/RAND_MAX)*(max-min);
}

int randint(int min, int max)
{
	return (int)(min+((double)rand()/RAND_MAX)*(max-min));
}



std::string operator+(std::string const &a, int b){
  std::ostringstream oss;
  oss<<a<<b;
  return oss.str();
}

std::string operator+(std::string const &a, double b){
  std::ostringstream oss;
  oss<<a<<b;
  return oss.str();
}

string tolower(string A){
	string B = A;
	for(int I = 0; I<A.length();I++){
		B[I] = tolower(A[I]);
	}
	return B;
}
string toupper(string A){
	string B = A;
	for(int I = 0; I<A.length();I++){
		B[I] = toupper(A[I]);
	}
	return B;
}

except::except(const char *a, int b, const char *str){
	mystr = (string(str)+" ["+string(a)+", line "+b+"]").c_str();
}
const char* except::what() const throw()
{
	return mystr;
}

/*
//dafuk is this
#define TL_DR_1 typename vector< pair<size_t, vector<pair<K,V>>> >::iterator
#define TL_DR_2 typename vector<pair<K,V>>::iterator
class hashtable
{ //lol wut
	vector< pair<size_t, vector<pair<void *,void *>>> > buckets;
	//copies a key-object pair
	template <typename K, typename V> void set(K k, V v)
	{
		std::hash<K> hasher;
		size_t myHash = hasher(k);
		for(TL_DR_1 I = buckets.begin(); I != buckets.end(); I++)
		{
			if(I.first == myHash)
			{
				for(TL_DR_2 T = I.second.begin(); T != I.second.end(); T++)
				{
					if(*((K*)T.first) == k)
					{
						*T.second = v;
						return;
					}
				}
				I.push_back(pair<void *, void*>(k,v));
			}
		}
		buckets.push_back(vector<pair<K,V>>((k,v)));
	}
	template <typename K, typename V> V get(K k)
	{
		std::hash<K> hasher;
		size_t myHash = hasher(k);
		for(TL_DR_1 I = buckets.begin(); I != buckets.end(); I++)
		{
			if(I.first == myHash)
			{
				for(TL_DR_2 T = I.second.begin(); T != I.second.end(); T++)
				{
					if(*((K*)T.first) == k)
					{
						return T.second;
					}
				}
			}
		}
		return NULL;
	}
};
I don't need your shitty metatables
especially if they don't work
*/