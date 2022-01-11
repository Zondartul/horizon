#include "texture.h"
#include "bitmap.h"
texture::texture(){
	UV = {{0,0},{0,0},{0,0}};
}
int texture::w(){if(!bmp){return 0;} return UV.size.x*bmp->width;}
int texture::h(){if(!bmp){return 0;} return UV.size.y*bmp->height;}
vec2i texture::size(){return {w(),h()};} 
rect texture::rect(){return ::rect(size());}
