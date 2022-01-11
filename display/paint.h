void paintRect(int X1, int Y1, int X2, int Y2);
void paintTexturedRect(int X1, int Y1, int X2, int Y2, GLuint tex);
void paintCircle(int X1, int Y1, int Radius);
void paintCircleOutline(int X1, int Y1, int Radius);
void paintRectOutline(int X1, int Y1, int X2, int Y2);
void setColor(color3i color);
void setAlpha(byte alpha);
extern color3i paintColor;
extern byte paintAlpha;