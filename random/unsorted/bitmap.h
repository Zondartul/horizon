#ifndef BITMAP_GUARD
#define BITMAP_GUARD


enum pixelFormat{TL_ALPHA, TL_RGB, TL_RGBA, TL_LUMINANCE, TL_LUMINANCE_ALPHA};
struct bitmap;
struct bitmapHandle{
	bitmap *bmp;
}

// texture.bmp -> resource.bitmap[123]
// bmp = resource.bitmap.new() 
// bmp = resource.bitmap.convert()
// 
// model.t -> resource.texture[123]
//

#endif