#ifndef GUI5SPLIT_GUARD
#define GUI5SPLIT_GUARD
#include "gui/Gui5/GUI5base.h"

class GUI5split:public GUI5base{
	vector<int> ratio;
	bool vertical;
	int borderSize;
	int mouseOverBar; //says which bar has mouse-over right now.
	int ratioTotal(); //sums up all the numbers in the ratio vector.
	bool dragging;	  //currently dragging one of the bars
	bool cursorSet;   //cursor has been changed
	public:
	GUI5split();
	GUI5split &setRatio(vector<int> newRatio);
	GUI5split &setVertical(bool newVertical);
	GUI5split &setBorderSize(int borderSize);
	void receiveMessageExtra(message *msg);
	void render();
	void layout();
};

#endif