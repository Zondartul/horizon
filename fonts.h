struct glyphkind
{
    int bearingX;
    int bearingY;
    int sizeX;
    int sizeY;
    int advance;
    GLuint texture;
};
glyphkind* GenerateFont(const char* filepath, int size,bool aa);
void setFont(void *Font);
int printw(int x, int y, int xlim, int ylim, string format, ...);
int printwrich(int x, int y, int xlim, int ylim, int* defcolor, string format, ...);
void fontFree(glyphkind *Font);
