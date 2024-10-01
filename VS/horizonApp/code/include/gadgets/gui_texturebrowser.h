#ifndef GUI_TEXTUREBROWSER_H
#define GUI_TEXTUREBROWSER_H
#include "GUI/GUI.h"

class GUItextureBrowser: public GUIwindow{
	public:
	GUItextureBrowser();
	void addTile(string imgpath="", string label="");
	void populate(); /// add tiles for the textures in the current directory
	void clear();	/// remove all tiles from view
	void setDirectory(string path);
	void onBtnBrowse();
	string basepath = "resource/textures";
	GUIcompoundProperty Pwindow, Ptile;
	GUIlabel *lbl;
	GUIbutton *btn;
	GUIselectionGroup *sg;
	GUIscrollbar *scroll;
	GUItextEntry *TBpath;
	GUIbutton *TBbtnBrowse;
	std::string dir; /// current directory
	int numTiles = 0;
};
void openTextureBrowser();
#endif