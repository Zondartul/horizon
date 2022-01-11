#include "gui/Gui2/GUI2image.h"
#include "display/paint.h"

GUI2image::GUI2image():GUI2base()
{
	movable = false;
	resizible = false;
	size = {256,256};
	ImageTex = 0;
}
void GUI2image::setImage(char *path)
{
	//ImageTex = LoadTextureRAW(path, 1);
	ImageTex = getTextureHandle(path);//LoadTexture(path).t;//GenTextureBMP(path).t;
}
void GUI2image::render(void *arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x+1,pos.y+size.y+1);
	
	//draw

	paintTexturedRect(pos.x,pos.y+1,pos.x+size.x,pos.y+size.y+1, ImageTex);
	//int X = pos.x;
	//float E = size.x;
	//int Yi = pos.y;
	//int Yf = pos.y+size.y;
	//vec3i col = {0,0,0};
	
	glDisable(GL_SCISSOR_TEST);
}