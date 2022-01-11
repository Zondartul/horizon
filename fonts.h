struct glyphkind
{
    int bearingX;
    int bearingY;
    int sizeX;
    int sizeY;
    int advance;
    GLuint texture;
};
glyphkind* GenerateFont(const char* filepath, int size);
void setFont(glyphkind *Font);
int printw(int x, int y, const char* format, ...);
void fontFree(glyphkind *Font);
