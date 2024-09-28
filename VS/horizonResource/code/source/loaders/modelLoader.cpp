#ifdef USE_TINY_OBJ_LOADER
	#define TINYOBJLOADER_IMPLEMENTATION
	#include "Ext/tiny_obj_loader/tiny_obj_loader.h"
#endif

#include "math/simplemath.h"
#include "program/file.h"
#include "resource/model.h"
#include "resource/resource.h"
#include "loaders/modelLoader.h"
#include "util/globals_resource.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
using namespace std;

map<string,model*> modelCache;
vector<tinyobj::shape_t> shapes;
vector<tinyobj::material_t> materials;

void model::add(model B){
	for(unsigned int I = 0; I < B.tris.size(); I++){
		tris.push_back(B.tris[I]);
	}
}

void model::recalculateNormals(){
	for(unsigned int I = 0; I < tris.size(); I++){
		auto &A = tris[I].v[0];
		auto &B = tris[I].v[1];
		auto &C = tris[I].v[2];
		
		//vec3f norm = (B.pos-A.pos).cross(C.pos-A.pos).norm();
		vec3 norm = normalize(cross((B.pos - A.pos),(C.pos - A.pos)));
		A.normal = norm;
		B.normal = norm;
		C.normal = norm;
	}
}

AABB model::getAABB(){
	AABB aabb;
	aabb = aabb.setStart(tris[0].v[0].pos);
	aabb = aabb.setEnd(tris[0].v[0].pos);
	for(unsigned int I = 0; I < tris.size(); I++){
		for(unsigned int J = 0; J < 3; J++){
			//vec3f p = tris[I].v[J].pos;
			vec3 p = tris[I].v[J].pos;
			aabb = aabb.setStart({min(aabb.start.x,p.x),min(aabb.start.y,p.y),min(aabb.start.z,p.z)});
			aabb = aabb.setEnd({max(aabb.end.x,p.x),max(aabb.end.y,p.y),max(aabb.end.z,p.z)});
		}
	}
	return aabb;
}


//model *getModel(string name){
//	if(!modelCache.count(name)){
//		string filepath = locateResource("model",name.c_str());
//		model *M = loadModel(filepath.c_str());
//		M->t = getTexture(string()+"model_"+name);
//		modelCache[name] = M;
//	}
//	return modelCache[name];
//}

void printCurrentModel();

vertex readVertex(tinyobj::mesh_t S, int I){
	vertex A;
	A.pos = {S.positions[S.indices[I]*3],S.positions[S.indices[I]*3+1],S.positions[S.indices[I]*3+2]};
	A.normal = {S.normals[S.indices[I]*3],S.normals[S.indices[I]*3+1],S.normals[S.indices[I]*3+2]};
	A.uv = {S.texcoords[S.indices[I]*2],S.texcoords[S.indices[I]*2+1]};
	return A;
}

triangle readTriangle(tinyobj::mesh_t S, int I){
	triangle T;
	T.v[0] = readVertex(S, I*3);
	T.v[1] = readVertex(S, I*3+1);
	T.v[2] = readVertex(S, I*3+2);
	return T;
}

model *loadModel(const char *filepath){
	string inputfile = filepath;

	string err;
	bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str());
	if(!err.empty()){
		//warning("loadModel: error: %s\n",err.c_str());
		cerr << "warning: " << "loadModel: error :" << err << endl;
	}
	if(!ret){
		//error("loadModel: LoadObj failed\n");
		throw runtime_error("loadModel: LoadObj failed");
	}
	if(!shapes.size()){
		//error("loadModel: empty obj file\n");
		throw runtime_error("loadModel: empty obj file");
	}
	//printCurrentModel();
	
	model *M = new model();
	auto &S = shapes[0].mesh;
	int numVerts = S.positions.size()/3;
	int numTris = S.num_vertices.size();
	for(int I = 0; I < numTris; I++){
		M->tris.push_back(readTriangle(S,I));		
	}
	printf("model loaded: %d triangles, %d vertices (%d%% reuse)\n",numTris, numVerts, 100-(33*numVerts/numTris));
	//printf("loadModel: loaded %d verts, %d faces\n",
		//						shapes[0].mesh.positions.size()/3,
			//					shapes[0].mesh.ids.size());
	return M;
}

void printCurrentModel(){
	for (size_t i = 0; i < shapes.size(); i++) {
	  printf("shape[%ld].name = %s\n", (long)i, shapes[i].name.c_str());
	  printf("Size of shape[%ld].indices: %ld\n", (long)i, (long)shapes[i].mesh.indices.size());
	  printf("Size of shape[%ld].material_ids: %ld\n", (long)i, (long)shapes[i].mesh.material_ids.size());
	  assert((shapes[i].mesh.indices.size() % 3) == 0);
	  for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
		printf("  idx[%ld] = %d, %d, %d. mat_id = %d\n", (long)f, shapes[i].mesh.indices[3*f+0], shapes[i].mesh.indices[3*f+1], shapes[i].mesh.indices[3*f+2], shapes[i].mesh.material_ids[f]);
	  }

	  printf("shape[%ld].vertices: %ld\n", (long)i, (long)shapes[i].mesh.positions.size());
	  assert((shapes[i].mesh.positions.size() % 3) == 0);
	  for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
		printf("  v[%ld] = (%f, %f, %f)\n", (long)v,
		  shapes[i].mesh.positions[3*v+0],
		  shapes[i].mesh.positions[3*v+1],
		  shapes[i].mesh.positions[3*v+2]);
	  }
	}

	for (size_t i = 0; i < materials.size(); i++) {
	  printf("material[%ld].name = %s\n", (long)i, materials[i].name.c_str());
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