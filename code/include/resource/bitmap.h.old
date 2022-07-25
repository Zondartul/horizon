#ifndef BITMAP_GUARD
#define BITMAP_GUARD
#include <vector>
using std::vector;
#include "vec.h"
#include <memory>
using std::shared_ptr;
#include <string>
using std::string;
enum pixelFormat{TL_ALPHA, TL_RGB, TL_RGBA, TL_LUMINANCE, TL_LUMINANCE_ALPHA};
struct pixel{
	int R;
	int G;
	int B;
	int A;
};
typedef shared_ptr<vector<unsigned char>> bytebuff;
struct bitmap{
	string name;
	int height;
	int width;
	pixelFormat format;		
	bytebuff data;	
	int numPixels();		
	int numBytes();		
	int coordToIndex(vec2 pos);		
	vec2 indexToCoord(int I);			
	pixel read_pixel(vec2 pos);		
	void write_pixel(vec2 pos, pixel P); 
	pixel read_pixel(int I);			
	void write_pixel(int I, pixel P);	
	bitmap();
	bitmap clone();
	bitmap changeFormat(pixelFormat format_out);
	bitmap crop(int height, int width);
	bitmap rotateCCW();
	bitmap rotateCW();
	bitmap flipVertical();
	bitmap flipHorizontal();
	void insert(bitmap BMP, vec2 pos);	
	void forEachPixel(void (*f)(pixel *P));
	void saveAs(const char *filename);
	void setBuffer(unsigned char *buff, int size);
    unsigned char *getBuffer();
};
int bytesPerPixel(pixelFormat F);	
bitmap blankBitmap(int height, int width, pixelFormat format);
#endif
