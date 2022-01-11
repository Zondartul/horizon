#include "modelprimitives.h"
#include "rmodel.h"
#include "editmodel.h"

rmodel *generateBox(vec3 scale){
	e_model EM;
	
	e_vertex *A = new e_vertex({0,0,0},&EM);
	e_vertex *B = new e_vertex({1,0,0},&EM);
	e_vertex *C = new e_vertex({0,1,0},&EM);
	e_vertex *D = new e_vertex({1,1,0},&EM);
	e_vertex *E = new e_vertex({0,0,1},&EM);
	e_vertex *F = new e_vertex({1,0,1},&EM);
	e_vertex *G = new e_vertex({0,1,1},&EM);
	e_vertex *H = new e_vertex({1,1,1},&EM);
	
	e_triangle *top1 = new e_triangle(E,F,G,&EM);
	e_triangle *top2 = new e_triangle(H,F,G,&EM);
	
	e_triangle *bottom1	= new e_triangle(A,B,C,&EM);
	e_triangle *bottom2 = new e_triangle(D,B,C,&EM);
	
	e_triangle *left1 = new e_triangle(A,B,E,&EM);
	e_triangle *left2 = new e_triangle(F,B,E,&EM);
	
	e_triangle *right1 = new e_triangle(D,C,H,&EM);
	e_triangle *right2 = new e_triangle(G,C,H,&EM);
	
	e_triangle *front1 = new e_triangle(B,D,F,&EM);
	e_triangle *front2 = new e_triangle(H,D,F,&EM);
	
	//e_triangle *back1 = new e_triangle(E,G,A,&EM); //this is how you rotate a face
	//e_triangle *back2 = new e_triangle(C,G,A,&EM);
	//e_triangle *back1 = new e_triangle(A,E,C,&EM);
	//e_triangle *back2 = new e_triangle(G,E,C,&EM);
	e_triangle *back1 = new e_triangle(E,A,G,&EM);
	e_triangle *back2 = new e_triangle(C,A,G,&EM);
	
	e_selection selAll = EM.selectAll();
	selAll.scale(vec3(0.5,0.5,0.5),scale);
	selAll.uv_project_box();
	selAll.rebuildRmodel();
	return selAll.rm[2];
}