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

//holds weak pointers (but not literally) to other elements
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
	e_selection() = delete; //never make a selection without a EM for it to refer to
	e_selection(e_model *EM);

	//selection editing
	void addElements(e_selection sel2);		//adds to sel all elements in sel2
	void removeElements(e_selection sel2);	//removes from sel all elements in sel2
	void clear();							//removes all elements from selection

	//selection generation
	e_selection getVerts();			//returns the portion of this selection with only the vertices
	e_selection getEdges();			//returns the portion of this selection with only the edges
	e_selection getTris();			//returns the portion of this selection with only the triangle



	e_selection getImplicitVerts();	//returns all the implied vertices (<verts> + those in edges + those in <triangles>)

	e_selection getImplicitEdges();	//returns all the implied edges (those that share <verts> + <edges> + those in <triangles>)
									//runs faster if no verts explicitly selected.
	e_selection getImplicitTris();	//returns all the impleid triangles (those that share <verts> + those that share <edges> + <triangles>)
									//runs faster if no verts or edges explicitly selected.

	//returns [0]: minimal set: selected tris, selected edges not in tris, and selected verts not in tris or edges
	//returns [1]: maximal set: selected tris, selected edges + those in tris, and selected verts + those in tris + those in edges
	vector<e_selection> getNormalized();
	//selection measurement
	vec3 center();					//geometric mean of all vertices, centers of all edges, and centers of all triangles.
	AABB getAABB();

	//model editing
	void deleteAll();									//erases elements from the model
	void move(vec3 offset); 							//moves <selection> by <offset>
	void rotate(vec3 center, vec3 axis, float angle); 	//rotates <selection> around <axis> that passes through <center> by <angle> degrees.
	void scale(vec3 center, float scale);				//scales <selection> uniformly by <scale> away from <center>
	void scale(vec3 center, vec3 scale);				//scales <selection> by <scale> per-axis away from <center>

	void uv_project_box(vec3 origin,float scale);		//applies projects texture using box-project.
	void uv_scale(float scale);							//multiplies every UV coord by number

	e_selection addTo(e_model *EM);						//adds all elements to a different e_model

	void recalculateNormalsSmooth();					//sets shading to "smooth"
	void recalculateNormalsFlat();						//sets shading to "flat"
	void removeDuplicates();							//merges vertices with the same position

	void rebuildRmodel();
	void rebuildRmodel_vertHelper(rmodel *rm);
	void rebuildRmodel_edgeHelper(rmodel *rm);
	void rebuildRmodel_triHelper(rmodel *rm);
	void rebuildRmodel_wireHelper(rmodel *rm);
	void render();
	//extrudes <selected>
	//returns [0]: newly created elements
	//returns [1]: elements connecting the existing elements to new elements.
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
	void recalcEdges(); //this should be done by e_model::recalculateNeighbors()
};

//it bork
template<typename T> T& listGet(list<T> &L,int N){if(N >= L.size()){error("index out of bounds");} auto I = L.begin(); while(N--){ I++;} return *I;}

struct e_model{
	list<e_vertex*> verts;
	list<e_edge*> edges;
	list<e_triangle*> tris;
	list<e_face*> faces;
	bool isElaborate = false;
	e_selection selectAll();
	//e_selection selectBox(vec3 start, vec3 end);
	const char *checkDuplicates(bool repair);			//checks if there are duplicate (by reference) elements
	const char *checkDegenerate(bool repair);			//remove or repair weird edges and triangles (model  must be stripped)
	const char *checkDanglingPointers(bool repair);	//remove references to deleted elements
	//void stripNeighbors();
	void removePartsInfo();					//remove non-essential connectivity info
	void removeTouchingInfo();
	void recalculateNeighbors();				//reconstruct additional connectivity info
	void repair();								//all of the above
	void sanityCheck();
	rmodel *getRmodel(int mode = 2);						//build and return a rmodel;
	rmpack getRmpack();
};
#include <string>
using std::string;

string toString(e_model *EM);

#endif
