#include "display/paint.h"

void paintRect(int X1, int Y1, int X2, int Y2)
{
    glBegin(GL_QUADS);
    glVertex2i(X1, Y1);
    glVertex2i(X1, Y2);
    glVertex2i(X2, Y2);
    glVertex2i(X2, Y1);
    glEnd();
}
void paintTexturedRect(int X1, int Y1, int X2, int Y2, GLuint tex)
{
	glColor3f(1.0f,1.0f,1.0f);
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, tex );
    glBegin(GL_QUADS);
	glTexCoord2f(0,1);
    glVertex2i(X1, Y1);
	glTexCoord2f(0,0);
    glVertex2i(X1, Y2);
	glTexCoord2f(1,0);
    glVertex2i(X2, Y2);
	glTexCoord2f(1,1);
    glVertex2i(X2, Y1);
    glEnd();	
	glDisable( GL_TEXTURE_2D );
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