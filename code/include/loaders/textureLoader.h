#ifndef TEXTURELOADER_GUARD
#define TEXTURELOADER_GUARD
#include "texture.h"


//returns a texture by the given name.
//if no such texture exists, attempts to 
//load a texture by that filename.
texture *loadTexture(const char *filename);

#endif