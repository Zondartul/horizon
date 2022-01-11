#ifndef GUIGRID_GUARD
#define GUIGRID_GUARD
#include "GUIbase.h"

struct gridcell{
	int spanx;
	int spany;
	GUIbase *child;
};
typedef vector<gridcell> gridrow;
typedef vector<gridrow> gridkind;

struct gridline{
	float min;		//minimum size
	float max;		//maximum size
	float weight;	//how much of the extra space to give this line
	float cur;		//current size of grid line (set automatically)
	float pos;		//current position of grid line (set automatically)
};

struct linespan{
	int from;
	int to;
	gridcell *cell;
};

class GUIgrid:public virtual GUIbase{
	public:
	GUIgrid();
	virtual void grid(GUIbase *child, int row = -1, int col = 0, int spanx = 1, int spany = 1);
	virtual void configureRow(int row, float min, float max, float weight);
	virtual void configureColumn(int col, float min, float max, float weight);
	void resize(int numrows, int numcols);
	void clear();
	gridkind rows;
	vector<gridline> rowsettings;
	vector<gridline> colsettings;
	vector<linespan> rowspans;
	vector<linespan> colspans;
	virtual void render();
	virtual void invalidate();
};

#endif
