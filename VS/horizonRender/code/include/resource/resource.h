#ifndef RESOURCE_GUARD
#define RESOURCE_GUARD
#include "util/globals_render.h"
#include <vector>
#include <string>
using std::string;
using std::vector;
//central resource/asset manager, owns all CPU-side resources.
//										network
//                   +->generators -+     |^
//                   |              v     v|
// HDD -> loaders -> resource manager -> game -----+-> render -> GPU
//  ^                        |           	       |
//  +---- savers  <----------+                     +-> sound  -> SPU
//

struct bitmap;
struct texture;
class model;
struct font;

bitmap DLLAPI_RENDER *getBmage(string name);
texture DLLAPI_RENDER *getTexture(string name);
texture DLLAPI_RENDER *getModelTexture(string name);
model DLLAPI_RENDER *getModel(string name);
font DLLAPI_RENDER *getFont(string name);

vector<bitmap*> DLLAPI_RENDER listBitmaps();
vector<texture*> DLLAPI_RENDER listTextures();
vector<model*> DLLAPI_RENDER listModels();
vector<font*> DLLAPI_RENDER listFonts();

//about textures, bitmap, atlases and materials:
// bmp contains picture data
// texture is subsection of a picture that is referrable by render/GPU.
//		render handle is how renderer keeps track of picture data inside GPU.
// material is a collection of textures that specifies fancy visual properties.
//		e.g. color/diffuse, specular, emissive, normals, height/bump, transparency, etc.
//
//
//
// material +>texture1------>bmp1 (not uploaded)
//			|
//		   	+>texture2------>bmp2
//		   	|	  +------------>rhandle1
//			|
//			+>texture3----+->bmp3
//                +-------|--+->rhandle2 (atlased texture)
//						  |  |
// material	+>texture1----+  |
//			|     +----------+
//			|
//		   	+>texture2
//
//
//
void DLLAPI_RENDER loadAssets();

#endif
