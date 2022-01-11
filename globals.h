#include <string>
#include <sstream>
#include <iostream>
#include <cstdarg>
#include <vector>
#include <list>
#include <map>

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#define listForwardI iterator I = subscribers.start(), E = subscribers.end(); I!=E;I++
#define BRDB 4
// border-border
#define BRDT 36
// border-title
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

struct vec2i
{
    int x;
    int y;
};
struct vec2f
{
    double x;
    double y;
};
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

struct color3i
{
	int r;
	int g;
	int b;
};

color3i operator + (color3i A, color3i B)
{
	return {clamp(A.r+B.r,0,255),clamp(A.g+B.g,0,255),clamp(A.b+B.b,0,255)};
}

struct vec3i
{
	operator color3i();
    int x;
    int y;
    int z;
};
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
struct vec4i
{
	int x1;
	int y1;
	int x2;
	int y2;
	bool contains(vec2i A)
	{
		return  (A.x >= x1) && (A.x <= x2) &&
				(A.y >= y1) && (A.y <= y2);
	}
};
bool operator == (vec4i A, vec4i B)
{
	return ((A.x1==B.x1)&&(A.y1==B.y1)&&(A.x2==B.x2)&&(A.y2==B.y2));
}

vec3i::operator color3i()
{
	return {clamp(x,0,255),clamp(y,0,255),clamp(z,0,255)};
}

struct listNode
{
    listNode* next;
    void* thing;
};

void* Tahoma8;//no aa
void* Tahoma12;//no aa
void* Tahoma18;
void* Tahoma20;
void* Tahoma22;
void* Calibri8;
void* Calibri12;
void* Calibri18;
void* Calibri20;
void* Calibri22;
void* CourierNew8;
void* CourierNew12;
void* CourierNew18;
void* CourierNew20;
void* CourierNew22;

struct color4i
{
	int r;
	int g;
	int b;
	int a;
};


int ms = 0;
int freq = 66;
float theta;
int counter = 0;
float width = 1024.0f;
float height = 640.0f;
color4i bground;
vec2i mousePos;

typedef void (*funcptr)(void *arg); // let "funcptr" be the "pointer to a void-returning funtion that takes
									                      // a pointer-to-void" type.

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

template<class T> class ptrSniper // I'm in way over my head.
{
	public:
	vector<T**> pointers;
	T* upperThis;
	void get(T** ptr) // function takes adress of the pointer. Pointer's adress saved, pointer assigned to object.
	{
		for(int i=0;i<pointers.size();i++)
		{
			if(pointers[i]==ptr)
			{
				if(*ptr==upperThis)
				{printf("ptrSniper: ptr already defined\n");return;}
				else
				{printf("ptrSniper: refreshing ptr\n");*ptr = upperThis; return;}
			}
		}
		pointers.push_back(ptr);
		*ptr = upperThis;
		printf("ptrSniper: new ptr protected\n");
	}
	~ptrSniper()
	{
		printf("ptrSniper: sniping %d pointers\n", pointers.size());
		for(int i = 0; i<pointers.size(); i++)
		{
			if(*(pointers[i])==upperThis){*(pointers[i])=0;} //if no longer pointing here, do nothing.
		}
	}
};

template <typename T> int sgn(T val) 
{
    return (T(0) < val) - (val < T(0));
}

double random(double min, double max)
{
	return min+((double)rand()/RAND_MAX)*(max-min);
}

template <typename T> string toString(T *val)
{
	char C[127];
	sprintf(C, "%s_@_%p", typeid(T).name(), val);
	return string(C);
}

template <typename T> T bits(T val, int start, int end)
{
	return val & ((unsigned long long)(pow(2,end+1)-1) >> start); 
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