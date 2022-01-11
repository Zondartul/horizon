//models.h
#include "resource/models.h"
#include "math/vectors.h"


int& idtriangle::operator [](int N){return v[N];}

idtriangle::idtriangle(){
	v[0] = v[1] = v[2] = 0;
}
idtriangle::idtriangle(int A, int B, int C){
	v[0] = A;
	v[1] = B;
	v[2] = C;
}
idtriangle::idtriangle(vec A, vec B, vec C, vector<vec> *points){
	bool Afound = false;
	bool Bfound = false;
	bool Cfound = false;
	for(int I = 0; I<points->size(); I++){
		if((*points)[I] == A){Afound = true; v[0] = I;}
		if((*points)[I] == B){Bfound = true; v[1] = I;}
		if((*points)[I] == C){Cfound = true; v[2] = I;}
	}
	if(!Afound){points->push_back(A); v[0] = points->size()-1;}
	if(!Bfound){points->push_back(B); v[1] = points->size()-1;}
	if(!Cfound){points->push_back(C); v[2] = points->size()-1;}
	printf("making a triangle with v[0] = %d, v[1] = %d, v[2] = %d\n", v[0], v[1], v[2]);
}

vec idtriangle::getNormal(vector<vec> *points){
	return ((*points)[v[2]]-(*points)[v[0]]).cross((*points)[v[1]]-(*points)[v[0]]).norm();
}

vec idtriangle::getPlane(vector<vec> *points){
	return getNormal(points)*((*points)[v[0]].dot(getNormal(points)));
}

vec& vtriangle::operator [](int N){return v[N];}
vtriangle::vtriangle(){
	v[0] = v[1] = v[2] = {0,0,0};
}
vtriangle::vtriangle(vec A, vec B, vec C){
	v[0] = A;
	v[1] = B;
	v[2] = C;
}
vec vtriangle::getNormal(){
	return (v[2]-v[0]).cross(v[1]-v[0]).norm();
}

vec vtriangle::getPlane(){
	return getNormal()*(v[0].dot(getNormal()));
}

/*struct model{
	vector<vec> vertices;
	vector<idtriangle> mesh;
	vector<textriangle> uvmap;
	vector<texture> textures;
	model();
}*/
model::model(){
}
void model::add(model *M){
	float offset = vertices.size();
	vertices.insert(vertices.end(), M->vertices.begin(), M->vertices.end());
	for(int I = 0; I < M->mesh.size(); I++){
		idtriangle tri = M->mesh[I];
		tri[0] += offset;
		tri[1] += offset;
		tri[2] += offset;
		mesh.push_back(tri);
	}
}
/*
model::model()
{
	numtris = 0; numtextures = 0;
	mesh = NULL;
	normals = NULL;
	texmap = NULL;
	textures = NULL;
	min = max = {0,0,0};
	blendmode = 1;
}
void model::applyRenderFlags(int R)
{
	// is texmap missing?
	for(int i = 0;i<numtris;i++)
	{
		texmap[i].flags = (renderflags)R;
	}
}
void model::expandTriangle(vec point, int tri) //no texture support yet
{
	numtris += 2;
	mesh = (vtriangle*)realloc((void*)mesh, numtris*sizeof(vtriangle));
	vtriangle t = mesh[tri];
	
	mesh[tri] 		= (vtriangle){t.v[0],t.v[1],point};
	mesh[numtris-2] = (vtriangle){point,t.v[1],t.v[2]};
	mesh[numtris-1] = (vtriangle){t.v[2],t.v[0],point};
}
vec model::getNormalFlat(int tri)
{
	return (mesh[tri].v[1]-mesh[tri].v[0]).cross(mesh[tri].v[2]-mesh[tri].v[0]).norm();
}
vector<int> model::getAdjacentTris(int tri, int vert)
{
	vector<int> Adj;
	vec v = mesh[tri].v[vert];
	for(int i = 0;i<numtris;i++)
	{
		if((mesh[i].v[0]==v)||
			(mesh[i].v[1]==v)||
			(mesh[i].v[2]==v))
		{
			Adj.push_back(i);
		}
	}
	return Adj;
}
*/