#ifndef GUI5CONSTRAINT_GUARD
#define GUI5CONSTRAINT_GUARD
#include "stdint.h"
#include "gui/Gui5/GUI5base.h"
//constraints are classes that are used to create automatic GUI layouts
//First, assume that 

enum GUI5constraintDependency{
	getsParentSize		= 1 << 0,
	getsParentPos		= 1 << 1,
	getsNeighborSize	= 1 << 2,
	getsNeighborPos		= 1 << 3,
	getsChildrenSize	= 1 << 4,
	getsChildrenPos		= 1 << 5,
	getsOwnSize			= 1 << 6,
	getsOwnPos			= 1 << 7,
	setsParentSize		= 1 << 8,
	setsParentPos		= 1 << 9,
	setsNeighborSize	= 1 << 10,
	setsNeighborPos		= 1 << 11,
	setsChildrenSize	= 1 << 12,
	setsChildrenPos		= 1 << 13,
	setsOwnSize			= 1 << 14,
	setsOwnPos			= 1 << 15
};

class GUI5constraint{
	public:
	uint16_t dependencyFlags;	//used by constraintManager to activate this constraint
	GUI5base *self;				//refers to the element to which the constraint is applied
	GUI5constraint();			//constructor
	virtual void invalidate();	//re-establishes the layout relationship 
								//specified by this constraint
};

//sizes the element according to contents
class GUI5constraintSizeToContents:public GUI5constraint{
	public:
	GUI5constraintSizeToContents(GUI5base *self);
	virtual void invalidate();
};

//sizes the element according to parent size
class GUI5constraintPercentParentSize:public GUI5constraint{
	public:
	//size becomes ratiox*parent.x + addx
	float ratiox;
	float ratioy;
	int addx;
	int addy;
	GUI5constraintPercentParentSize(GUI5base *self);
	virtual void invalidate();
};

//lines up children in horizontal rows
class GUI5constraintHorizontalList:public GUI5constraint{
	public:
	//if true, elements will go to the second row when they reach the edge.
	bool wrap;
	//vertical alignment within horizontal rows:
	//0 - top, 1 - center, 2 - bottom
	//row height is to-contents.
	int verticalAlignment; 
	GUI5constraintHorizontalList(GUI5base *self);
	virtual void invalidate();
};

//lines up children in vertical columns
class GUI5constraintVerticalList:public GUI5constraint{
	public:
	//if true, elements will go to the second column when they reach the edge.
	bool wrap;
	//horizontal alignment within vertical rows:
	//0 - left, 1 - center, 2 - right
	//column width is to-contents.
	int horizontalAlignment; 
	GUI5constraintVerticalList(GUI5base *self);
	virtual void invalidate();
};

//aligns the child top-middle-bottom and left-middle-right.
class GUI5constraintAlignment:public GUI5constraint{
	public:
	int verticalAlignment;		//0 - top, 1 - center, 2 - bottom
	int horizontalAlignment;	//0 - left, 1 - center, 2 - bottom
	GUI5constraintAlignment(GUI5base *self);
	virtual void invalidate();
};

#endif