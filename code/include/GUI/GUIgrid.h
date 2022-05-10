#ifndef GUIGRID_GUARD
#define GUIGRID_GUARD

struct gridcell{
	int spanx;
	int spany;
	GUIbase *child;
};
typedef vector<gridcell> gridrow;
typedef vector<gridrow> gridkind;

struct gridline{
	float min;		
	float max;		
	float weight;	
	float cur;		
	float pos;		
};

struct linespan{
	int from;
	int to;
	gridcell *cell;
};

/* --- GUI GRID MANUAL ---
  1. create your elements and give them a size. Size of elements determines size of grid, not vice-versa.
  2. call grid->addChild(element). The element will not yet receive a layout.
  
  if you know the number of elements in advance,
    3. call grid->resize(num rows, num columns).
    Once a row or column exists, it can be configured for max/min size and weight.
    Row and column will obtain size from elements you put in them, but will always be at least min and at most max pixels.
    4. call grid->grid(element, row, column) to put the element (that is already a child) into the right position.
							..spanx, spany should make the element span several cells starting from the one specified.
							
  if you don't know the number of elements in advance,
    3. call grid->grid(element, -1, col) to make a new row and put the element at position col in that row.
	
  THE GRID WILL AUTO-RESIZE THE ROWS AND COLUMNS TO MATCH CONTENT
  THE GRID ITSELF WILL BE AS BIG AS CONTENTS UNLESS SIZE TO PARENT IS SET, then contents will be smaller unless allowed to expand.
  check the size of parent object as the grid will ignore it and may end up clipped.
  
*/

class GUIgrid:public virtual GUIbase{
	public:
	GUIgrid();
	virtual void grid(GUIbase *child, int row = -1, int col = 0, int spanx = 1, int spany = 1);
	virtual void configureRow(int row, float min, float max, float weight);
	virtual void configureColumn(int col, float min, float max, float weight);
	int getNumRows();
	int getNumCols();
	void resize(unsigned int numrows, unsigned int numcols);
	void clear();
	bool bSizeToParent = false;
	gridkind rows;
	vector<gridline> rowsettings;
	vector<gridline> colsettings;
	vector<linespan> rowspans; 
	vector<linespan> colspans; 

	void shrinkChildren();
	void sizeToGrid(); 
	virtual void invalidate();
	
	
	virtual string getType(); 
};

#endif
