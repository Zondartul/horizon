#ifndef BITMAP_GUARD
#define BITMAP_GUARD
#include <vector>
using std::vector;
#include "vec.h"
#include <memory>
using std::shared_ptr;
#include <string>
using std::string;
//functions to manipulate bitmaps

enum pixelFormat{TL_ALPHA, TL_RGB, TL_RGBA, TL_LUMINANCE, TL_LUMINANCE_ALPHA};

struct pixel{
	int R;
	int G;
	int B;
	int A;
	// int getLuminance();
	// int setLuminance();
	// int getHue();
	// int setHue();
	// int getSaturation();
	// int setSaturation();
	// int getValue();
	// int setValue();
};
///Used to store raw image data and manipulate it.
struct bitmap{
	//data 
	string name;
	int height;///<height in pixels
	int width;///<width in pixels
	pixelFormat format;		///<how many elements per pixel
	shared_ptr<unsigned char> data;	///<pointer to raw data
	
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
};

///tells you how many bytes are needed to represent a pixel at a given format
int bytesPerPixel(pixelFormat F);	

///creates a bitmap and fills it with a color.
bitmap blankBitmap(int height, int width, pixelFormat format);
#endif