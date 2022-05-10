#ifndef RESOURCE_GUARD
#define RESOURCE_GUARD
#include <string>
using std::string;
#include <vector>
using std::vector;









struct bitmap;
struct texture;
class model;
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

























void loadAssets();

struct gs_resourceKind {
	vector<bitmap*> g_bitmaps;
	vector<texture*> g_textures;
	vector<model*> g_models;
	vector<font*> g_fonts;
};

#endif
