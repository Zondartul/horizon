#include "modules/display/paint.h"
#include "GL/gl.h"
#include "colors.h"

void clearScreen(){
	vec3ub color = skyblue;
	glClearColor(color.r/255.0,color.g/255.0,color.b/255.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void setViewport(rect viewport){
	glViewport(viewport.start.x,viewport.start.y,viewport.size.x,viewport.start.y);
}

// #define white		{255,255,255}
// #define lightgray	{196,196,196}
// #define	gray		{128,128,128}
// #define darkgray	{64,64,64}
// #define black		{0,0,0}
// #define crimson		{128,0,0}
// #define red			{255,0,0}
// #define orange		{255,128,0}
// #define yellow		{255,255,0}
// #define lime		{196,255,0}
// #define green		{0,255,0}
// #define forestgreen	{0,128,64}
// #define cyan		{0,255,255}
// #define skyblue		{135,206,235}
// #define blue		{0,0,255}
// #define midnightblue	{0,64,128}
// #define pink		{255,196,196}
// #define violet		{128,0,255}
// #define purple		{196,0,255}