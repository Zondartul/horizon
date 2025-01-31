#include "GUI_internal.h"


//GUIgrid ----------------------------------------------------------------------

GUIgrid::GUIgrid(){}
void GUIgrid::grid(GUIbase *child, int row, int col, int spanx, int spany){
	printf("grid(%p, row %d, col %d, spanx %d, spany %d)\n",child,row,col,spanx,spany);
	//row = -1 means "new row"
	spanx = (int)max(spanx,1);
	spany = (int)max(spany,1);
	int extra_cells_x = spanx-1;
	int extra_cells_y = spany-1;
	bool nextRow = (row == -1);
	if(row == -1){row = getNumRows();}
	
	int min_size_y = max(row + extra_cells_y + 1, 1);
	int min_size_x = max(col + extra_cells_x + 1, 1);
	printf("min_size_y 1 = %d, min_size_x 1 = %d\n",min_size_y, min_size_x);
	min_size_y = (int)max(min_size_y, getNumRows());
	min_size_x = (int)max(min_size_x, getNumCols());
	printf("min_size_y 2 = %d, min_size_x 2 = %d\n",min_size_y, min_size_x);
	printf("numRows = %d, numCols = %d\n",getNumRows(),getNumCols());
	
	if((getNumRows() < min_size_y) || (getNumCols() < min_size_x)){resize(min_size_y, min_size_x);}
	
	if(spanx > 1){colspans.push_back(linespan{col,col+spanx-1,&(rows[row][col])});}
	if(spany > 1){rowspans.push_back(linespan{row,row+spany-1,&(rows[row][col])});}

	printf(".grid: putting child at row %d, col %d\n",row,col);
	rows[row][col] = gridcell{spanx,spany,child};
	printf(".grid: area before = %s\n",toCString(area));
	printf(".grid: client area before = %s\n",toCString(clientArea));
	invalidateTree();
	printf(".grid: area after  = %s\n",toCString(area));
	printf(".grid: client area after  = %s\n",toCString(clientArea));
}
void GUIgrid::configureRow(int row, float min, float max, float weight){
	rowsettings[row] = gridline{min,max,weight,0,0};
	invalidateTree();
}
void GUIgrid::configureColumn(int col, float min, float max, float weight){
	colsettings[col] = gridline{min,max,weight,0,0};
	invalidateTree();
}


int GUIgrid::getNumRows(){return rows.size();}
int GUIgrid::getNumCols(){if(rows.size()){return rows[0].size();}else return 0;}

void GUIgrid::resize(unsigned int numrows, unsigned int numcols){
	printf("resize(%d,%d)\n",numrows,numcols);

	//does every row have settings?
	while(rowsettings.size() < numrows){
		rowsettings.push_back(gridline{-1,-1,0,0,0});
	}
	//does every column have settings?
	while(colsettings.size() < numcols){
		colsettings.push_back(gridline{-1,-1,0,0,0});
	}

	//are there enough rows?
	while(rows.size() < rowsettings.size()){
		rows.push_back(gridrow());
	}
	//does every row have enough columns?
	for(unsigned int R = 0; R < rows.size(); R++){
		while(rows[R].size() < colsettings.size()){
			rows[R].push_back(gridcell{1,1,0});
		}
	}
}

void GUIgrid::clear(){
	for(unsigned int R = 0; R < rowsettings.size(); R++){
		for(unsigned int C = 0; C < colsettings.size(); C++){
			delete rows[R][C].child; //can has suppress-invalidate nao?
		}
	}
	rows.clear();
	rowsettings.clear();
	colsettings.clear();
}
#define nonnan(x) if(isnan(x)){error("result is NaN: %s\n",#x);}

void GUIgrid::invalidate(){
//first pass: measure row/column dimensions
	//measure row height
	for(unsigned int R = 0; R < rowsettings.size(); R++){
		float maxy = 0;
		for(unsigned int C = 0; C < colsettings.size(); C++){
			auto &cell = rows[R][C];
			auto &E = cell.child;
			float y = 0;
			if(E){y = E->area.size.y;}
			if(cell.spany > 1){y = 0;} //span will be dealt with later
			nonnan(y);
			maxy = max(maxy,y);
		}
		//preserve max content height for now, we still need to figure
		//out how much extra space we have.
		//grid expands when there is extra space.
		//grid does NOT shrink when there is not enough space.
		rowsettings[R].cur = maxy; //set row height to maximum height
		nonnan(maxy);
	}
	//distribute span height
	for(unsigned int RS = 0; RS < rowspans.size(); RS++){
		int from = rowspans[RS].from;
		int to = rowspans[RS].to;
		auto &E = rowspans[RS].cell->child;
		if(!E){continue;}
		float availy = 0;
		for(int R = from; R <= to; R++){
			availy = availy + rowsettings[R].cur;
		}
		float desiredy = E->area.size.y;
		if(availy){
			float coeff = desiredy/availy;
			if(coeff > 1.f){
				for(int R = from; R < to; R++){
					rowsettings[R].cur = coeff*rowsettings[R].cur;
				}
			}
		}else{
			for(int R = from; R < to; R++){
				rowsettings[R].cur = desiredy/((float)(to-from+1));
			}
		}
	}
	//measure column width
	for(unsigned int C = 0; C < colsettings.size(); C++){
		float maxx = 0;
		for(unsigned int R = 0; R < rowsettings.size(); R++){
			auto &cell = rows[R][C];
			auto &E = cell.child;
			float x = 0;
			if(E){x = E->area.size.x;}
			if(cell.spanx > 1){x = 0;} //span will be dealt with later
			nonnan(x);
			maxx = max(maxx,x);
		}
		//same here
		colsettings[C].cur = maxx; //set column width to maximum width
		nonnan(maxx);
	}
	//distribute span width
	//distribute span height
	for(unsigned int CS = 0; CS < colspans.size(); CS++){
		int from = colspans[CS].from;
		int to = colspans[CS].to;
		auto &E = colspans[CS].cell->child;
		if(!E){continue;}
		float availx = 0;
		for(int C = from; C <= to; C++){
			availx = availx + colsettings[C].cur;
		}
		float desiredx = E->area.size.x;
		if(availx){
			float coeff = desiredx/availx;
			if(coeff > 1.f){
				for(int C = from; C <= to; C++){
					colsettings[C].cur = coeff*colsettings[C].cur;
				}
			}
		}else{
			for(int C = from; C <= to; C++){
				colsettings[C].cur = desiredx/((float)(to-from+1));
			}
		}
	}
	//measure total height/width and total weight
	float totaly = 0;
	float totalweighty = 0;
	float totalx = 0;
	float totalweightx = 0;
	for(unsigned int R = 0; R < rowsettings.size(); R++){
		totaly = totaly + rowsettings[R].cur;
		totalweighty = totalweighty + rowsettings[R].weight;
	}
	for(unsigned int C = 0; C < colsettings.size(); C++){
		totalx = totalx + colsettings[C].cur;
		totalweightx = totalweightx + colsettings[C].weight;
	}
	nonnan(totaly);
	nonnan(totalweighty);
	nonnan(totalx);
	nonnan(totalweightx);
	float extray = 0;
	float extrax = 0;
	if(parent && bSizeToParent){
		extray = max(parent->clientArea.size.y - totaly,0);
		extrax = max(parent->clientArea.size.x - totalx,0);
	}
	//if too small to fit everything, set area to total area.

	area = rect(vec2(totalx+extrax,totaly+extray));
	clientArea = area;

//second pass: define row/col dimensions
	//new row height
	float posy = 0;
	for(unsigned int R = 0; R < rowsettings.size(); R++){
		auto &L = rowsettings[R];
		nonnan(L.cur);
		//row wants <weight>/<totalweight> of the extra dimension
		float extra_desired = 0;
		if(L.weight){
			extra_desired = extray*L.weight/totalweighty;
		}
		//but is constrained by the min/max dimensions
		float new_plain = L.cur;
		float new_expanded = L.cur+extra_desired;
		if(L.min != -1){
			new_plain = max(new_plain,L.min);
			new_expanded = max(new_expanded,L.min);
		}
		if(L.max != -1){
			new_plain = min(new_plain,L.max);
			new_expanded = min(new_expanded,L.max);
		}
		float extra_given = new_expanded - new_plain;
		L.cur = new_expanded;
		nonnan(L.cur);
		extray = extray - extra_given;
		L.pos = posy;
		nonnan(L.pos);
		posy = posy + L.cur;
	}
	//new column width
	float posx = 0;
	for(unsigned int C = 0; C < colsettings.size(); C++){
		auto &L = colsettings[C];
		//row wants <weight>/<totalweight> of the extra dimension
		float extra_desired = 0;
		if(L.weight){
			extra_desired = extrax*L.weight/totalweightx;
		}
		//but is constrained by the min/max dimensions
		float new_plain = L.cur;
		float new_expanded = L.cur+extra_desired;
		if(L.min != -1){
			new_plain = min(new_plain,L.min);
			new_expanded = min(new_expanded,L.min);
		}
		if(L.max != -1){
			new_plain = max(new_plain,L.max);
			new_expanded = max(new_expanded,L.max);
		}
		float extra_given = new_expanded - new_plain;
		L.cur = new_expanded;
		nonnan(L.cur);
		extrax = extrax - extra_given;
		L.pos = posx;
		nonnan(L.pos);
		posx = posx + L.cur;
	}
//third pass: set new cell position and size
	for(unsigned int R = 0; R < rowsettings.size(); R++){
		for(unsigned int C = 0; C < colsettings.size(); C++){
			auto &cell = rows[R][C];
			if(cell.child){
				//calc. available y-size
				float ysize = 0;
				for(int I = 0; I < cell.spany; I++){
					ysize += rowsettings[R+I].cur;
				}
				//calc. available x-cells
				float xsize = 0;
				for(int I = 0; I < cell.spanx; I++){
					xsize += colsettings[C+I].cur;
				}
				float ypos = rowsettings[R].pos;
				float xpos = colsettings[C].pos;
				rect c_area = cell.child->area;
				vec2 acenter_offset = c_area.size/2.f;
				vec2 bcenter_offset = vec2(xsize,ysize)/2.f;
				c_area = c_area.moveTo(vec2(xpos,ypos)-acenter_offset+bcenter_offset);
				cell.child->area = c_area;
			}
		}
	}
}

void GUIgrid::shrinkChildren(){
	for(int R = 0; R != rows.size(); R++){
		gridline row_setting = rowsettings[R];
		for(auto C = 0; C != rows[R].size(); C++){
			gridline col_setting = colsettings[C];
			gridcell &gc = rows[R][C];
			//we ignore spans for now, implement later
			float sizex = col_setting.cur;
			float sizey = row_setting.cur;
			if(gc.child){
				gc.child->moveTo(vec2(0,0));
				gc.child->setSize(vec2(sizex,sizey));
			}
		}
	}
}

//grid is automatically enlarged to fit all elements,
//this can automatically shrink it.
void GUIgrid::sizeToGrid(){
	float sizex = 0;
	float sizey = 0;
	for(auto R = rowsettings.begin(); R != rowsettings.end(); R++){
		sizey += R->cur;
	}
	for(auto C = colsettings.begin(); C != colsettings.end(); C++){
		sizex += C->cur;
	}
	printf("sizeToGrid: x = %f (%d cols), y = %f (%d rows)\n",
		sizex, colsettings.size(),
		sizey, rowsettings.size());
	setSize(vec2(sizex,sizey));
	printf("size after invalidate: x = %f, y = %f\n",area.size.x, area.size.y);
}

string GUIgrid::getType(){return "GUIgrid";}











