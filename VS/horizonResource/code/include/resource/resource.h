#ifndef RESOURCE_GUARD
#define RESOURCE_GUARD
#include "util/globals_resource.h"
#include "util/res_or_err.hpp"
#include "util/zerror.hpp"
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

Result<bitmap*,zError> DLLAPI_RESOURCE getBmage(string name);
Result<texture*,zError> DLLAPI_RESOURCE getTexture(string name);
Result<texture*,zError> DLLAPI_RESOURCE getModelTexture(string name);
Result<model*,zError> DLLAPI_RESOURCE getModel(string name);
Result<font*,zError> DLLAPI_RESOURCE getFont(string name);

vector<bitmap*> DLLAPI_RESOURCE listBitmaps();
vector<texture*> DLLAPI_RESOURCE listTextures();
vector<model*> DLLAPI_RESOURCE listModels();
vector<font*> DLLAPI_RESOURCE listFonts();

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
void DLLAPI_RESOURCE loadAssets();
void DLLAPI_RESOURCE initFallbackCursors();
#endif
