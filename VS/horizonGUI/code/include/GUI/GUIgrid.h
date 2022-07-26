#ifndef GUIGRID_GUARD
#define GUIGRID_GUARD
#include "util/globals_gui.h"

struct DLLAPI_GUI gridcell{
	int spanx;
	int spany;
	GUIbase *child;
};

typedef vector<gridcell> gridrow;
typedef vector<gridrow> gridkind;

struct DLLAPI_GUI gridline{
	float min;		
	float max;		
	float weight;	
	float cur;		
	float pos;		
};

struct DLLAPI_GUI linespan{
	int from;
	int to;
	gridcell *cell;
};

class DLLAPI_GUI GUIgrid:public virtual GUIbase{
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
