#include "gui/Gui2/GUI2button.h"
#include "resource/textureloader.h"
#include "display/paint.h"
#include "resource/fonts.h"
GUI2button::GUI2button():GUI2base()
{
	func = NULL;
	arg = NULL;
	text = "";
	image = 0;
}
void GUI2button::onClick(int mb)
{
	if((mb==0)&&mouseOver)
	{
		GUI2base::onClick(mb);
		if(func)(func(arg));
		printf("[%s]\n",toString(this).c_str());
	}
}
void GUI2button::setImage(string path)
{
	//ImageTex = LoadTextureRAW(path, 1);
	image = getTextureHandle(path);//LoadTexture(path.c_str()).t;//GenTextureBMP(path).t;
}
void GUI2button::render(void *arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	
	setColor(color_panel);
	if(mouseOver){setAlpha(255);}else{setAlpha(64);}
	if(image==0){paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);}
	else{paintTexturedRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y,image);}
	setAlpha(255);
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setColor(color_text);
	printw(pos.x+2,pos.y-4, size.x, -1,text);
	
	//glDisable(GL_SCISSOR_TEST);
}
