#include "textureLoader.h"
#include <map>
using std::map;
#include "stdio.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPG
#include "stb_image.h"

#include "GL/gl.h"
#include "selfaware.h"
#include "globals.h"

textureHandle::textureHandle(){
	t = 0;
}
texture::texture(){
	UV = {{0,0},{0,0},{0,0}};
}
int texture::w(){return UV.size.x*handle.BMP.width;}
int texture::h(){return UV.size.y*handle.BMP.height;}
vec2i texture::size(){return {w(),h()};} 
rect texture::rect(){return ::rect(size());}

const char *getErrorString(GLenum error){
	switch(error){
		case(GL_NO_ERROR):return "OK";break;
		case(GL_INVALID_ENUM):return "INVALID_ENUM";break;
		case(GL_INVALID_VALUE):return "INVALID_VALUE";break;
		case(GL_INVALID_OPERATION):return "INVALID_OPERATION";break;
		case(GL_OUT_OF_MEMORY):return "OUT_OF_MEMORY";break;
		case(GL_STACK_UNDERFLOW):return "STACK_UNDERFLOW";break;
		case(GL_STACK_OVERFLOW):return "STACK_OVERFLOW";break;
		default:return "UNKNOWN_ERROR"; break;
	}
}

// struct texture{
	// string name;
	// int height;
	// int width;
	// unsigned int t;
// };

map<string, texture> textureCache;

texture getTexture(string name){
	if(!textureCache.count(name)){
		string path = locateResource("texture",name.c_str());
		bitmap BMP = loadImage(path.c_str());
		textureHandle h = uploadImage(BMP);
		//texture t = makeTexture(name, h);
		//textureCache[name] = t;
		makeTexture(name,h);
	}
	return textureCache[name];
}
bitmap loadImage(const char *filename){
	FILE *f = fopen(filename, "rb");
	if(!f){printf("ERROR: loadImage: can't open [%s]\n",filename);exit(0);}
	int width = 0;
	int height = 0;
	int components = 0;
	int requested_components = 4;
	unsigned char *data = stbi_load(filename, &width, &height, &components, requested_components);
	if(!data){printf("ERROR: loadImage: opened but can't parse [%s]\n",filename); exit(0);}
	bitmap BMP;
	BMP.width = width;
	BMP.height = height;
	BMP.format = TL_RGBA;
	BMP.data.reset(data);
	return BMP;
}
texture makeTexture(string name, textureHandle h){
	texture t;
	t.name = name;
	t.handle = h;
	t.UV = rectf(1,1);//rect({0,0},{h.BMP.width,h.BMP.height});
	textureCache[name] = t;
	return t;
}

GLenum pixelFormatToGL(pixelFormat F){
	switch(F){
	case(TL_ALPHA):
		return GL_ALPHA;
	break;
	case(TL_RGB):
		return GL_RGB;
	break;
	case(TL_RGBA):
		return GL_RGBA;
	break;
	case(TL_LUMINANCE):
		return GL_LUMINANCE;
	break;
	case(TL_LUMINANCE_ALPHA):
		return GL_LUMINANCE_ALPHA;
	break;
	}
	return (int)F;
}

textureHandle uploadImage(bitmap BMP){
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glGetError();
	glTexImage2D(GL_TEXTURE_2D, 0, pixelFormatToGL(BMP.format), BMP.width, BMP.height, 0, pixelFormatToGL(BMP.format), GL_UNSIGNED_BYTE, BMP.data.get());
	GLenum error = glGetError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	textureHandle h;
	h.BMP = BMP;
	h.t = textureID;
	return h;
}

