#ifndef BITMAP_GUARD
#define BITMAP_GUARD
#include "math/vec.h"
#include <vector>
#include <memory>
#include <string>
using std::vector;
using std::shared_ptr;
using std::string;
//functions to manipulate bitmaps

enum pixelFormat{TL_ALPHA, TL_RGB, TL_RGBA, TL_LUMINANCE, TL_LUMINANCE_ALPHA};

struct pixel{
	int R;
	int G;
	int B;
	int A;

};
///Used to store raw image data and manipulate it.
typedef shared_ptr<vector<unsigned char>> bytebuff;
struct bitmap{
	//data 
	string name;
	int height;///<height in pixels
	int width;///<width in pixels
	pixelFormat format;		///<how many elements per pixel
	bytebuff data;	///<pointer to raw data
	
	//functions
	int numPixels();		///<counts pixels
	int numBytes();		///<counts bytes
	int coordToIndex(vec2 pos);		///<x/y to I
	vec2 indexToCoord(int I);			///<I to x/y
	pixel read_pixel(vec2 pos);		///<gets pixel at x/y
	void write_pixel(vec2 pos, pixel P); ///<sets pixel at x/y
	pixel read_pixel(int I);			///<gets pixel at I
	void write_pixel(int I, pixel P);	///<sets pixel at I

	///uninitialized bitmap of 0 size.
	bitmap();
	///creates a copy of this bitmap
	bitmap clone();
	///returns the same bitmap with a different representation
	bitmap changeFormat(pixelFormat format_out);
	///reduces or increases the canvas size of a bitmap
	bitmap crop(int height, int width);
	///rotates by 90 degrees counter-clockwise
	bitmap rotateCCW();
	///rotates by 90 degrees clockwise
	bitmap rotateCW();
	///flips along the y axis
	bitmap flipVertical();
	///flips along the x axis
	bitmap flipHorizontal();
	///paints the other bitmap over this one
	void insert(bitmap BMP, vec2 pos);	
	///calls a function for each pixel
	void forEachPixel(void (*f)(pixel *P));
	///saves image to disk
	void saveAs(const char *filename);
	
	//takes the buffer over and wraps it into the bytebuffer thing.
	void setBuffer(unsigned char *buff, int size);
    //returns a pointer to the currently owned image buffer    
    unsigned char *getBuffer();

};

///tells you how many bytes are needed to represent a pixel at a given format
int bytesPerPixel(pixelFormat F);	

///creates a bitmap and fills it with a color.
bitmap blankBitmap(int height, int width, pixelFormat format);
#endif
