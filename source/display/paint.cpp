#include "display/paint.h"

void paintLine(int X1, int Y1, int X2, int Y2){
	glBegin(GL_LINES);
	glVertex2i(X1, Y1);
	glVertex2i(X2, Y2);
	glEnd();
}

void paintLine(vec2i P1, vec2i P2){
	paintLine(P1.x, P1.y, P2.x, P2.y);
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
void paintRect(rect A){
	paintRect(A.getx(),A.gety(),A.getx2(),A.gety2());
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

color3i paintColor;
byte paintAlpha;

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