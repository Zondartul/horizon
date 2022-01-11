#define EXCEPT_SUPPRESS //lol, have to suppress the macro in this particular
//translation unit, because that's where the REAL except is actually defined
#include "util/globals.h"

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


vec2i operator + (vec2i A, vec2i B)
{
	return {A.x+B.x, A.y+B.y};
}
vec2i operator - (vec2i A, vec2i B)
{
	return {A.x-B.x, A.y-B.y};
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

rect &rect::setx(int x){this->x = x; x2 = x+w; return *this;}
rect &rect::sety(int y){this->y = y; y2 = y+h; return *this;}
rect &rect::setw(int w){this->w = w; x2 = x+w; return *this;}
rect &rect::seth(int h){this->h = h; y2 = y+h; return *this;}
rect &rect::setx2(int x2){this->x2 = x2; w = x2-x; return *this;}
rect &rect::sety2(int y2){this->y2 = y2; h = y2-y; return *this;}
bool rect::contains(vec2i A){return (A.x >= x) && (A.x <= x2) && (A.y >= y) && (A.y <= y2);}
bool rect::contains(vec4i A){return contains((vec2i){A.x1,A.y1}) && contains((vec2i){A.x2,A.y2});}
bool rect::contains(rect A){return contains((vec2i){A.x,A.y}) && contains((vec2i){A.x2,A.y2});}
vec2i rect::clamp(vec2i A){return {::clamp(A.x, x, x2),::clamp(A.y,y,y2)};} //nothing::something means "find something in global scope"
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