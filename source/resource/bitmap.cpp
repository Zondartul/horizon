#include "bitmap.h"
#include "simplemath.h"
#include "globals.h"

bitmap::bitmap(){
	height = 0;
	width = 0;
	format = TL_RGBA;
}
int bitmap::numPixels(){
	return height*width;
}
int bitmap::numBytes(){
	return numPixels()*bytesPerPixel(format);
}
int bitmap::coordToIndex(vec2i pos){
	return width*pos.y+pos.x;
}
vec2i bitmap::indexToCoord(int I){
	return {I/width,I%width};
}
pixel bitmap::read_pixel(vec2i pos){
	return read_pixel(coordToIndex(pos));
}
void bitmap::write_pixel(vec2i pos, pixel P){
	write_pixel(coordToIndex(pos), P);
}

pixel bitmap::read_pixel(int I){
	pixel P;
	int *R = &P.R;
	int *G = &P.G;
	int *B = &P.B;
	int *A = &P.A;
	*R = *G = *B = *A = 0;
	unsigned char *d = data.get();
	switch(format){
		case(TL_ALPHA):
			*A = d[I];
		break;
		case(TL_RGB):
			*R = d[3*I];
			*G = d[3*I+1];
			*B = d[3*I+2];
		break;
		case(TL_RGBA):
			*R = d[4*I];
			*G = d[4*I+1];
			*B = d[4*I+2];
			*A = d[4*I+3];
		break;
		case(TL_LUMINANCE):
			*R = *G = *B = d[I];
		break;
		case(TL_LUMINANCE_ALPHA):
			*R = *G = *B = d[2*I];
			*A = d[2*I+1];
		break;
	}
	return P;
}
void bitmap::write_pixel(int I, pixel P){
	int R = P.R;
	int G = P.G;
	int B = P.B;
	int A = P.A;
	int L;
	unsigned char *d = data.get();
	switch(format){
		case(TL_ALPHA):
			d[I] = A;
		break;
		case(TL_RGB):
			d[3*I] = R;
			d[3*I+1] = G;
			d[3*I+2] = B;
		break;
		case(TL_RGBA):
			d[4*I] = R;
			d[4*I+1] = G;
			d[4*I+2] = B;
			d[4*I+3] = A;
		break;
		case(TL_LUMINANCE):
			L = 0.2126*R + 0.7152*G + 0.0722*B;
			d[I] = L;
		break;
		case(TL_LUMINANCE_ALPHA):
			L = 0.2126*R + 0.7152*G + 0.0722*B;
			d[2*I] = L;
			d[2*I+1] = A;
		break;
	}
}

bitmap bitmap::changeFormat(pixelFormat format_out){
	bitmap newBMP = blankBitmap(height, width, format_out);
	int R,G,B,A;
	int n = numPixels();
	for(int I = 0; I < n; I++){
		newBMP.write_pixel(I, read_pixel(I));
	}
	return newBMP;
}

bitmap bitmap::crop(int newheight, int newwidth){
	bitmap BMP = blankBitmap(newheight,newwidth,format);
	BMP.insert(*this,{0,0});
	return BMP;
}
bitmap bitmap::rotateCCW(){
	bitmap BMP = blankBitmap(width,height,format);
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			BMP.write_pixel({y,width-1-x},read_pixel({x,y}));
		}
	}
	return BMP;
}
bitmap bitmap::rotateCW(){
	bitmap BMP = blankBitmap(width,height,format);
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			BMP.write_pixel({height-1-y,x},read_pixel({x,y}));
		}
	}
	return BMP;
}
bitmap bitmap::flipVertical(){
	bitmap BMP = blankBitmap(height,width,format);
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			BMP.write_pixel({x,height-1-y},read_pixel({x,y}));
		}
	}
	return BMP;
}
bitmap bitmap::flipHorizontal(){
	bitmap BMP = blankBitmap(height,width,format);
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			BMP.write_pixel({width-1-x,y},read_pixel({x,y}));
		}
	}
	return BMP;
}
void bitmap::insert(bitmap BMP, vec2i pos){
	//printf("BMP [%d x %d]: insert BMP [%d x %d] at (%d,%d)\n",
	//		width,height,BMP.width,BMP.height,pos.x,pos.y);
	int minx = clamp(pos.x,0,width);
	int maxx = min(pos.x+BMP.width,width);
	int miny = clamp(pos.y,0,height);
	int maxy = min(pos.y+BMP.height,height);
	//printf("minx = %d\n",minx);
	//printf("maxx = %d\n",maxx);
	//printf("miny = %d\n",miny);
	//printf("maxy = %d\n",maxy);
	for(int y = miny; y < maxy; y++){
		for(int x = minx; x < maxx; x++){
			int readx = x-pos.x;
			int ready = y-pos.y;
			//printf("pixel read (%d,%d), write (%d,%d)\n",readx,ready,x,y);
			write_pixel({x,y},BMP.read_pixel({readx,ready}));
		}
	}
}
void bitmap::forEachPixel(void (*f)(pixel *P)){
	for(int I = 0; I < numPixels(); I++){
		pixel P = read_pixel(I);
		(*f)(&P);
		write_pixel(I,P);
	}
}

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

//int stbi_write_png(char const *filename, 
//		int w, int h, int comp, const void *data, int stride_in_bytes);
   
void bitmap::saveAs(const char *filename){
	int comp;
	switch(format){
		case(TL_ALPHA):
			changeFormat(TL_LUMINANCE_ALPHA).saveAs(filename);
			return;
		break;
		case(TL_RGB):
			comp = 3;
		break;
		case(TL_RGBA):
			comp = 4;
		break;
		case(TL_LUMINANCE):
			comp = 1;
		break;
		case(TL_LUMINANCE_ALPHA):
			comp = 2;
		break;
	}
	stbi_write_png(filename,width,height,comp,data.get(), width*bytesPerPixel(format));
}

int bytesPerPixel(pixelFormat F){
	switch(F){
		case(TL_ALPHA): return 1; break;
		case(TL_RGB): return 3; break;
		case(TL_RGBA): return 4; break;
		case(TL_LUMINANCE): return 1; break;
		case(TL_LUMINANCE_ALPHA): return 2; break;
	}
}

bitmap blankBitmap(int height, int width, pixelFormat format){
	bitmap BMP;
	BMP.height = height;
	BMP.width = width;
	BMP.format = format;
	int size = BMP.numBytes();
	if(!size){size=1;}
	//printf("bbmp (%dx%d) BMP = %p, size = %d\n",height,width,&BMP, size);
	unsigned char *buff = (unsigned char*)calloc(1,size);
	if(!buff){printf("can't alloc bitmap!\n");exit(0);}
	//printf("bbmp buff = %p\n",buff);
	BMP.data.reset(buff);
	return BMP;
}

bitmap bitmap::clone(){
	bitmap BMP;
	BMP.height = height;
	BMP.width = width;
	BMP.format = format;
	int size = BMP.numBytes();
	if(!size){size=1;}
	//printf("bbmp (%dx%d) BMP = %p, size = %d\n",height,width,&BMP, size);
	unsigned char *buff = (unsigned char*)calloc(1,size);
	if(!buff){printf("can't alloc bitmap!\n");exit(0);}
	memcpy(buff,data.get(),size);
	//printf("bbmp buff = %p\n",buff);
	BMP.data.reset(buff);
	return BMP;
}

