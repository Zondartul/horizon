#ifndef GUI_TEXTUREBROWSER_H
#define GUI_TEXTUREBROWSER_H
#include "GUI.h"
class GUItextureBrowser: public GUIwindow{
	public:
	GUItextureBrowser();
	void addTile(string imgpath="", string label="");
	void populate();
	GUIcompoundProperty Pwindow, Ptile;
	GUIlabel *lbl;
	GUIbutton *btn;
	GUIselectionGroup *sg;
	GUIscrollbar *scroll;
	int numTiles = 0;
};
void openTextureBrowser();
#endif