#include "display/paint.h"
////-#include "main/control.h"
#include "util/globals.h"

void paintInit(){
	setColor({0,0,0});
	setAlpha(255);
	paintTex = 0;
	paintP1 = (vec2i){0,0};
	paintP2 = (vec2i){0,0};
	paintP3 = (vec2i){0,0};
	paintScissorEnabled = false;
}

void paintLine(int X1, int Y1, int X2, int Y2){
	glBegin(GL_LINES);
	glVertex2i(X1, Y1);
	glVertex2i(X2, Y2);
	glEnd();
}

void paintLine(vec2i P1, vec2i P2){
	paintLine(P1.x, P1.y, P2.x, P2.y);
}
void paintLine(){
	paintLine(paintP1, paintP2);
}
void paintRect(int X1, int Y1, int X2, int Y2)
{
    glBegin(GL_QUADS);
    glVertex2i(X1, Y1);
    glVertex2i(X1, Y2);
    glVertex2i(X2, Y2);
    glVertex2i(X2, Y1);
    glEnd();
}
void paintRect(vec2i P1, vec2i P2){
	paintRect(P1.x, P1.y, P2.x, P2.y);
}
void paintRect(rect A){
	paintRect(A.getx(),A.gety(),A.getx2(),A.gety2());
}
void paintRect(){
	paintRect(paintP1, paintP2);
}
void paintTexturedRect(int X1, int Y1, int X2, int Y2, texture *tex)
{
	glColor3f(1.0f,1.0f,1.0f);
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, tex->t );
    glBegin(GL_QUADS);
	glTexCoord2f(0,0);
    glVertex2i(X1, Y1);
	glTexCoord2f(0,1);
    glVertex2i(X1, Y2);
	glTexCoord2f(1,1);
    glVertex2i(X2, Y2);
	glTexCoord2f(1,0);
    glVertex2i(X2, Y1);
    glEnd();	
	glDisable( GL_TEXTURE_2D );
}
void paintTexturedRect(rect A, texture *tex){
	paintTexturedRect(A.getx(),A.gety(),A.getx2(),A.gety2(),tex);
}
void paintTexturedRect(){
	paintTexturedRect(paintP1.x, paintP1.y, paintP2.x, paintP2.y, paintTex);
}
void paintCircle(int X1, int Y1, int Radius)
{
	glBegin(GL_POLYGON);
	for(double I = 0; I<36;I++)
	{
		glVertex2i(X1+Radius*sin(3.1415926535897932384626433832795*I/18), Y1+Radius*cos(3.1415926535897932384626433832795*I/18));
	}
	glEnd();
}
void paintCircleOutline(int X1, int Y1, int Radius)
{
	glBegin(GL_LINE_LOOP);
	for(double I = 0; I<36;I++)
	{
		glVertex2i(X1+Radius*sin(3.1415926535897932384626433832795*I/18), Y1+Radius*cos(3.1415926535897932384626433832795*I/18));
	}
	glEnd();
}
void paintRectOutline(int X1, int Y1, int X2, int Y2)
{
    float x1 = X1;// /width-1.0f;
    float x2 = X2;// /width-1.0f;
    float y1 = Y1;// /height-1.0f;
    float y2 = Y2;// /height-1.0f;
    glBegin(GL_LINES);

    glVertex2f(x1, y1);
    glVertex2f(x2, y1);

    glVertex2f(x2, y1);
    glVertex2f(x2, y2);

    glVertex2f(x2, y2);
    glVertex2f(x1-1, y2);


    glVertex2f(x1, y2);
    glVertex2f(x1, y1);

    glEnd();
}
void paintRectOutline(rect A){
	paintRectOutline(A.getx(),A.gety(),A.getx2(),A.gety2());
}
void paintRectOutline(){
	paintRectOutline(paintP1.x, paintP1.y, paintP2.x, paintP2.y);
}

void paintTriangle(int X1, int Y1, int X2, int Y2, int X3, int Y3){
	glBegin(GL_TRIANGLES);
	
	glVertex2i(X1,Y1);
	glVertex2i(X2,Y2);
	glVertex2i(X3,Y3);
	
	glEnd();
}
void paintTriangle(vec2i P1, vec2i P2, vec2i P3){
	paintTriangle(P1.x,P1.y,P2.x,P2.y,P3.x,P3.y);
}

color3i paintColor;
byte paintAlpha;
vec2i paintP1, paintP2, paintP3;
texture *paintTex;
rect paintScissor;
bool paintScissorEnabled;

void setColor(color3i color)
{
	paintColor = color;
	glColor4f(paintColor.r/255.0f,paintColor.g/255.0f,paintColor.b/255.0f,paintAlpha/255.0f);
}
void setAlpha(byte alpha)
{
	paintAlpha = alpha;
	glColor4f(paintColor.r/255.0f,paintColor.g/255.0f,paintColor.b/255.0f,paintAlpha/255.0f);
}
void setColorAlpha(color4i coloralpha){
	setColor((color3i){coloralpha.r, coloralpha.g, coloralpha.b});
	setAlpha(coloralpha.a);
}
void setScissor(rect scissor){ //zero scissor is treated as entire screen uncropped.
	rect screen = getScreenRect();
	if(scissor == rect()){
		scissor = screen;
	}
	paintScissor = scissor;
/* 	glLineWidth(3);
	glColor4f(1.0f,0,0,1.0f);
	float x1 = scissor.getx();
	float x2 = scissor.getx2();
	float y1 = scissor.gety();
	float y2 = scissor.gety2();
	glDisable(GL_SCISSOR_TEST);
	glBegin(GL_LINES);
	// []
	glVertex2f(x1, y1);
    glVertex2f(x2, y1);

    glVertex2f(x2, y1);
    glVertex2f(x2, y2);

    glVertex2f(x2, y2);
    glVertex2f(x1-1, y2);
	
	glVertex2f(x1, y2);
    glVertex2f(x1, y1);
	
	
	// X
	// glVertex2f(x1, y1);
	// glVertex2f(x2, y2);
	
	// glVertex2f(x2, y1);
	// glVertex2f(x1, y2);
	//
	glEnd();
	glEnable(GL_SCISSOR_TEST);
	setColor(paintColor);
	setAlpha(paintAlpha);
	glLineWidth(1); */
	glScissor(scissor.getx()-1, screen.geth()-scissor.gety2()-1, scissor.getw()+1, scissor.geth()+1);
	//printf("new scissor: %s\n",scissor.toString().c_str());
}
void setScissorEnabled(bool scissorEnabled){
	if(paintScissorEnabled != scissorEnabled){
		paintScissorEnabled = scissorEnabled;
		setScissor(rect());
	}
	if(scissorEnabled){
		glEnable(GL_SCISSOR_TEST);
	}else{
		glDisable(GL_SCISSOR_TEST);
	}
}

tempPaintSettings::tempPaintSettings(){color = paintColor; alpha = paintAlpha; P1 = paintP1; P2 = paintP2; P3 = paintP3; tex = paintTex; scissor = paintScissor; scissorEnabled = paintScissorEnabled;}
tempPaintSettings::~tempPaintSettings(){setColor(color); setAlpha(alpha); setP1(P1); setP2(P2); setP3(P3); setTexture(tex); setScissor(scissor); setScissorEnabled(scissorEnabled);}
tempPaintSettings &tempPaintSettings::setColor(color3i color){::setColor(color); return *this;}
tempPaintSettings &tempPaintSettings::setAlpha(byte alpha){::setAlpha(alpha); return *this;}
tempPaintSettings &tempPaintSettings::setP1(int X, int Y){paintP1 = (vec2i){X,Y}; return *this;}
tempPaintSettings &tempPaintSettings::setP1(vec2i P){paintP1 = P; return *this;}
tempPaintSettings &tempPaintSettings::setP2(int X, int Y){paintP2 = (vec2i){X,Y}; return *this;}
tempPaintSettings &tempPaintSettings::setP2(vec2i P){paintP2 = P; return *this;}
tempPaintSettings &tempPaintSettings::setP3(int X, int Y){paintP3 = (vec2i){X,Y}; return *this;}
tempPaintSettings &tempPaintSettings::setP3(vec2i P){paintP3 = P; return *this;}
tempPaintSettings &tempPaintSettings::setRect(rect R){paintP1 = R.getStart(); paintP2 = R.getEnd(); return *this;}
tempPaintSettings &tempPaintSettings::setTexture(texture *tex){paintTex = tex; return *this;}
tempPaintSettings &tempPaintSettings::setScissor(rect scissor){::setScissor(scissor); return *this;}
tempPaintSettings &tempPaintSettings::crop(rect scissor){::setScissor(getScissor().clamp(scissor)); return *this;}
tempPaintSettings &tempPaintSettings::setScissorEnabled(bool scissorEnabled){::setScissorEnabled(scissorEnabled);}
color3i tempPaintSettings::getColor(){return paintColor;}
byte tempPaintSettings::getAlpha(){return paintAlpha;}
vec2i tempPaintSettings::getP1(){return paintP1;}
vec2i tempPaintSettings::getP2(){return paintP2;}
vec2i tempPaintSettings::getP3(){return paintP3;}
texture *tempPaintSettings::getTexture(){return paintTex;}
rect tempPaintSettings::getScissor(){return paintScissor;}
bool getScissorEnabled(){return paintScissorEnabled;}





