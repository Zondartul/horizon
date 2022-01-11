#include "GUIgrid.h"


//GUIgrid ----------------------------------------------------------------------

GUIgrid::GUIgrid(){}
void GUIgrid::grid(GUIbase *child, int row, int col, int spanx, int spany){
	//printf("grid(%p, row %d, col %d, spanx %d, spany %d)\n",child,row,col,spanx,spany);
	//if(!rows.size()){resize(1,1);}

	if(row == -1){row = rows.size() - 1;}
	spanx = max(spanx,1);
	spany = max(spany,1);
	resize(row+spany,col+spanx);
	if(spanx > 1){colspans.push_back(linespan{col,col+spanx-1,&(rows[row][col])});}
	if(spany > 1){rowspans.push_back(linespan{row,row+spany-1,&(rows[row][col])});}

	rows[row][col] = (gridcell){spanx,spany,child};
	invalidateTree();
	//printf("\n");
}
void GUIgrid::configureRow(int row, float min, float max, float weight){
	rowsettings[row] = (gridline){min,max,weight,0,0};
	invalidateTree();
}
void GUIgrid::configureColumn(int col, float min, float max, float weight){
	colsettings[col] = (gridline){min,max,weight,0,0};
	invalidateTree();
}
void GUIgrid::render(){
	//setScissoring(false);
	//GUIbase::render();
	//setColor(vec3(255,128,128));
	//setAlpha(128);
	//drawRect(worldArea());
	//setAlpha(255);
	//setColor(vec3(128,0,0));
	//drawRectOutline(worldArea());
	//setColor(vec3(255,0,255));
	//drawRectOutline(parent->parent->clientToWorld(parent->clientArea));
	//drawRectOutline(parent->visibleClientArea());
	//printf("g    area: %s\n",toString(area).c_str());
	// printf("gp   area: %s\n",toString(parent->area).c_str());
	// printf("gpc  area: %s\n",toString(parent->clientArea).c_str());
	// printf("gpp  area: %s\n",toString(parent->parent->area).c_str());
	// printf("gppc area: %s\n",toString(parent->parent->clientArea).c_str());
	// printf("\n");
	// exit(0);
	//drawRectOutline(visibleArea());
	/*
	setColor(vec3(255,0,0));
	vec2 offset = worldArea().start;
	for(int R = 0; R < rowsettings.size(); R++){
		for(int C = 0; C < colsettings.size(); C++){
			float y = rowsettings[R].pos;
			float ys = rowsettings[R].cur;
			float x = colsettings[C].pos;
			float xs = colsettings[C].cur;
			rect cellrect = rect().setStart(offset+vec2(x,y)).setSize(vec2(xs,ys));
			//frameprint(fstring("row %d, col %d. x=%f, y=%f, xs=%f, ys=%f, rect=%s",R,C,x,y,xs,ys,toString(cellrect).c_str()));
			drawRectOutline(cellrect);
		}
	}
	*/

}

void GUIgrid::resize(int numrows, int numcols){
	//printf("resize(%d,%d)\n",numrows,numcols);

	//does every row have settings?
	while(rowsettings.size() < numrows){
		rowsettings.push_back((gridline){-1,-1,0,0,0});
		//printf("added settings for row %d (%d rowsets)\n",rowsettings.size()-1,rowsettings.size());
	}
	//does every column have settings?
	while(colsettings.size() < numcols){
		colsettings.push_back((gridline){-1,-1,0,0,0});
		//printf("added settings for col %d (%d colsets)\n",colsettings.size()-1,colsettings.size());
	}

	//are there enough rows?
	while(rows.size() < rowsettings.size()){
		rows.push_back(gridrow());
		//printf("added row %d              (%d rows)\n",rows.size()-1, rows.size());
	}
	//does every row have enough columns?
	for(int R = 0; R < rows.size(); R++){
		while(rows[R].size() < colsettings.size()){
			rows[R].push_back((gridcell){1,1,0});
			//printf("new cell at row %d, col %d (%d cols)\n",R,rows[R].size()-1,rows[R].size());
		}
	}
}

void GUIgrid::clear(){
	for(int R = 0; R < rowsettings.size(); R++){
		for(int C = 0; C < colsettings.size(); C++){
			delete rows[R][C].child; //can has suppress-invalidate nao?
		}
	}
	rows.clear();
	rowsettings.clear();
	colsettings.clear();
}
#define nonnan(x) if(isnan(x)){error("result is NaN: %s\n",#x);}

void GUIgrid::invalidate(){
	//printf("grid invalidate");
//start with usual invalidate stuff
	//fill entire parent client area.
	//actually, set size later. expand but do not shrink.
	// if(parent){
		// printf("+parent\n");
		// area = rect(parent->clientArea.size);//parent->clientArea.moveTo(vec2(0,0));
	// }else{printf(", no parent\n");}
	// clientArea = area;
//first pass: measure row/column dimensions
	//debug print
	for(int R = 0; R < rowsettings.size(); R++){for(int C = 0; C < colsettings.size(); C++){
		auto &E = rows[R][C].child;
		//printf("cell (%d,%d):",R,C);
		//if(E){printf("area %s\n",toString(E->area).c_str());}
		// else{printf("no element\n");}
	}}
	//measure row height
	for(int R = 0; R < rowsettings.size(); R++){
		float maxy = 0;
		for(int C = 0; C < colsettings.size(); C++){
			auto &cell = rows[R][C];
			auto &E = cell.child;
			float y = 0;
			if(E){y = E->area.size.y;}
			if(cell.spany > 1){y = 0;} //span will be dealt with later
			nonnan(y);
			maxy = max(maxy,y);
			//if(C == 0){maxy = y;}
			//else{maxy = max(maxy,y);}
		}
		//preserve max content height for now, we still need to figure
		//out how much extra space we have.
		//grid expands when there is extra space.
		//grid does NOT shrink when there is not enough space.
		rowsettings[R].cur = maxy; //set row height to maximum height
		//printf("row %d max height = %f\n",R,maxy);
		nonnan(maxy);
	}
	//distribute span height
	for(int RS = 0; RS < rowspans.size(); RS++){
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
	for(int C = 0; C < colsettings.size(); C++){
		float maxx = 0;
		for(int R = 0; R < rowsettings.size(); R++){
			auto &cell = rows[R][C];
			auto &E = cell.child;
			float x = 0;
			if(E){x = E->area.size.x;}
			if(cell.spanx > 1){x = 0;} //span will be dealt with later
			nonnan(x);
			maxx = max(maxx,x);
			//if(R == 0){maxx = x;}
			//else{maxx = max(maxx,x);}
		}
		//same here
		colsettings[C].cur = maxx; //set column width to maximum width
		//printf("col %d max width = %f\n",C,maxx);
		nonnan(maxx);
	}
	//distribute span width
	//distribute span height
	for(int CS = 0; CS < colspans.size(); CS++){
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
	for(int R = 0; R < rowsettings.size(); R++){
		totaly = totaly + rowsettings[R].cur;
		totalweighty = totalweighty + rowsettings[R].weight;
	}
	for(int C = 0; C < colsettings.size(); C++){
		totalx = totalx + colsettings[C].cur;
		totalweightx = totalweightx + colsettings[C].weight;
	}
	nonnan(totaly);
	nonnan(totalweighty);
	nonnan(totalx);
	nonnan(totalweightx);
	float extray = 0;
	float extrax = 0;
	if(parent){
		extray = max(parent->clientArea.size.y - totaly,0);
		extrax = max(parent->clientArea.size.x - totalx,0);
	}
	//if too small to fit everything, set area to total area.
	//printf("area1 = %s\n",toString(area).c_str());
	area = rect(vec2(totalx+extrax,totaly+extray));
	clientArea = area;
	//printf("area2 = %s\n",toString(area).c_str());

//second pass: define row/col dimensions
	//new row height
	float posy = 0;
	for(int R = 0; R < rowsettings.size(); R++){
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

		//float extra_given = clamp(L->cur+extra_desired,L->min,L->max) - clamp(L->cur,L->min,L->max);
		//L->cur = clamp(L->cur+extra_desired,L->min,L->max);
		//extray = extray - extra_given;
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
	for(int C = 0; C < colsettings.size(); C++){
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

		//float extra_given = clamp(L->cur+extra_desired,L->min,L->max) - clamp(L->cur,L->min,L->max);
		//L->cur = clamp(L->cur+extra_desired,L->min,L->max);
		//extray = extray - extra_given;
		float extra_given = new_expanded - new_plain;
		L.cur = new_expanded;
		nonnan(L.cur);
		extrax = extrax - extra_given;
		L.pos = posx;
		nonnan(L.pos);
		posx = posx + L.cur;
	}
//third pass: set new cell position and size
	for(int R = 0; R < rowsettings.size(); R++){
		for(int C = 0; C < colsettings.size(); C++){
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
				//printf("cell (%d,%d) span %d,%d area %s\n",R,C,cell.spanx,cell.spany,toString(cell.child->area).c_str());
				cell.child->area = c_area;
				//cell.child->invalidate();
			}
		}
	}
}
