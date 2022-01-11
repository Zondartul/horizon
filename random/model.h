#ifndef MODEL_GUARD
#define MODEL_GUARD
#include <vector>
using std::vector;

//we'll do a vertex referrer graph (optimisation) later

//lone vertices
struct m_vertex{
	int vA;
	int cA=0;
};

//lone edges
struct m_edge{
	int vA;
	int vB;
	int cA=0;
	int cB=0;
};

//triangles
struct m_triangle{
	int vA;
	int vB;
	int vC;
	int UVA=0;
	int UVB=0;
	int UVC=0;
	int cA=0;
	int cB=0;
	int cC=0;
};

//contains information, other than the model,
//necessary for rendering an instance of an object
struct renderOptions{
	vec pos;
};


//there are two kinds of model.
//the edit-model contains data that simplifies editing.
//the render-model deals with specifics of displaying the model.
//the r_model needs to be loaded before it can be rendered.
//if changes are made, the model needs to be loaded again for them to take effect.
//once deleted, if the model had been loaded, it will unload itself.

class editmodel{
	public:
	vector<vec> positions;	//contains unique vec3f vertex position data
	vector<vec> UVs;		//contains unique vec2f vertex UV data
	vector<vec> colors;		//contains unique vec3f vertex colors
	int material;
	vector<m_vertex> vertices; //contains indexing vertices
	vector<m_edge> edges;			//contains indexing edges
	vector<m_triangle> triangles;	//contains indexing triangles
	
	model();
	~model();
};

class rendermodel{
	editmodel *M;
	vector<GLfloat> buffer;
	int numVerts;
	int numEdges;
	int numTris;
	rendermodel();
	~rendermodel();
	void load();	//update OpenGL VBOs and VAOs (prepare to render)
	void unload();	//destroy OpenGL VBOs and VAOs (clean up)
	void render(renderOptions O);	//draw the loaded VAO.
	void buffer_vertex(m_vertex V);
	void buffer_edge(m_edge E);
	void buffer_triangle(m_triangle T);
	void buffer_position(vec V);
	void buffer_UV(vec UV);
	void buffer_color(vec C);
};

#endif