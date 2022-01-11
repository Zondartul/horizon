#include "editmodel.h"
#include "globals.h"
#include "paint.h"
#include "simplemath.h"
#include <algorithm>
#include <map>
#include "stringUtils.h"
using std::map;
//MACROS

//x - e_vertex/edge/triangle *element
//n - e_selection neighbor
//v - vector<e_v/e/t *> elements
//I - for-loop iterator name
//c - code to run

//generic vector macros
#define contains(_v,_x)					(std::find((_v).begin(), (_v).end(), (_x)) != (_v).end())
#define push_if_not_contains(_v,_x)		{auto _I = std::find((_v).begin(), (_v).end(), (_x)); if(_I == (_v).end()){(_v).push_back(_x);}}
#define remove_if_contains(_v,_x)		{auto _I = std::find((_v).begin(), (_v).end(), (_x)); if(_I != (_v).end()){(_v).erase(_I);}} 
#define for_all(_v,_I,c)				{auto &v = _v; for(auto _I = (_v).begin(); _I != (_v).end(); _I++){c;}}

#define ne(I) (I)->neighbors_essential
#define nd(I) (I)->neighbors_direct
//iterate over the elements of *this
#define for_all_elements(_I,c){			\
	for_all(verts,_I,c);				\
	for_all(edges,_I,c);				\
	for_all(tris,_I,c);					\
}
	
//iterate over the elements of something else
#define for_all_ne(_x,_I,c){			\
	auto &x = _x;						\
	auto &n = (_x)->neighbors_essential;\
	for_all(n.verts,_I,c);				\
	for_all(n.edges,_I,c);				\
	for_all(n.tris,_I,c);				\
}	
#define for_all_nd(_x,_I,c){			\
	auto &x = _x;						\
	auto &n = (x)->neighbors_direct;	\
	for_all(n.verts,_I,c);				\
	for_all(n.edges,_I,c);				\
	for_all(n.tris,_I,c);				\
}	
#define for_all_n(_x,_I,c){				\
	for_all_ne(_x,_I,c);				\
	for_all_nd(_x,_I,c);				\
}

//todo: add equality operator for edges and triangles
e_vertex::e_vertex(vec3 pos, e_model *EM):neighbors_essential(EM),neighbors_direct(EM){
	this->pos = pos;
	EM->verts.push_back(this);
}

e_edge::e_edge(e_vertex *A, e_vertex *B, e_model *EM):neighbors_essential(EM),neighbors_direct(EM){
	neighbors_essential.verts.push_back(A); 
	neighbors_essential.verts.push_back(B);
	EM->edges.push_back(this);
}

e_triangle::e_triangle(e_vertex *A, e_vertex *B, e_vertex *C, e_model *EM):neighbors_essential(EM),neighbors_direct(EM){
	neighbors_essential.verts.push_back(A); 
	neighbors_essential.verts.push_back(B);
	neighbors_essential.verts.push_back(C);
	EM->tris.push_back(this);
}

bool tri_contains_edge(e_triangle *T, e_edge *E){
	bool hasE1 = contains(T->neighbors_essential.verts,E->neighbors_essential.verts[0]);
	bool hasE2 = contains(T->neighbors_essential.verts,E->neighbors_essential.verts[1]);
	return hasE1 && hasE2;
}

void tri_add_edges(e_triangle *T, e_model *EM){
	//printf("adding edges from tri %p to %p\n",T,EM);
	e_vertex *A = T->neighbors_essential.verts[0];
	e_vertex *B = T->neighbors_essential.verts[1];
	e_vertex *C = T->neighbors_essential.verts[2];
	
	new e_edge(A,B,EM);
	new e_edge(B,C,EM);
	new e_edge(C,A,EM);
}

void e_face::recalcEdges(){
	e_model *EM = neighbors_essential.EM;
	int n=0;
	for_all(neighbors_essential.tris,T,{
		n++;
		tri_add_edges(*T,EM); //hack, due to neighbor population not being implemented yet
	});
	//printf("added %d tris\n",n);
	int i=0,j=0,k=0;
	neighbors_essential.edges.clear();
	for_all(EM->edges,E,{	//for all edges in the model
		i++;
		//is it one of the triangle edges?
		for_all(neighbors_essential.tris,T,{
			j++;
			if(tri_contains_edge(*T,*E)){
				k++;
				//have we added this edge before?
				if(contains(neighbors_essential.edges,*E)){
					//printf("edge removed\n");
					//that means we've seen it in one of the triangles.
					//it's an internal edge so we remove it
					//(assumes sheet topology for the face)
					remove_if_contains(neighbors_essential.edges,*E);
				}else{
					//printf("edge added\n");
					//else we add it. If we don't see it again then it's external.
					neighbors_essential.edges.push_back(*E);
				}
			}
		});
	});
	//printf("recalcEdges: i:%d, j:%d, k:%d, final edges: %d\n",i,j,k,neighbors_essential.edges.size());
}

e_face::e_face(vector<e_triangle*> tris, e_model *EM):neighbors_essential(EM),neighbors_direct(EM){
	int j = 0;
	for_all(tris,T,{
		//printf("tri %d -> face\n", j++);
		neighbors_essential.tris.push_back(*T);
	});
	recalcEdges();
	EM->faces.push_back(this);
}

#define rns_helper_add_normal(x) \
	if(!vertex_normals.count(x)){\
		vertex_normals[x] = vec3(0,0,0);\
	}\
	vertex_normals[x] += face_normal;

void e_selection::recalculateNormalsSmooth(){
	map<e_vertex*,vec3> vertex_normals;
	for_all(tris,T,{
		e_vertex *A = (*T)->neighbors_essential.verts[0];
		e_vertex *B = (*T)->neighbors_essential.verts[1];
		e_vertex *C = (*T)->neighbors_essential.verts[2];
		vec3 face_normal = cross((B->pos-A->pos),(C->pos-A->pos));
		(*T)->face_normal = face_normal;
		rns_helper_add_normal(A);
		rns_helper_add_normal(B);
		rns_helper_add_normal(C);
	});
	for(auto I = vertex_normals.begin(); I != vertex_normals.end(); I++){
		vec3 N = I->second;
		N = normalizeSafe(N);
		I->second = N;
	}
	for_all(tris,T,{
		e_vertex *A = (*T)->neighbors_essential.verts[0];
		e_vertex *B = (*T)->neighbors_essential.verts[1];
		e_vertex *C = (*T)->neighbors_essential.verts[2];
		(*T)->vert_normals.clear();
		(*T)->vert_normals.push_back(vertex_normals[A]);
		(*T)->vert_normals.push_back(vertex_normals[B]);
		(*T)->vert_normals.push_back(vertex_normals[C]);
	});
}

struct vec3orderer{
	bool operator()(const vec3 &A, const vec3 &B) const{
		if(A.x < B.x){return true;}
		if(B.x < A.x){return false;}
		if(A.y < B.y){return true;}
		if(B.y < A.y){return false;}
		if(A.z < B.z){return true;}
		if(B.z < A.z){return false;}
		return false;
	}
};
//merge vertices with the same position
void e_selection::removeDuplicates(){
	printf("remove duplicates. Before: %d verts\n", EM->verts.size());
	map<vec3,e_vertex*,vec3orderer> vertmap;
	set<e_vertex*> dupes;
	// first pass: remap all references
	for_all(tris,T,{
		for_all((*T)->neighbors_essential.verts,I,{
			vec3 pos = (*I)->pos;
			if(vertmap.count(pos)){
				if(vertmap[pos] != *I){
					dupes.insert(*I);
					*I = vertmap[pos];
				}
			}else{
				vertmap[pos] = *I;
			}
		});
	});
	for_all(edges,E,{
		for_all((*E)->neighbors_essential.verts,I,{
			vec3 pos = (*I)->pos;
			if(vertmap.count(pos)){
				if(vertmap[pos] != *I){
					dupes.insert(*I);
					*I = vertmap[pos];
				}
			}else{
				vertmap[pos] = *I;
			}
		});
	});
	//second pass: actually delete the duplicates
	for(auto I = EM->verts.begin(); I != EM->verts.end();){
		if(dupes.count(*I)){I = EM->verts.erase(I);}else{I++;}
	}
	for(auto I = verts.begin(); I != verts.end();){
		if(dupes.count(*I)){I = verts.erase(I);}else{I++;}
	}
	printf("After: %d verts\n", EM->verts.size());
}

//lololol macros
#define rrm_helper0(rm, v, color, UV, normal)  {	\
		vec3 pos = (v)->pos;				\
		(rm)->vertices->push_back(pos);	\
		(rm)->colors->push_back(color);	\
		(rm)->uvs->push_back(UV);		\
		(rm)->normals->push_back(normal);\
	}									
#define rrm_helper(rm, vn, color,UV,normal)	{										\
		rrm_helper0((rm), (*I)->neighbors_essential.verts[(vn)],(color),(UV),(normal));	\
	}

	
void e_selection::rebuildRmodel(){
	printf("rebuild Rmodel\n");
	if(!rm[0]){
		rm[0] = new rmodel();
		rm[1] = new rmodel();
		rm[2] = new rmodel();
	}
	rm[0]->clear();
	rm[1]->clear();
	rm[2]->clear();
	//construct RM 0 as points
	int i = 0, j = 0, k = 0;
	printf("construct rm[0] as points\n");
	for_all(verts,I,
		rrm_helper0(rm[0],*I,colorVerts,vec2(0,0),vec3(0,0,1));
		i++;
	);
	printf("added %d verts\n",i);
	//construct RM 1 as edges	
	i = 0; j = 0; k = 0;
	printf("construct rm[1] as edges\n");
//leave this for when face mode is on
//(in face mode, each face is treated as an approximately flat (smooth) surface.
/*	for_all(faces,F,{
		i++;
		for_all((*F)->neighbors_essential.edges,I,
			j++;
			if((*I)->neighbors_essential.verts.size() != 2){error("what a weird edge (%d vertices)\n",(*I)->neighbors_essential.verts.size());}
			rrm_helper(rm[1],0,colorEdges,vec2(0,0),vec3(0,0,1));
			rrm_helper(rm[1],1,colorEdges,vec2(1,0),vec3(0,0,1));
		);}
	);
	printf("added %d faces, %d edges\n",i,j);
*/
	for_all(tris,F,{
		if((*F)->neighbors_essential.edges.size() != 3){error("what a weird triangle (%d edges)\n",(*F)->neighbors_essential.edges.size());}
		i++;
		for_all((*F)->neighbors_essential.edges,I,
			j++;
			if((*I)->neighbors_essential.verts.size() != 2){error("what a weird edge (%d vertices)\n",(*I)->neighbors_essential.verts.size());}
			rrm_helper(rm[1],0,colorEdges,vec2(0,0),vec3(0,0,1));
			rrm_helper(rm[1],1,colorEdges,vec2(1,0),vec3(0,0,1));
		);
	});
	printf("added %d tris, %d edges\n",i,j);
	//construct RM 2 as triangles
	i = 0; j = 0; k = 0;
	printf("construct rm[2] as triangles\n");
	for_all(tris,I,{
		i++;
		if((*I)->neighbors_essential.verts.size() != 3){
			error("what a weird triangle (%d vertices)\n",
			(*I)->neighbors_essential.verts.size());
		}
		vec3 VA = ne(*I).verts[0]->pos;
		vec3 VB = ne(*I).verts[1]->pos;
		vec3 VC = ne(*I).verts[2]->pos;
		vec3 vnA;
		vec3 vnB;
		vec3 vnC;
		//printf("v_n.size() = %d -> ",(*I)->vert_normals.size());
		if((*I)->vert_normals.size() == 3){
			//printf("own normals\n");
			vnA = (*I)->vert_normals[0];
			vnB = (*I)->vert_normals[1];
			vnC = (*I)->vert_normals[2];
		}else{
			//printf("flat normals\n");
			vec3 vn = cross(VB-VA,VC-VA);
			vnA = vnB = vnC = vn;
		}
		vec3 colA = colorTris;
		vec3 colB = colorTris;
		vec3 colC = colorTris;
		
		if(rainbowTris){
			colA = colA * vec3(1.f,0.f,0.f);
			colB = colB * vec3(0.f,1.f,0.f);
			colC = colC * vec3(0.f,0.f,1.f);	
		}
		vec2 UV1 = vec2(0,0);
		vec2 UV2 = vec2(1,0);
		vec2 UV3 = vec2(0,1);
		if((*I)->uvs.size() >= 3){
			UV1 = (*I)->uvs[0];
			UV2 = (*I)->uvs[1];
			UV3 = (*I)->uvs[2];
		}
		rrm_helper(rm[2],0,colA,UV1,vnA);
		rrm_helper(rm[2],1,colB,UV2,vnB);
		rrm_helper(rm[2],2,colC,UV3,vnC);
		
	});
	printf("added %d triangles\n",i);
	rm[0]->finalize();
	rm[1]->finalize();
	rm[2]->finalize();
	setLayer(loadLayer);
	uploadRmodel(rm[0]);
	uploadRmodel(rm[1]);
	uploadRmodel(rm[2]);
}

void e_selection::render(){
	//setColor({255,255,255});
	//setColoring(true); //per-vertex coloring
	
	//setTransparency(true);setAlpha(0.5f*255.f);
	//setColor({64,0,128});
	setRenderMode(3);
	setAlpha(alphaTris);
	drawRmodel(rm[2]);
	//setTransparency(false); setAlpha(255.f);
	
	//setDepthTest(false);
	//setColor({0,0,0});
	//setLineWidth(2.f);
	setRenderMode(2);
	setAlpha(alphaEdges);
	drawRmodel(rm[1]);
	//setLineWidth(1.f);
	//setDepthTest(true);
	
	//setPointSize(3);
	//setColor({255,255,255});
	setRenderMode(1);
	setAlpha(alphaVerts);
	drawRmodel(rm[0]);
}

e_selection::e_selection(e_model *EM){this->EM = EM;}

void e_selection::addElements(e_selection sel2){
	for_all(sel2.verts,I,	push_if_not_contains(verts,*I));
	for_all(sel2.edges,I,	push_if_not_contains(edges,*I));
	for_all(sel2.tris,I,	push_if_not_contains(tris,*I));
}
void e_selection::removeElements(e_selection sel2){
	for_all(sel2.verts,I,	remove_if_contains(verts,*I));
	for_all(sel2.edges,I,	remove_if_contains(edges,*I));
	for_all(sel2.tris,I,	remove_if_contains(tris,*I));
}
void e_selection::clear(){
	verts.clear();
	edges.clear();
	tris.clear();
}

e_selection e_selection::getVerts(){
	e_selection sel(EM);
	sel.verts = verts;
	return sel;
}
e_selection e_selection::getEdges(){
	e_selection sel(EM);
	sel.edges = edges;
	return sel;
}
e_selection e_selection::getTris(){
	e_selection sel(EM);
	sel.tris = tris;
	return sel;
}

#define getImpVertsHelper(x) {e_vertex *V = x; if(!dups.count(V)){dups.insert(V);sel.verts.push_back(V);}}

e_selection e_selection::getImplicitVerts(){
	if(!EM){error("no EM\n");}
	e_selection sel(EM);
	set<e_vertex*> dups;
	for(auto I = verts.begin(); I != verts.end(); I++){
		getImpVertsHelper(*I);
	}
	for(auto I = edges.begin(); I != edges.end(); I++){
		getImpVertsHelper((*I)->neighbors_essential.verts[0]);
		getImpVertsHelper((*I)->neighbors_essential.verts[1]);
	}
	for(auto I = tris.begin(); I != tris.end(); I++){
		getImpVertsHelper((*I)->neighbors_essential.verts[0]);
		getImpVertsHelper((*I)->neighbors_essential.verts[1]);
		getImpVertsHelper((*I)->neighbors_essential.verts[2]);
	}
	return sel;
}

e_selection e_selection::getImplicitEdges(){
	if(!EM){error("no EM\n");}
	e_selection sel(EM);
	set<e_edge*> dups;
	e_selection verts = getImplicitVerts();
	for(auto I = EM->edges.begin(); I != EM->edges.end(); I++){
		auto &n = (*I)->neighbors_essential;
		if(n.verts.size() != 2){error("edge vert size = %d\n",n.verts.size());}
		e_vertex *v1 = (*I)->neighbors_essential.verts[0];
		e_vertex *v2 = (*I)->neighbors_essential.verts[1];
		if(contains(verts.verts,v1) && contains(verts.verts,v2)){
			if(!dups.count(*I)){dups.insert(*I); sel.edges.push_back(*I);}
		}
	}
	return sel;
}

e_selection e_selection::getImplicitTris(){
	if(!EM){error("no EM\n");}
	e_selection sel(EM);
	set<e_triangle*> dups;
	e_selection verts = getImplicitVerts();
	for(auto I = EM->tris.begin(); I != EM->tris.end(); I++){
		e_vertex *v1 = (*I)->neighbors_essential.verts[0];
		e_vertex *v2 = (*I)->neighbors_essential.verts[1];
		e_vertex *v3 = (*I)->neighbors_essential.verts[2];
		if(contains(verts.verts,v1) && contains(verts.verts,v2) && contains(verts.verts,v3)){
			if(!dups.count(*I)){dups.insert(*I); sel.tris.push_back(*I);}
		}
	}
	return sel;
}
/*
e_selection e_selection::getNeighborsEssential(){
	e_selection sel(EM);
	for_all(verts,I,
		for_all((*I)->neighbors_essential.verts,J,sel.verts.push_back(*J))
		for_all((*I)->neighbors_essential.edges,J,sel.edges.push_back(*J))
		for_all((*I)->neighbors_essential.tris,J,sel.tris.push_back(*J))
	);
	for_all(edges,I,
		for_all((*I)->neighbors_essential.verts,J,sel.verts.push_back(*J))
		for_all((*I)->neighbors_essential.edges,J,sel.edges.push_back(*J))
		for_all((*I)->neighbors_essential.tris,J,sel.tris.push_back(*J))
	);
	for_all(tris,I,
		for_all((*I)->neighbors_essential.verts,J,sel.verts.push_back(*J))
		for_all((*I)->neighbors_essential.edges,J,sel.edges.push_back(*J))
		for_all((*I)->neighbors_essential.tris,J,sel.tris.push_back(*J))
	);
	return sel;
}
e_selection e_selection::getNeighborsDirect(){
	e_selection sel(EM);
	for_all(verts,I,
		for_all((*I)->neighbors_direct.verts,J,sel.verts.push_back(*J))
		for_all((*I)->neighbors_direct.edges,J,sel.edges.push_back(*J))
		for_all((*I)->neighbors_direct.tris,J,sel.tris.push_back(*J))
	);
	for_all(edges,I,
		for_all((*I)->neighbors_direct.verts,J,sel.verts.push_back(*J))
		for_all((*I)->neighbors_direct.edges,J,sel.edges.push_back(*J))
		for_all((*I)->neighbors_direct.tris,J,sel.tris.push_back(*J))
	);
	for_all(tris,I,
		for_all((*I)->neighbors_direct.verts,J,sel.verts.push_back(*J))
		for_all((*I)->neighbors_direct.edges,J,sel.edges.push_back(*J))
		for_all((*I)->neighbors_direct.tris,J,sel.tris.push_back(*J))
	);
	return sel;
}
*/

e_selection e_selection::getNeighborsEssential(){
	if(!EM){error("no EM\n");}
	e_selection sel(EM);
	for_all(verts,K,
		for_all((*K)->neighbors_essential.verts,J,	push_if_not_contains(sel.verts,*J))
		for_all((*K)->neighbors_essential.edges,J,	push_if_not_contains(sel.edges,*J))
		for_all((*K)->neighbors_essential.tris,J,	push_if_not_contains(sel.tris,*J))
	);                                             
	for_all(edges,K,                                
		for_all((*K)->neighbors_essential.verts,J,	push_if_not_contains(sel.verts,*J))
		for_all((*K)->neighbors_essential.edges,J,	push_if_not_contains(sel.edges,*J))
		for_all((*K)->neighbors_essential.tris,J,	push_if_not_contains(sel.tris,*J))
	);                                             
	for_all(tris,K,                                 
		for_all((*K)->neighbors_essential.verts,J,	push_if_not_contains(sel.verts,*J))
		for_all((*K)->neighbors_essential.edges,J,	push_if_not_contains(sel.edges,*J))
		for_all((*K)->neighbors_essential.tris,J,	push_if_not_contains(sel.tris,*J))
	);
	return sel;
}
e_selection e_selection::getNeighborsDirect(){
	if(!EM){error("no EM\n");}
	e_selection sel(EM);
	for_all(verts,K,
		for_all((*K)->neighbors_direct.verts,J,push_if_not_contains(sel.verts,*J))
		for_all((*K)->neighbors_direct.edges,J,push_if_not_contains(sel.edges,*J))
		for_all((*K)->neighbors_direct.tris,J,push_if_not_contains(sel.tris,*J))
	);            
	for_all(edges,K,
		for_all((*K)->neighbors_direct.verts,J,push_if_not_contains(sel.verts,*J))
		for_all((*K)->neighbors_direct.edges,J,push_if_not_contains(sel.edges,*J))
		for_all((*K)->neighbors_direct.tris,J,push_if_not_contains(sel.tris,*J))
	);            
	for_all(tris, K,
		for_all((*K)->neighbors_direct.verts,J,push_if_not_contains(sel.verts,*J))
		for_all((*K)->neighbors_direct.edges,J,push_if_not_contains(sel.edges,*J))
		for_all((*K)->neighbors_direct.tris,J,push_if_not_contains(sel.tris,*J))
	);
	return sel;
}


vec3 e_selection::center(){
	vec3 CR = {0,0,0};
	float N = verts.size()+edges.size()+tris.size();
	for(auto I = verts.begin(); I != verts.end(); I++){
		CR += (*I)->pos/N;
	}
	for(auto I = edges.begin(); I != edges.end(); I++){
		vec3 A = (*I)->neighbors_essential.verts[0]->pos;
		vec3 B = (*I)->neighbors_essential.verts[1]->pos;
		CR += ((A+B)/2.f) / (float)N;
	}
	for(auto I = tris.begin(); I != tris.end(); I++){
		vec3 A = (*I)->neighbors_essential.verts[0]->pos;
		vec3 B = (*I)->neighbors_essential.verts[1]->pos;
		vec3 C = (*I)->neighbors_essential.verts[2]->pos;
		CR += ((A+B+C)/3.f) / (float)N;
	}
	return CR;
}

void e_selection::deleteAll(){
	if(!EM){error("no EM\n");}
	for(auto I = verts.begin(); I != verts.end(); I++){EM->verts.remove(*I);}
	for(auto I = edges.begin(); I != edges.end(); I++){EM->edges.remove(*I);}
	for(auto I = tris.begin(); I != tris.end(); I++){EM->tris.remove(*I);}
	verts.clear();	//we shouldn't keep pointers to things that have been deleted
	edges.clear();
	tris.clear();
	EM->recalculateNeighbors();
}

void e_selection::move(vec3 offset){
	if(edges.size() == 0 && tris.size() == 0){
		for(auto I = verts.begin(); I != verts.end(); I++){
			(*I)->pos += offset*(*I)->selection_weight;
		}
	}else{
		getImplicitVerts().move(offset);
	}
}
void e_selection::rotate(vec3 center, vec3 axis, float angle){
	if(edges.size() == 0 && tris.size() == 0){
		for(auto I = verts.begin(); I != verts.end(); I++){
			vec3 pos = (*I)->pos;
			pos = glm::rotate(pos-center,angle*d2r,axis)+center;
			(*I)->pos = pos;
		}
	}else{
		getImplicitVerts().rotate(center,axis,angle);
	}
} 
void e_selection::scale(vec3 center, float scale){
	if(edges.size() == 0 && tris.size() == 0){
		for(auto I = verts.begin(); I != verts.end(); I++){
			vec3 pos = (*I)->pos;
			pos = (pos-center)*scale+center;
			(*I)->pos = pos;
		}
	}else{
		getImplicitVerts().scale(center,scale);
	}
}
void e_selection::scale(vec3 center, vec3 scale){
	if(edges.size() == 0 && tris.size() == 0){
		for(auto I = verts.begin(); I != verts.end(); I++){
			vec3 pos = (*I)->pos;
			pos = (pos-center)*scale+center;
			(*I)->pos = pos;
		}
	}else{
		getImplicitVerts().scale(center,scale);
	}
}

#define map_left	{	UV.x =-V.x, UV.y = -V.z;}
#define map_right	{ 	UV.x = V.x, UV.y = -V.z;}
#define map_front	{ 	UV.x = V.y, UV.y = -V.z;}
#define map_back	{ 	UV.x =-V.y, UV.y = -V.z;}
#define map_top		{	UV.x =-V.x; UV.y =  V.y;}
#define map_bottom	{	UV.x =-V.x; UV.y = -V.y;}

void e_selection::uv_project_box(vec3 origin,float scale){
	if(!scale){printf("uv_project with zero scale!\n");}
	vec3 center = this->center();
	//int cnt1 = 0; int cnt2 = 0;
	for(auto I = tris.begin(); I != tris.end(); I++){
		//printf("tri %d\n",cnt1++);
		(*I)->uvs.clear();
		vec3 V1 = ne(*I).verts[0]->pos;
		vec3 V2 = ne(*I).verts[1]->pos;
		vec3 V3 = ne(*I).verts[2]->pos;
		//vec3 dV = normalize(cross(V2-V1,V3-V1));
		//printf("dv = %s\n",toString(dV).c_str());
		vec3 dV = normalizeSafe(cross(normalizeSafe(V2-V1),normalizeSafe(V3-V1))); //triangle flat normal
		float dotx = fabs(dot(dV,vec3(1,0,0)));
		float doty = fabs(dot(dV,vec3(0,1,0)));
		float dotz = fabs(dot(dV,vec3(0,0,1)));
		bool xmin = ((dotx < doty) && (dotx < dotz));
		bool ymin = ((doty < dotx) && (doty < dotz));
		bool zmin = ((dotz < dotx) && (dotz < doty));
		bool xmax = ((dotx > doty) && (dotx > dotz));
		bool ymax = ((doty > dotx) && (doty > dotz));
		bool zmax = ((dotz > doty) && (dotz > dotx));
		//printf("min xyz: %d %d %d, max xyz: %d %d %d\n",xmin,ymin,zmin,xmax,ymax,zmax);
		if(!xmax and !ymax and !zmax){
			printf("no max: dotx = %f, doty = %f, dotz = %f, dV = %s\n",dotx,doty,dotz,toString(dV).c_str());
		}
		for(auto J = ne(*I).verts.begin(); J != ne(*I).verts.end(); J++){
			//printf("vert %d\n",cnt2++);
			vec2 UV = vec2(0,0);
			vec3 V = (*J)->pos-origin;
			V = V*scale;
			
			if(zmax){map_top;}
			if(xmax){map_front;}
			if(ymax){map_left;}
			// if((abs(dV.y) > abs(dV.x))&&(abs(dV.y) > abs(dV.z))){
				// if(dV.y > 0){  UV.x	=-V.x, UV.y = -V.z;}//left
				// else		{ UV.x = V.x, UV.y = -V.z;}//right
			// }	
			// else if((abs(dV.x) > abs(dV.y))&&(abs(dV.x) > abs(dV.z))){
				// if(dV.x > 0){ UV.x = V.y, UV.y = -V.z;}//front
				// else		{  UV.x =-V.y, UV.y = -V.z;}//back
			// }
			// else {
				// if(dV.z > 0){   UV.x =-V.x; UV.y =  V.y;}//top
				// else		{UV.x =-V.x; UV.y = -V.y;}//bottom
			// }
			
			(*I)->uvs.push_back(UV);
		}
	}
}

void e_selection::uv_scale(float scale){
	for(auto I = tris.begin(); I != tris.end(); I++){
		for(auto J = (*I)->uvs.begin(); J != (*I)->uvs.end(); J++){
			vec2 UV = *J;
			UV.x = UV.x*scale;
			UV.y = UV.y*scale;
			*J = UV;
		}
	}
}

#define mapcontains(map,x) map.count(x)

e_vertex *mapVertex(e_vertex *v, 
					map<e_vertex*,e_vertex*> &vmap, 
					e_model *EM){
	if(!v){error("mV: no v\n");}
	if(!EM){error("mV: no EM\n");}
	e_vertex *v2;
	if(mapcontains(vmap,v)){
		v2 = vmap[v];
	}else{
		v2 = new e_vertex(v->pos,EM);
		vmap[v] = v2;
	}
	return v2;
}

e_edge *mapEdge(e_edge *e, 
				map<e_edge*,e_edge*> &emap, 
				map<e_vertex*,e_vertex*> &vmap, 
				e_model *EM){	
	if(!e){error("mE: no e\n");}
	if(!EM){error("mE: no EM\n");}
	e_edge *e2;
	if(mapcontains(emap,e)){
		e2 = emap[e];
	}else{
		e_vertex *v0 = e->neighbors_essential.verts[0];
		e_vertex *v1 = e->neighbors_essential.verts[1];
		e_vertex *v0_2 = mapVertex(v0,vmap,EM);
		e_vertex *v1_2 = mapVertex(v1,vmap,EM);
		e2 = new e_edge(v0_2,v1_2,EM);
		emap[e] = e2;
	}
	return e2;
}

e_triangle *mapTriangle(e_triangle *t, 
						map<e_triangle*,e_triangle*> &tmap, 
						map<e_vertex*,e_vertex*> &vmap, 
						e_model *EM){
	if(!t){error("mT: no t\n");}
	if(!EM){error("mT: no EM\n");}
	e_triangle *t2;
	if(mapcontains(tmap,t)){
		t2 = tmap[t];
	}else{
		e_vertex *v0 = t->neighbors_essential.verts[0];
		e_vertex *v1 = t->neighbors_essential.verts[1];
		e_vertex *v2 = t->neighbors_essential.verts[2];
		e_vertex *v0_2 = mapVertex(v0,vmap,EM);
		e_vertex *v1_2 = mapVertex(v1,vmap,EM);
		e_vertex *v2_2 = mapVertex(v2,vmap,EM);
		t2 = new e_triangle(v0_2,v1_2,v2_2,EM);
		tmap[t] = t2;
	}
	return t2;
}

e_face *mapFace(e_face *f, 
				map<e_face*,e_face*> &fmap, 
				map<e_triangle*,e_triangle*> &tmap, 
				map<e_vertex*,e_vertex*> &vmap,
				e_model *EM){
	if(!f){error("mF: no f\n");}
	if(!EM){error("mF: no EM\n");}
	e_face *f2;
	if(mapcontains(fmap,f)){
		f2 = fmap[f];
	}else{
		vector<e_triangle*> vt1 = f->neighbors_essential.tris;
		vector<e_triangle*> vt2;
		for(auto I = vt1.begin(); I != vt1.end(); I++){
			e_triangle *t1 = *I;
			e_triangle *t2 = mapTriangle(t1,tmap,vmap,EM);
			vt2.push_back(t2);
		}
		f2 = new e_face(vt2,EM);
		fmap[f] = f2;
	}
	return f2;
}
//inserts copies of all selected elements into a different e_model,
//then returns inserted elements as a new selection.
e_selection e_selection::addTo(e_model *EM){
	map<e_vertex*,e_vertex*> vmap;
	map<e_edge*,e_edge*> emap;
	map<e_triangle*,e_triangle*> tmap;
	map<e_face*,e_face*> fmap;
	e_selection sel(EM);
	int j = 0;
	for(auto I = verts.begin(); I != verts.end(); I++){
		//printf("adding vert %d\n",j++);
		e_vertex *v = *I;
		e_vertex *v2 = mapVertex(v,vmap,EM);
		sel.verts.push_back(v2);
	}
	j = 0;
	for(auto I = edges.begin(); I != edges.end(); I++){
		//printf("adding edge %d\n",j++);
		e_edge *e = *I;
		e_edge *e2 = mapEdge(e,emap,vmap,EM);
		sel.edges.push_back(e2);
	}
	j = 0;
	for(auto I = tris.begin(); I != tris.end(); I++){
		//printf("adding tri %d\n",j++);
		e_triangle *t = *I;
		e_triangle *t2 = mapTriangle(t,tmap,vmap,EM);
		sel.tris.push_back(t2);
	}
	j = 0;
	 //faces are bork?
	for(auto I = faces.begin(); I != faces.end(); I++){
		//printf("adding face %d\n",j++);
		e_face *f = *I;
		e_face *f2 = mapFace(f,fmap,tmap,vmap,EM);
		sel.faces.push_back(f2);
	}
	
	return sel;
}

//returns [0]: minimal set: selected tris, selected edges not in tris, and selected verts not in tris or edges
//returns [1]: maximal set: selected tris, selected edges + those in tris, and selected verts + those in tris + those in edges
vector<e_selection> e_selection::getNormalized(){
	e_selection vertsFromTris = getTris().getImplicitVerts();
	e_selection edgesFromTris = getTris().getImplicitEdges(); 
	e_selection vertsFromEdges = getEdges().getImplicitVerts();
	
	e_selection minTris = getTris();
	e_selection minEdges = getEdges();
	minEdges.removeElements(edgesFromTris);
	e_selection minVerts = getVerts();
	minVerts.removeElements(vertsFromTris);
	minVerts.removeElements(vertsFromEdges);
	
	e_selection maxTris = getTris();
	e_selection maxEdges = getEdges();
	maxEdges.addElements(edgesFromTris);
	e_selection maxVerts = getVerts();
	maxVerts.addElements(vertsFromTris);
	maxVerts.addElements(vertsFromEdges);
	
	e_selection minSel(EM); //minimal selection
	minSel.tris = minTris.tris;
	minSel.edges = minEdges.edges;
	minSel.verts = minVerts.verts;
	
	e_selection maxSel(EM); //maximal selection
	maxSel.tris = maxTris.tris;
	maxSel.edges = maxEdges.edges;
	maxSel.verts = maxVerts.verts;
	
	return {minSel, maxSel};
}

//extrudes <selected>
//returns [0]: newly created elements
//returns [1]: elements connecting the existing elements to new elements.

vector<e_selection> e_selection::extrude(){
	e_selection skirt(EM); //elements in the selection that are connected to anything outside the selection
	//1) normalize selection into tris, non-tri edges, and non-tri non-edge vertices
	vector<e_selection> normed = getNormalized();
	e_selection minimal = normed[0];
	e_selection maximal = normed[1];
	
	//2) calculate the skirt
	for_all(minimal.tris,I,{
		auto &ne = (*I)->neighbors_essential;
		auto &nd = (*I)->neighbors_direct;
		for_all(ne.verts,J,{
			auto &nd2 = (*J)->neighbors_direct;
			for_all(nd2.verts,K,{
				if(!contains(maximal.verts,*K)){
					skirt.verts.push_back(*J);
				}
			});
		});
		for_all(nd.edges,J,{
			auto &nd2 = (*J)->neighbors_direct;
			for_all(nd2.tris,K,{
				if(!contains(maximal.tris,*K)){
					skirt.edges.push_back(*J);
				}
			});
		});
	});
	
	
	vector<e_selection> vsel;
	vsel.push_back(skirt);
	vsel.push_back(skirt);
	return vsel;
}

/*
vector<e_selection> e_selection::extrude(){
	if(!EM){error("no EM\n");}
	vector<e_selection> vsel;
	e_selection selnew(EM);
	e_selection selcon(EM);
	//if(!EM){error("e_selection has no associated e_model\n");}
	if((verts.size()>0) + (edges.size()>0) + (tris.size()>0) > 1){error("can only extrude either verts, OR edges, OR tris");}
	
	for(auto I = verts.begin(); I != verts.end(); I++){
		e_vertex *oV = *I;
		e_vertex *nV = new e_vertex(oV->pos,EM);
		selnew.verts.push_back(nV);
		
		e_edge *nE = new e_edge(oV,nV,EM);
		selcon.edges.push_back(nE);
	}
	for(auto I = edges.begin(); I != edges.end(); I++){
		e_vertex *oV1 = ne(*I).verts[0];
		
		e_vertex *nV1 = new e_vertex(oV1->pos,EM);
		selnew.verts.push_back(nV1);
		
		e_vertex *oV2 = ne(*I).verts[1];
		e_vertex *nV2 = new e_vertex(oV2->pos,EM);
		selnew.verts.push_back(nV2);
		
		e_edge *nE1 = new e_edge(nV1,nV2,EM);
		selnew.edges.push_back(nE1);
		
		e_edge *nE2 = new e_edge(oV1,nV1,EM);
		selcon.edges.push_back(nE2);
		
		e_edge *nE3 = new e_edge(oV2,nV2,EM);
		selcon.edges.push_back(nE3);
		
		e_edge *nE4 = new e_edge(oV1,nV2,EM);
		selcon.edges.push_back(nE4);
		
		e_triangle *nT1 = new e_triangle(oV1,nV1,nV2,EM);
		selcon.tris.push_back(nT1);
		
		e_triangle *nT2 = new e_triangle(oV1,oV2,nV2,EM);
		selcon.tris.push_back(nT2);
	}
	vsel.push_back(selnew);
	vsel.push_back(selcon);
	EM->sanityCheck();
	EM->recalculateNeighbors();
	return vsel;
}
*/

/*
e_selection e_selection::merge(vec3 pos){
	e_selection sel(EM);
	//1) create a new vertex as pos
	//2) grab all implied vertices
	//3) grab all their neighbors 
	//4) grab the neighbors who do not have neighbors outside of (2)
	//5) erase all (4)
	//6) grab all implied vertices from (3) that are not in (2)
	//7) replace all vertices (6) ... wat
	/////// with the new vertex (1) ..?
	
	// take all the stuff that is connected to the selected stuff, make it connected to the new vertex instead, and erase all the selected stuff.
	// also triangles will get broken, so new edges will need to be made
	
	//stuff:
	//1) connected to only unselected (cou)
	//2) connected to selected and unselected (csu)
	//3) connected to only selected (cos)
	
	//1) connected to selected (cs)
	e_selection sel_iv = getImplicitVerts();
	e_selection sel2(EM);
	for(auto I = sel_iv.verts.begin(); I != sel_iv.verts.end(); I++){
		sel2.addElements((*I)->neighbors);
	}
	//2) remove 'cos' from 'cs' to get 'csu', and also remove 'cos' from EM
	for(auto I = sel2.edges.begin(); I != sel2.edges.end(); I++){
		e_vertex *v1 = (*I)->neighbors.verts[0];
		e_vertex *v2 = (*I)->neighbors.verts[1];
		if(contains(sel_iv.verts,v1) && contains(sel_iv.verts,v2)){
			EM->edges.remove(*I);
			I = sel2.edges.erase(I);
		}
	}
	for(auto I = sel2.tris.begin(); I != sel2.tris.end(); I++){
		e_vertex *v1 = (*I)->neighbors.verts[0];
		e_vertex *v2 = (*I)->neighbors.verts[1];
		e_vertex *v3 = (*I)->neighbors.verts[2];
		if(contains(sel_iv.verts,v1) && contains(sel_iv.verts,v2) && contains(sel_iv.verts,v3)){
			EM->tris.remove(*I);
			I = sel2.tris.erase(I);
		}
	}
	//3) in 'csu', replace selected with new vertex (possibly combine this with step 2 cause we will remove all sel anyway, but watch out for unselected implicit edges)
	e_vertex *nV = new e_vertex();
	nV->pos = pos;
	EM->verts.push_back(nV);
	
	for(auto I = sel2.edges.begin(); I != sel2.edges.end(); I++){
		e_vertex *v1 = (*I)->neighbors.verts[0];
		e_vertex *v2 = (*I)->neighbors.verts[1];
		if(contains(sel_iv.verts,v1)){(*I)->neighbors.verts[0] = nV;}
		if(contains(sel_iv.verts,v2)){(*I)->neighbors.verts[1] = nV;}
	}
	for(auto I = sel2.tris.begin(); I != sel2.tris.end(); I++){
		e_vertex *v1 = (*I)->neighbors.verts[0];
		e_vertex *v2 = (*I)->neighbors.verts[1];
		e_vertex *v3 = (*I)->neighbors.verts[2];
		if(contains(sel_iv.verts,v1)){(*I)->neighbors.verts[0] = nV;}
		if(contains(sel_iv.verts,v2)){(*I)->neighbors.verts[1] = nV;}
		if(contains(sel_iv.verts,v3)){(*I)->neighbors.verts[2] = nV;}
	}
	//4) remove all selected
	deleteAll();
	sel.verts.push_back(nV);
	EM->recalculateNeighbors();
	return sel;
}
*/

e_selection e_model::selectAll(){
	e_selection sel(this);
	for_all(verts,I,sel.verts.push_back(*I));
	for_all(edges,I,sel.edges.push_back(*I));
	for_all(tris,I,sel.tris.push_back(*I));
	for_all(faces,I,sel.faces.push_back(*I));
	return sel;
}

//--------------- OLD SETUP
//a neighbor is any element that needs to be modified if this element is removed/replaced
//neighbor types:
//	essential					makes up the definition of this element. subset of direct. Only vertices.
//	direct			contained	contains or is contained by this element. subset of indirect.
//  indirect		sharing		shares elements with this one. 
//	remote						indirect neighbors of a neighbor.

//--------------- NEW SETUP
//				 _______________________________________
//  			|		face	|  tri	| edge  |vertex |
//				|_______________|_______|_______|_______|
//definition	|   	tris  	| verts | verts |   pos	|
//				|---------------|-------|-------|-------|
//				|   tris		| edges |		|		|		  
//parts			|edges (outside)| verts | verts |		|
//				|verts (outside)|		|		|		|
//				|---------------|-------|-------|-------|
//touching		|	             faces					|
//				|				 tris					|
//				|				 edges					|
//				|			verts (through edges)		|
//				|_______________________________________|
//
//	definition - elements that are necessary and sufficient to define this element
//  parts - elements that are contained within this element (though likely shared)
//  touching - elements that touch this one (excluding parts).
//				of them, verts - verts of touching edges, that are opposite of part verts
//


//  face.definition - triangles
//  face.parts - triangles, outside edges, outside vertices
//  triangle.definition - vertices
//  triangle.parts - edges, vertices
//  edge.definition - vertices
//  edge.parts - vertices
//  

//right now lets deal with only the contained elements
//old:
//vnv - remote
//vne - direct
//vnt - direct
//env - essential
//ene - direct
//ent - indirect
//tnv - essential
//new:
//vnv - contained (empty)
//vne - contained
//vnt - contained
//env - contained (A,B)
//ene - contained (empty)
//ent - contained
//tnv - contained (A,B,C)
//tne - contained (AB,BC,CA)
//tnt - contained (empty)

//localized repair:
//loop:
//sel1 = all affected points
//sel2 = sel1+sel1.indirect_neighbors //neighbors of a selection are all the indirect neighbors minus the selection
// (aka sel2 = sel1.indirect_neighbors_all;) //including the selection
//if(repair sel1){sel1 = sel2; goto loop;}
//
//algo repairs affected area, and if changes were made, expands the area.
//HOWEVER, funcs should be made such that repair is never necessary.



void e_model::stripNeighbors(){
	//removes non-essential connectivity info (aka all neighbors except essential)
	//essential info is:
	//	vertex.pos
	//	edge.neighbors.verts[0,1]
	//	triangle.neighbors.verts[0,1,2]
	//verts, edges and tris are stored separately because they can all be floating.
	for_all_elements(I,(*I)->neighbors_direct.clear();)
	sanityCheck();
}

void e_model::repair(){
	stripNeighbors();
	checkDegenerate(true);
	recalculateNeighbors();
}
	
#define cdup_helper(v,code){		\
	set<typeof(v.front())> dups;	\
	for_all(v,I,{					\
		if(!dups.count(*I)){   	    \
			dups.insert(*I);        \
		}else{                      \
			code;                   \
			if(repair){             \
				I = v.erase(I);     \
			}else{return repneeded;}\
		}                           \
	});                             \
}

const char *e_model::checkDuplicates(bool repair){
	const char *repneeded = 0;
	cdup_helper(verts,repneeded = "duplicate vertex reference");
	cdup_helper(edges,repneeded = "duplicate edge reference");
	cdup_helper(tris,repneeded = "duplicate triangle reference");
	return repneeded;
}

#define cdp_helper() \
	if(!contains(v,*_J)){	\
		repneeded = true;	\
		if(repair){_J = (x)->n.v.erase(_J);}	\
	}
	
const char *e_model::checkDanglingPointers(bool repair){
	//if repair, repairs the model until there are no dangling pointers.
	//else, only checks if repair is necessary.
	//returns true if the model had dangling pointers.
	
	//safe against dangling neighbor pointers
	//may create (expose) degenerate elements
	//removes neighbor refs, does not remove actual elements
	
	//for each element:
	//	for each neighbor:
	//		check if this element is in the model
	//			if exists, do nothing
	//			if not exists, remove from neighbors
	const char *repneeded = 0;
	for_all_elements(I,{
		for_all_n(*I,J,
			if(!contains(v,*J)){
				repneeded = "dangling pointer";
				if(repair){J = v.erase(J);}	//v is the vector through which we iterate using J.
			}
		);
	});
	printf("dangling = %s\n",repneeded);
	return repneeded;
}
										
const char *e_model::checkDegenerate(bool repair){
	//if repair, repairs the model until it is not degenerate.
	//else, only checks if repair is necessary.
	//returns true if model was degenerate
	//should be safe against dangling neighbor pointers
	//model must be stripped prior to check
	const char *repneeded = 0;
	const char *didstuff = 0;
	cd_loop:
	//at the start of each run, removeDanglingPointers
	//	(we may have started with some, or created them last iteration)
	const char *duplicate = checkDuplicates(repair);
	if(duplicate && !repair){return duplicate;}
	const char *dangling = checkDanglingPointers(repair);
	if(dangling && !repair){return dangling;}
	for_all_elements(I,
		//degenerate elements:
		//element whose neighbor is itself (remove neighbor)
			for_all_n(*I,_I,					
				if((void*)*_I == (void*)x){	//hax because too lazy for propper typing			
					repneeded = "degenerate (1.self_neighbor)";
					if(repair){					
						_I = v.erase(_I);		
					}							
				}								
			);
			
	);
	//element with too many essential verts (remove extras)
	//element with not enough essential verts (remove element)
	#define cd_es_shrink(x,_v,n){					\
		auto &v = (x)->neighbors_essential._v;		\
		if(v.size() > n){	\
			repneeded = "degenerate (2.excess verts)";						\
			if(repair){(v).resize(n);}				\
		}											\
	}
	#define cd_es_min(_I,n){						\
		if((*_I)->neighbors_essential.verts.size() < n){	\
			repneeded = "degenerate (3.not enough verts)";						\
			if(repair){								\
				_I = v.erase(_I);					\
			}										\
		}											\
	}
	for_all(verts,I,
		cd_es_shrink(*I,verts,0);
		cd_es_shrink(*I,edges,0);
		cd_es_shrink(*I,tris,0);
	);
	for_all(edges,I,
		cd_es_shrink(*I,verts,2);
		cd_es_shrink(*I,edges,0);
		cd_es_shrink(*I,tris,0);
		cd_es_min(I,2);
	);
	for_all(tris,I,
		cd_es_shrink(*I,verts,3);
		cd_es_shrink(*I,edges,0);
		cd_es_shrink(*I,tris,0);
		cd_es_min(I,3);
	);
	//edge two repeated verts (if not stripped, replace edge with point)(else remove edge, keep points)
	for_all(edges,I,
		auto &v2 = (*I)->neighbors_essential.verts;
		if(v2[0] == v2[1]){
			repneeded = "degenerate (4.edge w/repeated verts)";
			if(repair){
				I = v.erase(I);
			}
		}
	);	
	//triangle  (replace tri with point)
	//triangle whose two endpoitns are the same (replace tri with edge)
	//triangle implies edges?
	//meh, just remove the traingle
	for_all(tris,I,
		auto &v2 = (*I)->neighbors_essential.verts;
		if((v2[0] == v2[1]) ||(v2[1] == v2[2]) ||(v2[2] == v2[0])){
			repneeded = "degenerate (5.tri w/repeated verts)";
			if(repair){
				I = v.erase(I);
			}
		}
	);
	//if anything done this time, check again
	if(repneeded && repair){didstuff = repneeded; repneeded = 0; goto cd_loop;}
	printf("degenerate = %s\n",didstuff);
	return didstuff;
	//error("removeDegenerates not implemented");
}

//if(v1 == v2){error("degenerate edge");}	//an edge can be connected to two points sharing the same pos, but it can't be connected to the same point twice
void e_model::recalculateNeighbors(){
	//0.1) essential: (env), (tnv)
	//0.2) strip neighbors
	stripNeighbors();
	checkDegenerate(true);
	//1) rebuild vertex.neighbors.edges (vne) from essential (env)
		for(auto I = edges.begin(); I != edges.end(); I++){
			//an edge is a neighbor of a vertex if it connects to it.
			e_vertex *v1 = ne(*I).verts[0];
			e_vertex *v2 = ne(*I).verts[1];
			nd(v1).edges.push_back(*I);
			nd(v2).edges.push_back(*I);
		}
	//2) rebuild vertex.neighbors.verts (vnv) from (vne) and (env)
		for(auto I = edges.begin(); I != edges.end(); I++){
			//two vertices are neighbors if there is an edge between them.
			e_vertex *v1 = ne(*I).verts[0];
			e_vertex *v2 = ne(*I).verts[1];
			nd(v1).verts.push_back(v2);
			nd(v2).verts.push_back(v1);		
		}
	//3) rebuild edges.neighbors.edges (ene) from (vne) and (env)
		for(auto I = verts.begin(); I != verts.end(); I++){
			for(auto J = nd(*I).edges.begin(); J != nd(*I).edges.end(); J++){
				//all the edges that connect to a given vertex are neighbors to each other.
				for(auto K = nd(*I).edges.begin(); K != J; K++){} //skip the edges we've already looked at
				for(auto K = nd(*I).edges.begin(); K != nd(*I).edges.end(); K++){
					if(K != J){
						nd(*J).edges.push_back(*K);
					}
				}
			}
		}
	//4) rebuild vertex.neighbors.tris (vnt) from essential (tnv)
		for(auto I = tris.begin(); I != tris.end(); I++){
			//a tri is a neighbor of a vertex if it connects to it.
			e_vertex *v1 = ne(*I).verts[0];
			e_vertex *v2 = ne(*I).verts[1];
			e_vertex *v3 = ne(*I).verts[2];
			nd(v1).tris.push_back(*I);
			nd(v2).tris.push_back(*I);
			nd(v3).tris.push_back(*I);
		}
	//5,6) rebuild tris.neighbors.edges (tne) and edges.neighbors.tris (ent) from (tnv) and (vne)
		for(auto I = tris.begin(); I != tris.end(); I++){
			//an edge is a neighbor of a tri if it connects to two of it's vertices.
			e_vertex *v1 = ne(*I).verts[0];
			e_vertex *v2 = ne(*I).verts[1];
			e_vertex *v3 = ne(*I).verts[2];
			//we could just iterate over every edge in the world but N^2 is no bueno
			#define v_edgecheck(v)												\
			for(auto J = nd(v).edges.begin(); J != nd(v).edges.end(); J++){		\
				e_vertex *va = ne(*J).verts[0];									\
				e_vertex *vb = ne(*J).verts[1];									\
				int m1 = (va == v1);											\
				int m2 = (va == v2);											\
				int m3 = (va == v3);											\
				int m4 = (vb == v1);											\
				int m5 = (vb == v2);											\
				int m6 = (vb == v3);											\
				/*two matches, assuming the model is not degenerate*/			\
				if(m1+m2+m3+m4+m5+m6 == 2){										\
					nd(*I).edges.push_back(*J);									\
					nd(*J).tris.push_back(*I);									\
				}																\
			}
			v_edgecheck(v1);
			v_edgecheck(v2);
			v_edgecheck(v3);
		}
	sanityCheck();
	//7) rebuild tris.neighbors.tris (tnt) from (tne) and (ent)
	//actually, not sure what tnt is.
	//(vnv) v 2
	//(vne) v 1
	//(vnt) v 4
	//
	//(env) e 
	//(ene) v 3
	//(ent) v 6
	//
	//(tnv) e 
	//(tne) v 5
	//(tnt) v 7
	
	// env +-> vne +-> vnv 
	//	   +-------|---^ 
	//     |       +-> ene
	//     +-------|---^
	//             +-> tne -> tnt
	// tnv +-------|---^      ^
	//     +->vnt  +-> ent ---+
	//     +-----------^
}

void e_model::sanityCheck(){
	const char *status = checkDegenerate(false);
	if(status){error("e_model error (%s)\n",status);}
}

rmodel *e_model::getRmodel(int mode){
	//printf("%p.getRmodel(%d)\n",this,mode);
	e_selection selAll = selectAll();
	selAll.rebuildRmodel();
	return selAll.rm[mode];
}

string toString(e_model *EM){
	if(EM){
		return fstring("(v:%d, e:%d, t:%d, f:%d)",EM->verts.size(), EM->edges.size(), EM->tris.size(), EM->faces.size());
	}else{
		return "(null)";
	}
}






