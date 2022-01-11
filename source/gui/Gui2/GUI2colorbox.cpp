#include "gui/Gui2/GUI2colorbox.h"
#include "main/control.h"
#include "display/paint.h"
GUI2colorbox::GUI2colorbox():GUI2base()
{
	movable = false;
	resizible = false;
	size = {256,256};
	colLU = {0,255,255};//hue,saturation,value
	colRU = {360,255,255};
	colLD = {0,0,255};
	colRD = {360,0,255};
	colorHSV = {0,0,0};
	colorRGB = {0,0,0};
	cursor = {-1,-1};
}
void GUI2colorbox::onClick(int mb)
{
	if(mb>0)
	{
		cursor = mouseP-pos;
		colorHSV = (colLU*(size.x-cursor.x)/size.x+colRU*(cursor.x)/size.x)*(size.y-cursor.y)/size.y +
					(colLD*(size.x-cursor.x)/size.x+colRD*(cursor.x)/size.x)*(cursor.y)/size.y;
		colorRGB = HSVtoRGB(colorHSV);
	}
}
void GUI2colorbox::render(void *arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	setColor(color_border);
	paintRectOutline(pos.x-1,pos.y-1,pos.x+size.x+1,pos.y+size.y+1);
	
	glBegin(GL_LINES);
	int X = pos.x;
	float E = size.x;
	int Yi = pos.y;
	int Yf = pos.y+size.y;
	color3i col = {0,0,0};
	for(float I = 0; I<E;I++)
	{
		col = HSVtoRGB((colLU*(E-I))/E+(colRU*I)/E);
		glColor3f(col.r/255.0f,col.g/255.0f,col.b/255.0f);
		glVertex2i(X, Yi);
		col = HSVtoRGB((colLD*(E-I))/E+(colRD*I)/E);
		//col = HSVtoRGB({(int)(360*I/E),255,255});
		glColor3f(col.r/255.0f,col.g/255.0f,col.b/255.0f);
		glVertex2i(X, Yf);
		X++;
	}
	glEnd();
	
	if(cursor.x>=0)
	{
		//setColor(colorRGB);
		//paintRectOutline(pos.x+cursor.x-1,pos.y+cursor.y-1,pos.x+cursor.x+1,pos.y+cursor.y+1);
		
		setColor(color_border);
		paintRectOutline(pos.x+cursor.x-2,pos.y+cursor.y-2,pos.x+cursor.x+2,pos.y+cursor.y+2);
	}
	//glDisable(GL_SCISSOR_TEST);
}