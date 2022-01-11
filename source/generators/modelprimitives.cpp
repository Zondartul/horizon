#include "modelprimitives.h"
#include "rmodel.h"
#include "editmodel.h"
#include "simplemath.h"
#include "stringUtils.h"

//
// A B
// C D
//
// E F
// G H

e_model *generateBox(vec3 scale, float texscale){
	e_model *EM = new e_model();;
	
	e_vertex *A = new e_vertex({0,0,0},EM);
	e_vertex *B = new e_vertex({1,0,0},EM);
	e_vertex *C = new e_vertex({0,1,0},EM);
	e_vertex *D = new e_vertex({1,1,0},EM);
	e_vertex *E = new e_vertex({0,0,1},EM);
	e_vertex *F = new e_vertex({1,0,1},EM);
	e_vertex *G = new e_vertex({0,1,1},EM);
	e_vertex *H = new e_vertex({1,1,1},EM);
	
	e_edge *AB = new e_edge(A,B,EM);
	e_edge *BD = new e_edge(B,D,EM);
	e_edge *DC = new e_edge(D,C,EM);
	e_edge *CA = new e_edge(C,A,EM);
	
	e_edge *EF = new e_edge(E,F,EM);
	e_edge *FH = new e_edge(F,H,EM);
	e_edge *HG = new e_edge(H,G,EM);
	e_edge *GE = new e_edge(G,E,EM);
	
	e_edge *AE = new e_edge(A,E,EM);
	e_edge *BF = new e_edge(B,F,EM);
	e_edge *CG = new e_edge(C,G,EM);
	e_edge *DH = new e_edge(D,H,EM);
	
	e_triangle *top1 = new e_triangle(E,F,G,EM);
	e_triangle *top2 = new e_triangle(G,F,H,EM);
	e_face *top = new e_face({top1,top2},EM);
	
	e_triangle *bottom1	= new e_triangle(C,B,A,EM);
	e_triangle *bottom2 = new e_triangle(D,B,C,EM);
	e_face *bottom = new e_face({bottom1,bottom2},EM);
	
	e_triangle *left1 = new e_triangle(A,B,E,EM);
	e_triangle *left2 = new e_triangle(E,B,F,EM);
	e_face *left = new e_face({left1,left2},EM);
	
	e_triangle *right1 = new e_triangle(D,C,H,EM);
	e_triangle *right2 = new e_triangle(H,C,G,EM);
	e_face *right = new e_face({right1,right2},EM);
	
	e_triangle *front1 = new e_triangle(B,D,F,EM);
	e_triangle *front2 = new e_triangle(F,D,H,EM);
	e_face *front = new e_face({front1,front2},EM);
	
	//e_triangle *back1 = new e_triangle(E,G,A,&EM); //this is how you rotate a face
	//e_triangle *back2 = new e_triangle(C,G,A,&EM);
	//e_triangle *back1 = new e_triangle(A,E,C,&EM);
	//e_triangle *back2 = new e_triangle(G,E,C,&EM);
	e_triangle *back1 = new e_triangle(G,A,E,EM);
	e_triangle *back2 = new e_triangle(C,A,G,EM);
	e_face *back = new e_face({back1,back2},EM);
	
	EM->recalculateNeighbors();
	e_selection selAll = EM->selectAll();
	selAll.move(vec3(-0.5,-0.5,-0.5));
	//selAll.scale(vec3(0.5,0.5,0.5),scale);
	selAll.scale(vec3(0,0,0),scale);
	vec3 center = selAll.center();
	vec3 dv = scale/2.f;
	dv.y *= -1;
	vec3 corner = center+dv;
	selAll.recalculateNormalsFlat();
	selAll.uv_project_box(corner,texscale);//-scale);
	//selAll.rebuildRmodel();
	//return selAll.rm[2];
	return EM;
}

e_model *generateCyllinder(float r, float h, int numsides, float texscale){
	printf("generateCyllinder(r = %f, h = %f, numsides = %d, texscale = %f\n",r,h,numsides,texscale);
	assert(numsides != 0);
	e_model *EM = new e_model();
	//int numsides = 16;
	vector<e_vertex*> verts;
	vector<e_triangle*> toptris;
	vector<e_triangle*> bottomtris;
	for(int I = 0; I < numsides; I++){
		float dt = 360.0*d2r/numsides;
		e_vertex *A = new e_vertex(rotate(vec3(r,0,h),dt*I,vec3(0,0,1)),EM);
		e_vertex *B = new e_vertex(rotate(vec3(r,0,0),		dt*I,vec3(0,0,1)),EM);
		verts.push_back(A);
		verts.push_back(B);
	}
	e_vertex *O = new e_vertex(vec3(0,0,0),EM); //origin
	e_vertex *T = new e_vertex(vec3(0,0,h),EM); //top
	for(int I = 0; I < numsides; I++){
		int idx1 = I;
		int idx2 = (I+1)%numsides;
		e_vertex *A1 = verts[idx1*2];
		e_vertex *B1 = verts[idx1*2+1];
		e_vertex *A2 = verts[idx2*2];
		e_vertex *B2 = verts[idx2*2+1];
		e_triangle *top = new e_triangle(T,A2,A1,EM);//(T,A1,A2,EM);
		e_triangle *bottom = new e_triangle(B2,B1,O,EM);//(O,B1,B2,EM);
		e_triangle *side1 = new e_triangle(A2,A1,B1,EM);//(A1,B1,A2,EM);
		e_triangle *side2 = new e_triangle(B1,B2,A2,EM);//(B1,A2,B2,EM);
		new e_face({side1,side2},EM);
		toptris.push_back(top);
		bottomtris.push_back(bottom);
	}
	new e_face(toptris, EM);
	new e_face(bottomtris, EM);
	
	{
		printf("before1: num tris = %d\n",EM->tris.size());
	}
	
	EM->recalculateNeighbors();
	e_selection selAll = EM->selectAll();
	selAll.recalculateNormalsSmooth();
	vec3 center = selAll.center();
	vec3 dv = vec3(r,r,h)/2.f;
	dv.y *= -1;
	vec3 corner = center+dv;
	selAll.uv_project_box(corner,texscale);
	
	{
		printf("before2: num tris = %d\n",EM->tris.size());
	}
	
	return EM;
}

e_model *generateCone(float r, float h, int numsides, float texscale){
	assert(numsides != 0);
	e_model *EM = new e_model();
	//int numsides = 16;
	vector<e_vertex*> verts;
	//vector<e_triangle*> toptris;
	vector<e_triangle*> bottomtris;
	for(int I = 0; I < numsides; I++){
		float dt = 360.0*d2r/numsides;
		e_vertex *A = new e_vertex(rotate(vec3(r,0,0),		dt*I,vec3(0,0,1)),EM);
		verts.push_back(A);
	}
	e_vertex *O = new e_vertex(vec3(0,0,0),EM); //origin
	e_vertex *T = new e_vertex(vec3(0,0,h),EM); //top
	for(int I = 0; I < numsides; I++){
		int idx1 = I;
		int idx2 = (I+1)%numsides;
		e_vertex *A1 = verts[idx1];
		e_vertex *A2 = verts[idx2];
		e_triangle *top = new e_triangle(A1,A2,T,EM);//(T,A1,A2,EM);
		e_triangle *bottom = new e_triangle(A2,A1,O,EM);//(O,A1,A2,EM);
		new e_face({top},EM);
		bottomtris.push_back(bottom);
	}
	new e_face(bottomtris, EM);
		
	EM->recalculateNeighbors();
	e_selection selAll = EM->selectAll();
	selAll.recalculateNormalsSmooth();
	vec3 center = selAll.center();
	vec3 dv = vec3(r,r,h)/2.f;
	dv.y *= -1;
	vec3 corner = center+dv;
	selAll.uv_project_box(corner,texscale);
		
	return EM;
}

e_model *generateSphere(float r, int numsides, int numslices, float texscale){
	assert(numsides != 0);
	assert(numslices != 0);
	e_model *EM = new e_model();
	//int numsides = 16;
	//int numslices = 8;
	//float r = scale.x;
	vector<vector<e_vertex*>> slices;
	vector<e_triangle*> toptris;
	vector<e_triangle*> bottomtris;
	float dt = 360.0*d2r/numsides;
	float dp = 180.0*d2r/numslices;
	for(int I = 0; I < numslices; I++){
		vector<e_vertex*> verts;
		for(int J = 0; J < numsides; J++){
			vec3 pos = vec3(r,0,0);
			pos = rotate(pos,dp*I-90.f*d2r,vec3(0,1,0));
			pos = rotate(pos,dt*J,vec3(0,0,1));
			e_vertex *A = new e_vertex(pos,EM);
			verts.push_back(A);
		}
		//printf("slices[%d]: %d verts\n",slices.size(),verts.size());
		slices.push_back(verts);
	}
	e_vertex *BT = new e_vertex(vec3(0,0,-r),EM); //bottom
	e_vertex *T = new e_vertex(vec3(0,0,r),EM); //top
	//make top tris
	for(int I = 0; I < numsides; I++){
		int idx1 = I;
		int idx2 = (I+1)%numsides;
		vector<e_vertex*> verts = slices[0];
		e_vertex *A1 = verts[idx1];
		e_vertex *A2 = verts[idx2];
		
		e_triangle *top = new e_triangle(T,A1,A2,EM);
		toptris.push_back(top);
		new e_face(toptris, EM);
	}
	//make bottom tris
	for(int I = 0; I < numsides; I++){
		int idx1 = I;
		int idx2 = (I+1)%numsides;
		vector<e_vertex*> verts = slices[numslices-1];
		e_vertex *B1 = verts[idx1];
		e_vertex *B2 = verts[idx2];
		
		e_triangle *bottom = new e_triangle(BT,B1,B2,EM);
		bottomtris.push_back(bottom);
		new e_face(bottomtris, EM);
	}
	//make side tris
	for(int I = 0; I < numslices-1; I++){
		vector<e_vertex*> verts1 = slices[I];
		vector<e_vertex*> verts2 = slices[I+1];
		for(int J = 0; J < numsides; J++){ 
			int idx1 = J;
			int idx2 = (J+1)%numsides;
			e_vertex *A1 = verts1[idx1];
			e_vertex *B1 = verts1[idx2];
			e_vertex *A2 = verts2[idx1];
			e_vertex *B2 = verts2[idx2];
			e_triangle *side1 = new e_triangle(A1,B1,A2,EM);
			e_triangle *side2 = new e_triangle(B1,A2,B2,EM);
			new e_face({side1,side2},EM);
		}
	}
	//printf("result e_model: %s\n",toCString(EM));
	
	EM->recalculateNeighbors();
	e_selection selAll = EM->selectAll();
	selAll.recalculateNormalsSmooth();
	vec3 center = selAll.center();
	vec3 dv = vec3(r,r,r)/2.f;
	dv.y *= -1;
	vec3 corner = center+dv;
	selAll.uv_project_box(corner,texscale);
	
	return EM;
}

e_model *generateSheet(int numx, int numy, vec3 scale, float texscale){
	assert(numx != 0);
	assert(numy != 0);
	e_model *EM = new e_model();
	float xstep = scale.x/numx;
	float ystep = scale.y/numy;
	printf("xstep = %f, ystep = %f\n",xstep,ystep);
	vector<e_vertex*> verts;
	for(int I = 0; I < numx+1; I++){
		for(int J = 0; J < numy+1; J++){
			vec3 pos = vec3(xstep*I,ystep*J,0);
			e_vertex *V = new e_vertex(pos,EM);
			verts.push_back(V);
			//printf("vert(%d,%d) = %s\n",I,J, toCString(pos));
		}
	}
	for(int I = 0; I < numx; I++){
		for(int J = 0; J < numy; J++){
			e_vertex *A = verts[(I+0)*(numy+1)+(J+0)];
			e_vertex *B = verts[(I+1)*(numy+1)+(J+0)];
			//e_vertex *C = verts[(I+1)*(numy+1)+(J+1)];
			//e_vertex *D = verts[(I+0)*(numy+1)+(J+1)];
			e_vertex *C = verts[(I+0)*(numy+1)+(J+1)];
			e_vertex *D = verts[(I+1)*(numy+1)+(J+1)];
			
			e_triangle *T1 = new e_triangle(A,B,C,EM);
			//e_triangle *T2 = new e_triangle(C,D,A,EM);
			e_triangle *T2 = new e_triangle(D,C,B,EM);
		}
	}
	
	EM->recalculateNeighbors();
	e_selection selAll = EM->selectAll();
	selAll.recalculateNormalsSmooth();
	vec3 center = selAll.center();
	vec3 dv = scale/2.f;
	dv.y *= -1;
	vec3 corner = center+dv;
	selAll.uv_project_box(corner,texscale);
	
	return EM;
}

e_model *generatePlane(vec3 normal, float offset, vec2 size, float texscale){
	e_model *EM = new e_model();
	
	vec3 planeX = normalizeSafe(cross(normal,vec3(0,0,1)));
	if(normal == vec3(0,0,1)){planeX = vec3(1,0,0);}
	if(normal == vec3(0,0,-1)){planeX = vec3(-1,0,0);}
	vec3 planeY = normalizeSafe(cross(normal,planeX));
	vec3 voffset = offset*normal;
	
	vec3 A = planeX*(-0.5f*size.x) + planeY*(-0.5f*size.y) + voffset;
	vec3 B = planeX*(+0.5f*size.x) + planeY*(-0.5f*size.y) + voffset;
	vec3 C = planeX*(-0.5f*size.x) + planeY*(+0.5f*size.y) + voffset;
	vec3 D = planeX*(+0.5f*size.x) + planeY*(+0.5f*size.y) + voffset; 
	
	e_vertex *vA = new e_vertex(A,EM);
	e_vertex *vB = new e_vertex(B,EM);
	e_vertex *vC = new e_vertex(C,EM);
	e_vertex *vD = new e_vertex(D,EM);
	
	e_triangle *T1 = new e_triangle(vA,vB,vC, EM);
	e_triangle *T2 = new e_triangle(vC,vB,vD, EM);
	
	EM->recalculateNeighbors();
	e_selection selAll = EM->selectAll();
	selAll.recalculateNormalsSmooth();
	vec3 center = selAll.center();
	selAll.uv_project_box(center,texscale);
	
	return EM;
}

















