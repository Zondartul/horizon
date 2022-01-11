#ifndef RESOURCE_GUARD
#define RESOURCE_GUARD
#include <string>
using std::string;
#include <vector>
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
struct model;
struct font;

bitmap *getBmage(string name);
texture *getTexture(string name);
texture *getModelTexture(string name);
model *getModel(string name);
font *getFont(string name);

vector<bitmap*> listBitmaps();
vector<texture*> listTextures();
vector<model*> listModels();
vector<font*> listFonts();

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
void loadAssets();

#endif
