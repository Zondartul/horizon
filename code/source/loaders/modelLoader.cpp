#include "modelLoader.h"
#include "globals.h"
//#include "selfaware.h"
#include "file.h"
#include <map>
using std::map;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <map>
using std::map;

#pragma warning(push, 0)
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#pragma warning(pop)

#include "simplemath.h"
#include "model.h"

void printCurrentModel();

vertex readVertex(tinyobj::mesh_t& S, int I){
	vertex A;
	A.pos = {S.positions[S.indices[I]*3],S.positions[S.indices[I]*3+1],S.positions[S.indices[I]*3+2]};
	A.normal = {S.normals[S.indices[I]*3],S.normals[S.indices[I]*3+1],S.normals[S.indices[I]*3+2]};
	A.uv = {S.texcoords[S.indices[I]*2],S.texcoords[S.indices[I]*2+1]};
	return A;
}

triangle readTriangle(tinyobj::mesh_t& S, int I){
	triangle T;
	T.v[0] = readVertex(S, I*3);
	T.v[1] = readVertex(S, I*3+1);
	T.v[2] = readVertex(S, I*3+2);
	return T;
}


vector<tinyobj::shape_t> g_shapes;
vector<tinyobj::material_t> g_materials;

void assert_equals(int A, int B, string msg){
    if(A != B){error("%s: got %d, expected %d\n",msg.c_str(),A,B);}
}

void assert_greater_equals(int A, int B, string msg){
    if(A < B){error("%s: got %d < expected %d\n",msg.c_str(),A,B);}
}

model *loadModel(const char *filepath){
	string err;
	bool ret = tinyobj::LoadObj(g_shapes, g_materials, err, filepath); //value-copy? wat
	if(!err.empty()){warning("loadModel: error: %s\n",err.c_str());}
	if(!ret){error("loadModel: LoadObj failed\n");}
	if(!g_shapes.size()){error("loadModel: empty obj file\n");}

	model *M = new model();
	tinyobj::mesh_t &S = g_shapes[0].mesh;

    int numIndices = S.indices.size();
    int numPositions = S.positions.size();
    int numNormals = S.normals.size();
    int numTexcoords = S.texcoords.size();

	int numVerts = S.positions.size()/3;
	int numTris = S.num_vertices.size();

	assert_greater_equals(numPositions, numTris*3, "broken model.obj, wrong number of vertex positions");
	assert_greater_equals(numNormals, numTris*3,   "broken model.obj, wrong number of vertex normals");
	assert_greater_equals(numTexcoords, numTris*2, "broken model.obj, wrong number of vertex UV coords");

	for(int I = 0; I < numTris; I++){
		M->tris.push_back(readTriangle(S,I));
	}
	printf("model loaded: %d triangles, %d vertices (%d%% reuse)\n",numTris, numVerts, 100-(33*numVerts/numTris));

	return M;
}



void printLastModel(){
	for (size_t i = 0; i < g_shapes.size(); i++) {
	  printf("shape[%ld].name = %s\n", i, g_shapes[i].name.c_str());
	  printf("Size of shape[%ld].indices: %ld\n", i, g_shapes[i].mesh.indices.size());
	  printf("Size of shape[%ld].material_ids: %ld\n", i, g_shapes[i].mesh.material_ids.size());
	  assert((g_shapes[i].mesh.indices.size() % 3) == 0);
	  for (size_t f = 0; f < g_shapes[i].mesh.indices.size() / 3; f++) {
		printf("  idx[%ld] = %d, %d, %d. mat_id = %d\n", f, g_shapes[i].mesh.indices[3*f+0], g_shapes[i].mesh.indices[3*f+1], g_shapes[i].mesh.indices[3*f+2], g_shapes[i].mesh.material_ids[f]);
	  }

	  printf("shape[%ld].vertices: %ld\n", i, g_shapes[i].mesh.positions.size());
	  assert((g_shapes[i].mesh.positions.size() % 3) == 0);
	  for (size_t v = 0; v < g_shapes[i].mesh.positions.size() / 3; v++) {
		printf("  v[%ld] = (%f, %f, %f)\n", v,
		  g_shapes[i].mesh.positions[3*v+0],
		  g_shapes[i].mesh.positions[3*v+1],
		  g_shapes[i].mesh.positions[3*v+2]);
	  }
	}

	for (size_t i = 0; i < g_materials.size(); i++) {
	  printf("material[%ld].name = %s\n", i, g_materials[i].name.c_str());
	  printf("  material.Ka = (%f, %f ,%f)\n", g_materials[i].ambient[0], g_materials[i].ambient[1], g_materials[i].ambient[2]);
	  printf("  material.Kd = (%f, %f ,%f)\n", g_materials[i].diffuse[0], g_materials[i].diffuse[1], g_materials[i].diffuse[2]);
	  printf("  material.Ks = (%f, %f ,%f)\n", g_materials[i].specular[0], g_materials[i].specular[1], g_materials[i].specular[2]);
	  printf("  material.Tr = (%f, %f ,%f)\n", g_materials[i].transmittance[0], g_materials[i].transmittance[1], g_materials[i].transmittance[2]);
	  printf("  material.Ke = (%f, %f ,%f)\n", g_materials[i].emission[0], g_materials[i].emission[1], g_materials[i].emission[2]);
	  printf("  material.Ns = %f\n", g_materials[i].shininess);
	  printf("  material.Ni = %f\n", g_materials[i].ior);
	  printf("  material.dissolve = %f\n", g_materials[i].dissolve);
	  printf("  material.illum = %d\n", g_materials[i].illum);
	  printf("  material.map_Ka = %s\n", g_materials[i].ambient_texname.c_str());
	  printf("  material.map_Kd = %s\n", g_materials[i].diffuse_texname.c_str());
	  printf("  material.map_Ks = %s\n", g_materials[i].specular_texname.c_str());
	  printf("  material.map_Ns = %s\n", g_materials[i].specular_highlight_texname.c_str());
	  std::map<std::string, std::string>::const_iterator it(g_materials[i].unknown_parameter.begin());
	  std::map<std::string, std::string>::const_iterator itEnd(g_materials[i].unknown_parameter.end());
	  for (; it != itEnd; it++) {
		printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
	  }
	  printf("\n");
	}
}