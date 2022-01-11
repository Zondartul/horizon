#include "textureLoader.h"
//#include <map>
//using std::map;
#include "stdio.h"



//#include "GL/gl.h"
//#include "selfaware.h"
//#include "file.h"
#include "globals.h"
#include "imageLoader.h"

//should get a specific texture loader eventually.

texture *loadTexture(const char *filename){
	bitmap *bmp = loadImage(filename);
	if(!bmp){return 0;}
	texture *t = new texture();
	t->bmp = bmp;
	t->UV = rectf({0,0},{1,1});
	return t;
}


// const char *getErrorString(GLenum error){
	// switch(error){
		// case(GL_NO_ERROR):return "OK";break;
		// case(GL_INVALID_ENUM):return "INVALID_ENUM";break;
		// case(GL_INVALID_VALUE):return "INVALID_VALUE";break;
		// case(GL_INVALID_OPERATION):return "INVALID_OPERATION";break;
		// case(GL_OUT_OF_MEMORY):return "OUT_OF_MEMORY";break;
		// case(GL_STACK_UNDERFLOW):return "STACK_UNDERFLOW";break;
		// case(GL_STACK_OVERFLOW):return "STACK_OVERFLOW";break;
		// default:return "UNKNOWN_ERROR"; break;
	// }
// }


//map<string, texture> textureCache;

// texture getTexture(string name){
	// if(!textureCache.count(name)){
		// string path = locateResource("texture",name.c_str());
		// bitmap BMP = loadImage(path.c_str());
		// textureHandle h = uploadImage(BMP);
		////texture t = makeTexture(name, h);
		////textureCache[name] = t;
		// makeTexture(name,h);
	// }
	// return textureCache[name];
// }
// texture makeTexture(string name, textureHandle h){
	// texture t;
	// t.name = name;
	// t.handle = h;
	// t.UV = rectf(1,1);//rect({0,0},{h.BMP.width,h.BMP.height});
	// textureCache[name] = t;
	// return t;
// }



// textureHandle uploadImage(bitmap BMP){
	// GLuint textureID;
	// glGenTextures(1, &textureID);
	// glBindTexture(GL_TEXTURE_2D, textureID);
	// glGetError();
	// glTexImage2D(GL_TEXTURE_2D, 0, pixelFormatToGL(BMP.format), BMP.width, BMP.height, 0, pixelFormatToGL(BMP.format), GL_UNSIGNED_BYTE, BMP.data.get());
	// GLenum error = glGetError();
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// textureHandle h;
	// h.BMP = BMP;
	// h.t = textureID;
	// printf("texture %p uploaded to GPU: %d (%dx%d)\n",BMP.data,textureID,BMP.height,BMP.width);
	// return h;
// }

