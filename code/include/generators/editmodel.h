#ifndef EDITMODEL_GUARD
#define EDITMODEL_GUARD
#include "vec.h"
#include "rmodel.h"
#include "globals.h"
#include <list>
#include <vector>
#include <set>
using std::list;
using std::vector;
using std::set;

class e_vertex;
class e_edge;
class e_triangle;
struct e_model;
class e_face;


struct e_selection{
	vector<e_vertex*>	verts;
	vector<e_edge*>		edges;
	vector<e_triangle*>	tris;
	vector<e_face*>		faces;
	e_model *EM=0;
	rmpack rms;
	vec3 colorVerts = {1.f,0,0};
	float alphaVerts = 255.f;
	vec3 colorEdges = {1.f,1.f,0};
	float alphaEdges = 255.f;
	vec3 colorTris = {1.f,1.f,1.f};
	float alphaTris = 255.f;
	bool rainbowTris = true;
	e_selection() = delete; 
	e_selection(e_model *EM);

	
	void addElements(e_selection sel2);		
	void removeElements(e_selection sel2);	
	void clear();							

	
	e_selection getVerts();			
	e_selection getEdges();			
	e_selection getTris();			



	e_selection getImplicitVerts();	

	e_selection getImplicitEdges();	
									
	e_selection getImplicitTris();	
									

	
	
	vector<e_selection> getNormalized();
	
	vec3 center();					
	AABB getAABB();

	
	void deleteAll();									
	void move(vec3 offset); 							
	void rotate(vec3 center, vec3 axis, float angle); 	
	void scale(vec3 center, float scale);				
	void scale(vec3 center, vec3 scale);				

	void uv_project_box(vec3 origin,float scale);		
	void uv_scale(float scale);							

	e_selection addTo(e_model *EM);						

	void recalculateNormalsSmooth();					
	void recalculateNormalsFlat();						
	void removeDuplicates();							

	void rebuildRmodel();
	void rebuildRmodel_vertHelper(rmodel *rm);
	void rebuildRmodel_edgeHelper(rmodel *rm);
	void rebuildRmodel_triHelper(rmodel *rm);
	void rebuildRmodel_wireHelper(rmodel *rm);
	void render();
	
	
	
	vector<e_selection> extrude();
};

class e_element{
	public:
	e_selection definition;
	e_selection parts;
	e_selection touching;
	float selection_weight = 1.0f;
	e_element() = delete;
	e_element(e_model *EM);
};

class e_vertex:public e_element{
	public:
	vec3 pos = {0,0,0};
	e_vertex() = default;
	e_vertex(vec3 pos, e_model *EM);
};

class e_edge:public e_element{
	public:
	e_edge() = default;
	e_edge(e_vertex *A, e_vertex *B, e_model *EM);
};

class e_triangle:public e_element{
	public:
	vec2 uvs[3];
	vec3 face_normal;
	vec3 vert_normals[3];
	e_triangle() = default;
	e_triangle(e_vertex *A, e_vertex *B, e_vertex *C, e_model *EM);
};

class e_face:public e_element{
	public:
	e_face() = default;
	e_face(vector<e_triangle*> tris, e_model *EM);
	void recalcEdges(); 
};


template<typename T> T& listGet(list<T> &L,int N){if(N >= L.size()){error("index out of bounds");} auto I = L.begin(); while(N--){ I++;} return *I;}

struct e_model{
	list<e_vertex*> verts;
	list<e_edge*> edges;
	list<e_triangle*> tris;
	list<e_face*> faces;
	bool isElaborate = false;
	e_selection selectAll();
	
	const char *checkDuplicates(bool repair);			
	const char *checkDegenerate(bool repair);			
	const char *checkDanglingPointers(bool repair);	
	
	void removePartsInfo();					
	void removeTouchingInfo();
	void recalculateNeighbors();				
	void repair();								
	void sanityCheck();
	rmodel *getRmodel(int mode = 2);						
	rmpack getRmpack();
};
#include <string>
using std::string;

string toString(e_model *EM);

#endif
