#include <string>
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
vec2i operator / (vec2i A, int B)
{
	return {A.x/B,A.y/B};
}
vec2i operator * (vec2i A, int B)
{
	return {A.x*B,A.y*B};
}

struct vec3i
{
    int x;
    int y;
    int z;
};
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
float width = 640.0f;
float height = 640.0f;
color4i bground;
vec2i mousePos;