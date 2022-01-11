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
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "simplemath.h"
#include "model.h"

// map<string,model> modelCache;

// model getModel(string name){
	// if(!modelCache.count(name)){
		// string filepath = locateResource("model",name.c_str());
		// model M = loadModel(filepath.c_str());
		// M.t = getTexture(string()+"models/"+name+"/model_"+name);
		// modelCache[name] = M;
	// }
	// return modelCache[name];
// }

void printCurrentModel();

vertex readVertex(auto S, int I){
	vertex A;
	A.pos = {S.positions[S.indices[I]*3],S.positions[S.indices[I]*3+1],S.positions[S.indices[I]*3+2]};
	A.normal = {S.normals[S.indices[I]*3],S.normals[S.indices[I]*3+1],S.normals[S.indices[I]*3+2]};
	A.uv = {S.texcoords[S.indices[I]*2],S.texcoords[S.indices[I]*2+1]};
	return A;
}

triangle readTriangle(auto S, int I){
	triangle T;
	T.v[0] = readVertex(S, I*3);
	T.v[1] = readVertex(S, I*3+1);
	T.v[2] = readVertex(S, I*3+2);
	return T;
}


vector<tinyobj::shape_t> shapes;
vector<tinyobj::material_t> materials;

void assert_equals(int A, int B, string msg){
    if(A != B){error("%s: got %d, expected %d\n",msg.c_str(),A,B);}
}

void assert_greater_equals(int A, int B, string msg){
    if(A < B){error("%s: got %d < expected %d\n",msg.c_str(),A,B);}
}

model *loadModel(const char *filepath){
	//string inputfile = filepath;

	string err;

	bool ret = tinyobj::LoadObj(shapes, materials, err, filepath); //value-copy? wat
	if(!err.empty()){warning("loadModel: error: %s\n",err.c_str());}
	if(!ret){error("loadModel: LoadObj failed\n");}
	if(!shapes.size()){error("loadModel: empty obj file\n");}
	//printCurrentModel();

	model *M = new model();
	tinyobj::mesh_t &S = shapes[0].mesh;

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
	//printf("loadModel: loaded %d verts, %d faces\n",
		//						shapes[0].mesh.positions.size()/3,
			//					shapes[0].mesh.ids.size());
	return M;
}



void printLastModel(){
	for (size_t i = 0; i < shapes.size(); i++) {
	  printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
	  printf("Size of shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
	  printf("Size of shape[%ld].material_ids: %ld\n", i, shapes[i].mesh.material_ids.size());
	  assert((shapes[i].mesh.indices.size() % 3) == 0);
	  for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
		printf("  idx[%ld] = %d, %d, %d. mat_id = %d\n", f, shapes[i].mesh.indices[3*f+0], shapes[i].mesh.indices[3*f+1], shapes[i].mesh.indices[3*f+2], shapes[i].mesh.material_ids[f]);
	  }

	  printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
	  assert((shapes[i].mesh.positions.size() % 3) == 0);
	  for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
		printf("  v[%ld] = (%f, %f, %f)\n", v,
		  shapes[i].mesh.positions[3*v+0],
		  shapes[i].mesh.positions[3*v+1],
		  shapes[i].mesh.positions[3*v+2]);
	  }
	}

	for (size_t i = 0; i < materials.size(); i++) {
	  printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
	  printf("  material.Ka = (%f, %f ,%f)\n", materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
	  printf("  material.Kd = (%f, %f ,%f)\n", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
	  printf("  material.Ks = (%f, %f ,%f)\n", materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
	  printf("  material.Tr = (%f, %f ,%f)\n", materials[i].transmittance[0], materials[i].transmittance[1], materials[i].transmittance[2]);
	  printf("  material.Ke = (%f, %f ,%f)\n", materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);
	  printf("  material.Ns = %f\n", materials[i].shininess);
	  printf("  material.Ni = %f\n", materials[i].ior);
	  printf("  material.dissolve = %f\n", materials[i].dissolve);
	  printf("  material.illum = %d\n", materials[i].illum);
	  printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
	  printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
	  printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
	  printf("  material.map_Ns = %s\n", materials[i].specular_highlight_texname.c_str());
	  std::map<std::string, std::string>::const_iterator it(materials[i].unknown_parameter.begin());
	  std::map<std::string, std::string>::const_iterator itEnd(materials[i].unknown_parameter.end());
	  for (; it != itEnd; it++) {
		printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
	  }
	  printf("\n");
	}
}
//tinyobjloader use example:
/*
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

std::string inputfile = "cornell_box.obj";
std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;

std::string err;
bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str());

if (!err.empty()) { // `err` may contain warning message.
  std::cerr << err << std::endl;
}

if (!ret) {
  exit(1);
}

std::cout << "# of shapes    : " << shapes.size() << std::endl;
std::cout << "# of materials : " << materials.size() << std::endl;

for (size_t i = 0; i < shapes.size(); i++) {
  printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
  printf("Size of shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
  printf("Size of shape[%ld].material_ids: %ld\n", i, shapes[i].mesh.material_ids.size());
  assert((shapes[i].mesh.indices.size() % 3) == 0);
  for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
    printf("  idx[%ld] = %d, %d, %d. mat_id = %d\n", f, shapes[i].mesh.indices[3*f+0], shapes[i].mesh.indices[3*f+1], shapes[i].mesh.indices[3*f+2], shapes[i].mesh.material_ids[f]);
  }

  printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
  assert((shapes[i].mesh.positions.size() % 3) == 0);
  for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
    printf("  v[%ld] = (%f, %f, %f)\n", v,
      shapes[i].mesh.positions[3*v+0],
      shapes[i].mesh.positions[3*v+1],
      shapes[i].mesh.positions[3*v+2]);
  }
}

for (size_t i = 0; i < materials.size(); i++) {
  printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
  printf("  material.Ka = (%f, %f ,%f)\n", materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
  printf("  material.Kd = (%f, %f ,%f)\n", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
  printf("  material.Ks = (%f, %f ,%f)\n", materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
  printf("  material.Tr = (%f, %f ,%f)\n", materials[i].transmittance[0], materials[i].transmittance[1], materials[i].transmittance[2]);
  printf("  material.Ke = (%f, %f ,%f)\n", materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);
  printf("  material.Ns = %f\n", materials[i].shininess);
  printf("  material.Ni = %f\n", materials[i].ior);
  printf("  material.dissolve = %f\n", materials[i].dissolve);
  printf("  material.illum = %d\n", materials[i].illum);
  printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
  printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
  printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
  printf("  material.map_Ns = %s\n", materials[i].specular_highlight_texname.c_str());
  std::map<std::string, std::string>::const_iterator it(materials[i].unknown_parameter.begin());
  std::map<std::string, std::string>::const_iterator itEnd(materials[i].unknown_parameter.end());
  for (; it != itEnd; it++) {
    printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
  }
  printf("\n");
}
*/
