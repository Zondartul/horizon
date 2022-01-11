#include "editmodel.h"
#include "globals.h"
#include "paint.h"
#include "simplemath.h"
#include <algorithm>

//MACROS

//x - e_vertex/edge/triangle *element
//n - e_selection neighbor
//v - vector<e_v/e/t *> elements
//I - for-loop iterator name
//c - code to run

//generic vector macros
#define contains(_v,_x)					(std::find((_v).begin(), (_v).end(), (_x)) != (_v).end())
#define push_if_not_contains(_v,_x)		{auto _I = std::find((_v).begin(), (_v).end(), (_x)); if(_I != (_v).end()){(_v).push_back(_x);}}
#define remove_if_contains(_v,_x)		{auto _I = std::find((_v).begin(), (_v).end(), (_x)); if(_I != (_v).end()){(_v).erase(_I);}} 
#define for_all(_v,_I,c)				{auto &v = _v; for(auto _I = (_v).begin(); _I != (_v).end(); _I++){c;}}

//iterate over the elements of *this
//#define for_all_verts(I,c)			for_all(verts,I,c);
//#define for_all_edges(I,c)			for_all(edges,I,c);
//#define for_all_tris(I,c)			for_all(tris,I,c);
#define for_all_elements(_I,c){			\
	for_all(verts,_I,c);				\
	for_all(edges,_I,c);				\
	for_all(tris,_I,c);					\
}
	
//iterate over the elements of something else
//#define for_all_n_verts(x,n,I,c)	for_all(x.n.verts,I,c);
//#define for_all_n_edges(x,n,I,c)	for_all(x.n.edges,I,c);
//#define for_all_n_tris(x,n,I,c)		for_all(x.n.tris,I,c);	
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
e_vertex::e_vertex(vec3 pos){this->pos = pos;}

e_edge::e_edge(e_vertex *A, e_vertex *B){
	neighbors_essential.verts.push_back(A); 
	neighbors_essential.verts.push_back(B);
}

e_triangle::e_triangle(e_vertex *A, e_vertex *B, e_vertex *C){
	neighbors_essential.verts.push_back(A); 
	neighbors_essential.verts.push_back(B);
	neighbors_essential.verts.push_back(C);
}
//lololol macros
#define rrm_helper0(rm, v, color)  {	\
		vec3 pos = (v)->pos;				\
		(rm)->vertices->push_back(pos);	\
		(rm)->colors->push_back(color);	\
	}									
#define rrm_helper(rm, vn, color)	{										\
		rrm_helper0((rm), (*I)->neighbors_essential.verts[(vn)],(color));	\
	}

void e_selection::rebuildRmodel(){
	if(!rm[0]){
		rm[0] = new rmodel();
		rm[1] = new rmodel();
		rm[2] = new rmodel();
	}
	rm[0]->clear();
	rm[1]->clear();
	rm[2]->clear();
	//for(auto I = verts.begin(); I != verts.end(); I++){
	for_all(verts,I,
		rrm_helper0(rm[0],*I,colorVerts);
		//vec3 pos = (*I)->pos;
		//rm[0]->vertices->push_back(pos);
		//rm[0]->colors->push_back(colorVerts);
	//}
	);
	//for(auto I = edges.begin(); I != edges.end(); I++){
	for_all(edges,I,
		if((*I)->neighbors_essential.verts.size() != 2){error("what a weird edge (%d vertices)\n",(*I)->neighbors_essential.verts.size());}
		rrm_helper(rm[1],0,colorEdges);
		rrm_helper(rm[1],1,colorEdges);
		//vec3 A = (*I)->neighbors_essential.verts[0]->pos;
		//vec3 B = (*I)->neighbors_essential.verts[1]->pos;
		//rm[1]->vertices->push_back(A);
		//rm[1]->colors->push_back(colorEdges);
		//rm[1]->vertices->push_back(B);
		//rm[1]->colors->push_back(colorEdges);
	//}
	);
	//for(auto I = tris.begin(); I != tris.end(); I++){
	for_all(tris,I,
		if((*I)->neighbors_essential.verts.size() != 3){error("what a weird triangle (%d vertices)\n",(*I)->neighbors_essential.verts.size());}
		rrm_helper(rm[2],0,colorTris);
		rrm_helper(rm[2],1,colorTris);
		rrm_helper(rm[2],2,colorTris);
		//vec3 A = (*I)->neighbors_essential.verts[0]->pos;
		//vec3 B = (*I)->neighbors_essential.verts[1]->pos;
		//vec3 C = (*I)->neighbors_essential.verts[2]->pos;
		//rm[2]->vertices->push_back(A);
		// rm[2]->colors->push_back(colorTris);
		// rm[2]->vertices->push_back(B);
		// rm[2]->colors->push_back(colorTris);
		// rm[2]->vertices->push_back(C);
		// rm[2]->colors->push_back(colorTris);
	//}
	);
	rm[0]->finalize();
	rm[1]->finalize();
	rm[2]->finalize();
	uploadRmodel(rm[0]);
	uploadRmodel(rm[1]);
	uploadRmodel(rm[2]);
}

void e_selection::render(){
	setColor({255,255,255});
	setColoring(true); //per-vertex coloring
	//setTransparency(true);
	//setAlpha(0.5f*255.f);
	setRenderMode(3);
	drawRmodel(rm[2]);
	//setTransparency(false);
	//setAlpha(255.f);
	
	//setDepthTest(false);
	setRenderMode(2);
	drawRmodel(rm[1]);
	//setDepthTest(true);
	
	//setPointSize(3);
	setRenderMode(1);
	drawRmodel(rm[0]);
}

e_selection::e_selection(e_model *EM){this->EM = EM;}

void e_selection::addElements(e_selection sel2){
	for_all(sel2.verts,I,	push_if_not_contains(verts,*I));
	for_all(sel2.edges,I,	push_if_not_contains(edges,*I));
	for_all(sel2.tris,I,	push_if_not_contains(tris,*I));
	//for(auto I = sel2.verts.begin(); I != sel2.verts.end(); I++){push_if_not_contains(verts,*I);}
	//for(auto I = sel2.edges.begin(); I != sel2.edges.end(); I++){push_if_not_contains(edges,*I);}
	//for(auto I = sel2.tris.begin(); I != sel2.tris.end(); I++)	{push_if_not_contains(tris,*I);}
}
void e_selection::removeElements(e_selection sel2){
	for_all(sel2.verts,I,	remove_if_contains(verts,*I));
	for_all(sel2.edges,I,	remove_if_contains(edges,*I));
	for_all(sel2.tris,I,	remove_if_contains(tris,*I));
	//for(auto I = sel2.verts.begin(); I != sel2.verts.end(); I++){remove_if_contains(verts,*I);}
	//for(auto I = sel2.edges.begin(); I != sel2.edges.end(); I++){remove_if_contains(edges,*I);}
	//for(auto I = sel2.tris.begin(); I != sel2.tris.end(); I++)	{remove_if_contains(tris,*I);}
}
void e_selection::clear(){
	verts.clear();
	edges.clear();
	tris.clear();
}

e_selection e_selection::getVerts(){
	e_selection sel;
	sel.EM = EM;
	sel.verts = verts;
	return sel;
}
e_selection e_selection::getEdges(){
	e_selection sel;
	sel.EM = EM;
	sel.edges = edges;
	return sel;
}
e_selection e_selection::getTris(){
	e_selection sel;
	sel.EM = EM;
	sel.tris = tris;
	return sel;
}

#define getImpVertsHelper(x) {e_vertex *V = x; if(!dups.count(V)){dups.insert(V);sel.verts.push_back(V);}}

e_selection e_selection::getImplicitVerts(){
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
	e_selection sel(EM);
	set<e_edge*> dups;
	e_selection verts = getImplicitVerts();
	for(auto I = EM->edges.begin(); I != EM->edges.end(); I++){
		e_vertex *v1 = (*I)->neighbors_essential.verts[0];
		e_vertex *v2 = (*I)->neighbors_essential.verts[1];
		if(contains(verts.verts,v1) && contains(verts.verts,v2)){
			if(!dups.count(*I)){dups.insert(*I); sel.edges.push_back(*I);}
		}
	}
	return sel;
}

e_selection e_selection::getImplicitTris(){
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
			(*I)->pos += offset;
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

//extrudes <selected>
//returns [0]: newly created elements
//returns [1]: elements connecting the existing elements to new elements.
/*
vector<e_selection> e_selection::extrude(){
	vector<e_selection> vsel;
	e_selection selnew;
	e_selection selcon;
	if(!EM){error("e_selection has no associated e_model\n");}
	if((verts.size()>0) + (edges.size()>0) + (tris.size()>0) > 1){error("can only extrude either verts, OR edges, OR tris");}
	
	selnew.EM = EM;
	selcon.EM = EM;
	
	for(auto I = verts.begin(); I != verts.end(); I++){
		e_vertex *oV = *I;
		e_vertex *nV = new e_vertex();
		nV->pos = oV->pos;
		EM->verts.push_back(nV);
		selnew.verts.push_back(nV);
		
		e_edge *nE = new e_edge();
		nE->neighbors.verts.push_back(oV);
		nE->neighbors.verts.push_back(nV);
		EM->edges.push_back(nE);
		selcon.edges.push_back(nE);
	}
	for(auto I = edges.begin(); I != edges.end(); I++){
		e_vertex *oV1 = (*I)->neighbors.verts[0];
		e_vertex *nV1 = new e_vertex();
		nV1->pos = oV1->pos;
		EM->verts.push_back(nV1);
		selnew.verts.push_back(nV1);
		
		e_vertex *oV2 = (*I)->neighbors.verts[1];
		e_vertex *nV2 = new e_vertex();
		nV2->pos = oV2->pos;
		EM->verts.push_back(nV2);
		selnew.verts.push_back(nV2);
		
		e_edge *nE1 = new e_edge();
		nE1->neighbors.verts.push_back(nV1);
		nE1->neighbors.verts.push_back(nV2);
		EM->edges.push_back(nE1);
		selnew.edges.push_back(nE1);
		
		e_edge *nE2 = new e_edge();
		nE2->neighbors.verts.push_back(oV1);
		nE2->neighbors.verts.push_back(nV1);
		EM->edges.push_back(nE2);
		selcon.edges.push_back(nE2);
		
		e_edge *nE3 = new e_edge();
		nE3->neighbors.verts.push_back(oV2);
		nE3->neighbors.verts.push_back(nV2);
		EM->edges.push_back(nE3);
		selcon.edges.push_back(nE3);
		
		e_triangle *nT1 = new e_triangle();
		nT1->neighbors.verts.push_back(oV1);
		nT1->neighbors.verts.push_back(nV1);
		nT1->neighbors.verts.push_back(nV2);
		EM->tris.push_back(nT1);
		selcon.tris.push_back(nT1);
		
		e_triangle *nT2 = new e_triangle();
		nT2->neighbors.verts.push_back(oV1);
		nT2->neighbors.verts.push_back(oV2);
		nT2->neighbors.verts.push_back(nV2);
		EM->tris.push_back(nT2);
		selcon.tris.push_back(nT2);
	}
	vsel.push_back(selnew);
	vsel.push_back(selcon);
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
	e_selection sel;
	sel.EM = this;
	for_all(verts,I,sel.verts.push_back(*I));
	for_all(edges,I,sel.edges.push_back(*I));
	for_all(tris,I,sel.tris.push_back(*I));
	//for(auto I = verts.begin(); I != verts.end(); I++){sel.verts.push_back(*I);}
	//for(auto I = edges.begin(); I != edges.end(); I++){sel.edges.push_back(*I);}
	//for(auto I = tris.begin(); I != tris.end(); I++){sel.tris.push_back(*I);}
	return sel;
}

//a neighbor is any element that needs to be modified if this element is removed/replaced
//neighbor types:
//	essential					makes up the definition of this element. subset of direct. Only vertices.
//	direct			contained	contains or is contained by this element. subset of indirect.
//  indirect		sharing		shares elements with this one. 
//	remote						indirect neighbors of a neighbor.

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
}

void e_model::repair(){
	stripNeighbors();
	checkDegenerate(true);
	recalculateNeighbors();
}

#define cdp_helper() \
	if(!contains(v,*_J)){	\
		repneeded = true;	\
		if(repair){_J = (x)->n.v.erase(_J);}	\
	}
	

bool e_model::checkDanglingPointers(bool repair){
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
	bool repneeded = false;
	for_all_elements(I,{
		for_all_n(*I,J,
			if(!contains(v,*J)){
				repneeded = true;
				if(repair){J = v.erase(J);}	//wonder if v will be replaced?
			}
		);
	});
	return repneeded;
}

//#define cd_check_self_neighbor(x) 	
//	for_all_elements2(I2,{
//			if(*I
//		}							
									

bool e_model::checkDegenerate(bool repair){
	//if repair, repairs the model until it is not degenerate.
	//else, only checks if repair is necessary.
	//returns true if model was degenerate
	//bool repneeded = false;
	//cd_loop:
	//	repneeded = checkDanglingPointers(repair);
	//	if(repneeded && !repair){return true;}
	//should be safe against dangling neighbor pointers
	//at the start of each run, removeDanglingPointers
	//	(we may have started with some, or created them last iteration)
	//degenerate elements:
	//element whose neighbor is itself (remove neighbor)
	//no such thing as an element with too many essential verts (do nothing)
	//element with not enough essential verts (remove element)
	//edge two repeated verts (replace edge with point)
	//triangle  (replace tri with point)
	//triangle whose two endpoitns are the same (replace tri with edge)
	//if anything done this time, check again
	
	error("removeDegenerates not implemented");
}

//if(v1 == v2){error("degenerate edge");}	//an edge can be connected to two points sharing the same pos, but it can't be connected to the same point twice

#define nI() (*I)->neighbors
#define nJ() (*J)->neighbors
#define n(x) x->neighbors
void e_model::recalculateNeighbors(){
	//0.1) essential: (env), (tnv)
	//0.2) strip neighbors
	stripNeighbors();
	checkDegenerate(true);
	//1) rebuild vertex.neighbors.edges (vne) from essential (env)
		//for(auto I = edges.begin(); I != edges.end(); I++){
		//	e_vertex *v1 = nI().verts[0];
		//	e_vertex *v2 = nI().verts[1];
		//	n(v1).edges.push_back(*I);
		//	n(v2).edges.push_back(*I);
		//}
	//2) rebuild vertex.neighbors.verts (vnv) from (vne) and (env)
		//for(auto I = verts.begin(); I != verts.end(); I++){
		//	e_vertex *v = *I;
		//	for(auto J = nI().edges.begin(); J != nI().edges.end(); J++){
		//		e_vertex *v1 = nJ().verts[0];
		//		e_vertex *v2 = nJ().verts[1];
		//		if(v1 == v){nI().verts.push_back(v2);}
		//		if(v2 == v){nI().verts.push_back(v1);}
		//	}
		//}
	//3) rebuild edges.neighbors.edges (ene) from (vne) and (env)
	//4) rebuild vertex.neighbors.tris (vnt) from essential (tnv)
	//5,6) rebuild tris.neighbors.edges (tne) and edges.neighbors.tris (ent) from (tnv) and (vne)
	//7) rebuild tris.neighbors.tris (tnt) from (tne) and (ent)
	
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










