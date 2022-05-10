








































#ifndef TINY_OBJ_LOADER_H_
#define TINY_OBJ_LOADER_H_

#include <string>
#include <vector>
#include <map>
#include <cmath>

namespace tinyobj {

typedef struct {
  std::string name;

  float ambient[3];
  float diffuse[3];
  float specular[3];
  float transmittance[3];
  float emission[3];
  float shininess;
  float ior;      
  float dissolve; 
  
  int illum;

  int dummy; 

  std::string ambient_texname;            
  std::string diffuse_texname;            
  std::string specular_texname;           
  std::string specular_highlight_texname; 
  std::string bump_texname;               
  std::string displacement_texname;       
  std::string alpha_texname;              
  std::map<std::string, std::string> unknown_parameter;
} material_t;

typedef struct {
  std::string name;

  std::vector<int> intValues;
  std::vector<float> floatValues;
  std::vector<std::string> stringValues;
} tag_t;

typedef struct {
  std::vector<float> positions;
  std::vector<float> normals;
  std::vector<float> texcoords;
  std::vector<unsigned int> indices;
  std::vector<unsigned char>
      num_vertices;              
  std::vector<int> material_ids; 
  std::vector<tag_t> tags;       
} mesh_t;

typedef struct {
  std::string name;
  mesh_t mesh;
} shape_t;

typedef enum
{
  triangulation = 1,        
  calculate_normals = 2,    
  
} load_flags_t;

class float3
{
public:
  float3()
    : x( 0.0f )
    , y( 0.0f )
    , z( 0.0f )
  {
  }

  float3(float coord_x, float coord_y, float coord_z)
    : x( coord_x )
    , y( coord_y )
    , z( coord_z )
  {
  }

  float3(const float3& from, const float3& to)
  {
    coord[0] = to.coord[0] - from.coord[0];
    coord[1] = to.coord[1] - from.coord[1];
    coord[2] = to.coord[2] - from.coord[2];
  }

  float3 crossproduct ( const float3 & vec )
  {
    float a = y * vec.z - z * vec.y ;
    float b = z * vec.x - x * vec.z ;
    float c = x * vec.y - y * vec.x ;
    return float3( a , b , c );
  }

  void normalize()
  {
    const float length = std::sqrt( ( coord[0] * coord[0] ) +
                                               ( coord[1] * coord[1] ) +
                                               ( coord[2] * coord[2] ) );
    if( length != 1 )
    {
      coord[0] = (coord[0] / length);
      coord[1] = (coord[1] / length);
      coord[2] = (coord[2] / length);
    }
  }

private:
  union
  {
    float coord[3];
    struct
    {
      float x,y,z;
    };
  };
};

class MaterialReader {
public:
  MaterialReader() {}
  virtual ~MaterialReader();

  virtual bool operator()(const std::string &matId,
                          std::vector<material_t> &materials,
                          std::map<std::string, int> &matMap,
                          std::string &err) = 0;
};

class MaterialFileReader : public MaterialReader {
public:
  MaterialFileReader(const std::string &mtl_basepath)
      : m_mtlBasePath(mtl_basepath) {}
  virtual ~MaterialFileReader() {}
  virtual bool operator()(const std::string &matId,
                          std::vector<material_t> &materials,
                          std::map<std::string, int> &matMap, std::string &err);

private:
  std::string m_mtlBasePath;
};








bool LoadObj(std::vector<shape_t> &shapes,       
             std::vector<material_t> &materials, 
             std::string &err,                   
             const char *filename, const char *mtl_basepath = NULL,
             unsigned int flags = 1 );





bool LoadObj(std::vector<shape_t> &shapes,       
             std::vector<material_t> &materials, 
             std::string &err,                   
             std::istream &inStream, MaterialReader &readMatFn,
             unsigned int flags = 1);


void LoadMtl(std::map<std::string, int> &material_map, 
             std::vector<material_t> &materials,       
             std::istream &inStream);
}

#ifdef TINYOBJLOADER_IMPLEMENTATION
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cctype>

#include <fstream>
#include <sstream>

#include "tiny_obj_loader.h"

namespace tinyobj {

MaterialReader::~MaterialReader() {}

#define TINYOBJ_SSCANF_BUFFER_SIZE (4096)

struct vertex_index {
  int v_idx, vt_idx, vn_idx;
  vertex_index() : v_idx(-1), vt_idx(-1), vn_idx(-1) {}
  explicit vertex_index(int idx) : v_idx(idx), vt_idx(idx), vn_idx(idx) {}
  vertex_index(int vidx, int vtidx, int vnidx)
      : v_idx(vidx), vt_idx(vtidx), vn_idx(vnidx) {}
};

struct tag_sizes {
  tag_sizes() : num_ints(0), num_floats(0), num_strings(0) {}
  int num_ints;
  int num_floats;
  int num_strings;
};


static inline bool operator<(const vertex_index &a, const vertex_index &b) {
  if (a.v_idx != b.v_idx)
    return (a.v_idx < b.v_idx);
  if (a.vn_idx != b.vn_idx)
    return (a.vn_idx < b.vn_idx);
  if (a.vt_idx != b.vt_idx)
    return (a.vt_idx < b.vt_idx);

  return false;
}

struct obj_shape {
  std::vector<float> v;
  std::vector<float> vn;
  std::vector<float> vt;
};


std::istream& safeGetline(std::istream& is, std::string& t)
{
    t.clear();

    
    
    
    
    

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case EOF:
            
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}

#define IS_SPACE( x ) ( ( (x) == ' ') || ( (x) == '\t') )
#define IS_DIGIT( x ) ( (unsigned int)( (x) - '0' ) < (unsigned int)10 )
#define IS_NEW_LINE( x ) ( ( (x) == '\r') || ( (x) == '\n') || ( (x) == '\0') )


static inline int fixIndex(int idx, int n) {
  if (idx > 0)
    return idx - 1;
  if (idx == 0)
    return 0;
  return n + idx; 
}

static inline std::string parseString(const char *&token) {
  std::string s;
  token += strspn(token, " \t");
  size_t e = strcspn(token, " \t\r");
  s = std::string(token, &token[e]);
  token += e;
  return s;
}

static inline int parseInt(const char *&token) {
  token += strspn(token, " \t");
  int i = atoi(token);
  token += strcspn(token, " \t\r");
  return i;
}




























static bool tryParseDouble(const char *s, const char *s_end, double *result) {
  if (s >= s_end) {
    return false;
  }

  double mantissa = 0.0;
  
  
  
  
  
  
  
  int exponent = 0;

  
  
  char sign = '+';
  char exp_sign = '+';
  char const *curr = s;

  
  int read = 0;
  
  bool end_not_reached = false;

  /*
          BEGIN PARSING.
  */

  
  if (*curr == '+' || *curr == '-') {
    sign = *curr;
    curr++;
  } else if (IS_DIGIT(*curr)) { /* Pass through. */
  } else {
    goto fail;
  }

  
  while ((end_not_reached = (curr != s_end)) && IS_DIGIT(*curr)) {
    mantissa *= 10;
    mantissa += static_cast<int>(*curr - 0x30);
    curr++;
    read++;
  }

  
  if (read == 0)
    goto fail;
  
  if (!end_not_reached)
    goto assemble;

  
  if (*curr == '.') {
    curr++;
    read = 1;
    while ((end_not_reached = (curr != s_end)) && IS_DIGIT(*curr)) {
      
      mantissa += static_cast<int>(*curr - 0x30) * pow(10.0, -read);
      read++;
      curr++;
    }
  } else if (*curr == 'e' || *curr == 'E') {
  } else {
    goto assemble;
  }

  if (!end_not_reached)
    goto assemble;

  
  if (*curr == 'e' || *curr == 'E') {
    curr++;
    
    if ((end_not_reached = (curr != s_end)) && (*curr == '+' || *curr == '-')) {
      exp_sign = *curr;
      curr++;
    } else if (IS_DIGIT(*curr)) { /* Pass through. */
    } else {
      
      goto fail;
    }

    read = 0;
    while ((end_not_reached = (curr != s_end)) && IS_DIGIT(*curr)) {
      exponent *= 10;
      exponent += static_cast<int>(*curr - 0x30);
      curr++;
      read++;
    }
    exponent *= (exp_sign == '+' ? 1 : -1);
    if (read == 0)
      goto fail;
  }

assemble:
  *result =
      (sign == '+' ? 1 : -1) * ldexp(mantissa * pow(5.0, exponent), exponent);
  return true;
fail:
  return false;
}
static inline float parseFloat(const char *&token) {
  token += strspn(token, " \t");
#ifdef TINY_OBJ_LOADER_OLD_FLOAT_PARSER
  float f = (float)atof(token);
  token += strcspn(token, " \t\r");
#else
  const char *end = token + strcspn(token, " \t\r");
  double val = 0.0;
  tryParseDouble(token, end, &val);
  float f = static_cast<float>(val);
  token = end;
#endif
  return f;
}

static inline void parseFloat2(float &x, float &y, const char *&token) {
  x = parseFloat(token);
  y = parseFloat(token);
}

static inline void parseFloat3(float &x, float &y, float &z,
                               const char *&token) {
  x = parseFloat(token);
  y = parseFloat(token);
  z = parseFloat(token);
}

static tag_sizes parseTagTriple(const char *&token) {
  tag_sizes ts;

  ts.num_ints = atoi(token);
  token += strcspn(token, "/ \t\r");
  if (token[0] != '/') {
    return ts;
  }
  token++;

  ts.num_floats = atoi(token);
  token += strcspn(token, "/ \t\r");
  if (token[0] != '/') {
    return ts;
  }
  token++;

  ts.num_strings = atoi(token);
  token += strcspn(token, "/ \t\r") + 1;

  return ts;
}


static vertex_index parseTriple(const char *&token, int vsize, int vnsize,
                                int vtsize) {
  vertex_index vi(-1);

  vi.v_idx = fixIndex(atoi(token), vsize);
  token += strcspn(token, "/ \t\r");
  if (token[0] != '/') {
    return vi;
  }
  token++;

  
  if (token[0] == '/') {
    token++;
    vi.vn_idx = fixIndex(atoi(token), vnsize);
    token += strcspn(token, "/ \t\r");
    return vi;
  }

  
  vi.vt_idx = fixIndex(atoi(token), vtsize);
  token += strcspn(token, "/ \t\r");
  if (token[0] != '/') {
    return vi;
  }

  
  token++; 
  vi.vn_idx = fixIndex(atoi(token), vnsize);
  token += strcspn(token, "/ \t\r");
  return vi;
}

static unsigned int
updateVertex(std::map<vertex_index, unsigned int> &vertexCache,
             std::vector<float> &positions, std::vector<float> &normals,
             std::vector<float> &texcoords,
             const std::vector<float> &in_positions,
             const std::vector<float> &in_normals,
             const std::vector<float> &in_texcoords, const vertex_index &i) {
  const std::map<vertex_index, unsigned int>::iterator it = vertexCache.find(i);

  if (it != vertexCache.end()) {
    
    return it->second;
  }

  assert(in_positions.size() > static_cast<unsigned int>(3 * i.v_idx + 2));

  positions.push_back(in_positions[3 * static_cast<size_t>(i.v_idx) + 0]);
  positions.push_back(in_positions[3 * static_cast<size_t>(i.v_idx) + 1]);
  positions.push_back(in_positions[3 * static_cast<size_t>(i.v_idx) + 2]);

  if ((i.vn_idx >= 0) &&
      (static_cast<size_t>(i.vn_idx * 3 + 2) < in_normals.size())) {
    normals.push_back(in_normals[3 * static_cast<size_t>(i.vn_idx) + 0]);
    normals.push_back(in_normals[3 * static_cast<size_t>(i.vn_idx) + 1]);
    normals.push_back(in_normals[3 * static_cast<size_t>(i.vn_idx) + 2]);
  }

  if ((i.vt_idx >= 0) &&
      (static_cast<size_t>(i.vt_idx * 2 + 1) < in_texcoords.size())) {
    texcoords.push_back(in_texcoords[2 * static_cast<size_t>(i.vt_idx) + 0]);
    texcoords.push_back(in_texcoords[2 * static_cast<size_t>(i.vt_idx) + 1]);
  }

  unsigned int idx = static_cast<unsigned int>(positions.size() / 3 - 1);
  vertexCache[i] = idx;

  return idx;
}

static void InitMaterial(material_t &material) {
  material.name = "";
  material.ambient_texname = "";
  material.diffuse_texname = "";
  material.specular_texname = "";
  material.specular_highlight_texname = "";
  material.bump_texname = "";
  material.displacement_texname = "";
  material.alpha_texname = "";
  for (int i = 0; i < 3; i++) {
    material.ambient[i] = 0.f;
    material.diffuse[i] = 0.f;
    material.specular[i] = 0.f;
    material.transmittance[i] = 0.f;
    material.emission[i] = 0.f;
  }
  material.illum = 0;
  material.dissolve = 1.f;
  material.shininess = 1.f;
  material.ior = 1.f;
  material.unknown_parameter.clear();
}

static bool exportFaceGroupToShape(
    shape_t &shape, std::map<vertex_index, unsigned int> vertexCache,
    const std::vector<float> &in_positions,
    const std::vector<float> &in_normals,
    const std::vector<float> &in_texcoords,
    const std::vector<std::vector<vertex_index> > &faceGroup,
    std::vector<tag_t> &tags, const int material_id, const std::string &name,
    bool clearCache, unsigned int flags, std::string& err ) {
  if (faceGroup.empty()) {
    return false;
  }

  bool triangulate( ( flags & triangulation ) == triangulation );
  bool normals_calculation( ( flags & calculate_normals ) == calculate_normals );

  
  for (size_t i = 0; i < faceGroup.size(); i++) {
    const std::vector<vertex_index> &face = faceGroup[i];

    vertex_index i0 = face[0];
    vertex_index i1(-1);
    vertex_index i2 = face[1];

    size_t npolys = face.size();

    if (triangulate) {

      
      for (size_t k = 2; k < npolys; k++) {
        i1 = i2;
        i2 = face[k];

        unsigned int v0 = updateVertex(
            vertexCache, shape.mesh.positions, shape.mesh.normals,
            shape.mesh.texcoords, in_positions, in_normals, in_texcoords, i0);
        unsigned int v1 = updateVertex(
            vertexCache, shape.mesh.positions, shape.mesh.normals,
            shape.mesh.texcoords, in_positions, in_normals, in_texcoords, i1);
        unsigned int v2 = updateVertex(
            vertexCache, shape.mesh.positions, shape.mesh.normals,
            shape.mesh.texcoords, in_positions, in_normals, in_texcoords, i2);

        shape.mesh.indices.push_back(v0);
        shape.mesh.indices.push_back(v1);
        shape.mesh.indices.push_back(v2);

        shape.mesh.num_vertices.push_back(3);
        shape.mesh.material_ids.push_back(material_id);
      }
    } else {

      for (size_t k = 0; k < npolys; k++) {
        unsigned int v =
            updateVertex(vertexCache, shape.mesh.positions, shape.mesh.normals,
                         shape.mesh.texcoords, in_positions, in_normals,
                         in_texcoords, face[k]);

        shape.mesh.indices.push_back(v);
      }

      shape.mesh.num_vertices.push_back(static_cast<unsigned char>(npolys));
      shape.mesh.material_ids.push_back(material_id); 
    }
  }

  if (normals_calculation && shape.mesh.normals.empty()) {
	  const size_t nIndexs = shape.mesh.indices.size();
	  if (nIndexs % 3 == 0) {
		  shape.mesh.normals.resize(shape.mesh.positions.size());
		  for (/*register*/ size_t iIndices = 0; iIndices < nIndexs; iIndices += 3) {
			  float3 v1, v2, v3;
			  memcpy(&v1, &shape.mesh.positions[shape.mesh.indices[iIndices] * 3], sizeof(float3));
			  memcpy(&v2, &shape.mesh.positions[shape.mesh.indices[iIndices + 1] * 3], sizeof(float3));
			  memcpy(&v3, &shape.mesh.positions[shape.mesh.indices[iIndices + 2] * 3], sizeof(float3));

			  float3 v12(v1, v2);
			  float3 v13(v1, v3);

			  float3 normal = v12.crossproduct(v13);
			  normal.normalize();

			  memcpy(&shape.mesh.normals[shape.mesh.indices[iIndices] * 3], &normal, sizeof(float3));
			  memcpy(&shape.mesh.normals[shape.mesh.indices[iIndices + 1] * 3], &normal, sizeof(float3));
			  memcpy(&shape.mesh.normals[shape.mesh.indices[iIndices + 2] * 3], &normal, sizeof(float3));
		  }
	  } else {

		  std::stringstream ss;
		  ss << "WARN: The shape " << name << " does not have a topology of triangles, therfore the normals calculation could not be performed. Select the tinyobj::triangulation flag for this object." << std::endl;
		  err += ss.str();
	  }
  }

  shape.name = name;
  shape.mesh.tags.swap(tags);

  if (clearCache)
    vertexCache.clear();

  return true;
}

void LoadMtl(std::map<std::string, int> &material_map,
             std::vector<material_t> &materials, std::istream &inStream) {

  
  material_t material;
  InitMaterial(material);

  while (inStream.peek() != -1) {
    std::string linebuf;
    safeGetline(inStream, linebuf);

    
    if (linebuf.size() > 0) {
      if (linebuf[linebuf.size() - 1] == '\n')
        linebuf.erase(linebuf.size() - 1);
    }
    if (linebuf.size() > 0) {
      if (linebuf[linebuf.size() - 1] == '\r')
        linebuf.erase(linebuf.size() - 1);
    }

    
    if (linebuf.empty()) {
      continue;
    }

    
    const char *token = linebuf.c_str();
    token += strspn(token, " \t");

    assert(token);
    if (token[0] == '\0')
      continue; 

    if (token[0] == '#')
      continue; 

    
    if ((0 == strncmp(token, "newmtl", 6)) && IS_SPACE((token[6]))) {
      
      if (!material.name.empty()) {
        material_map.insert(std::pair<std::string, int>(
            material.name, static_cast<int>(materials.size())));
        materials.push_back(material);
      }

      
      InitMaterial(material);

      
      char namebuf[TINYOBJ_SSCANF_BUFFER_SIZE];
      token += 7;
#ifdef _MSC_VER
      sscanf_s(token, "%s", namebuf, (unsigned)_countof(namebuf));
#else
      sscanf(token, "%s", namebuf);
#endif
      material.name = namebuf;
      continue;
    }

    
    if (token[0] == 'K' && token[1] == 'a' && IS_SPACE((token[2]))) {
      token += 2;
      float r, g, b;
      parseFloat3(r, g, b, token);
      material.ambient[0] = r;
      material.ambient[1] = g;
      material.ambient[2] = b;
      continue;
    }

    
    if (token[0] == 'K' && token[1] == 'd' && IS_SPACE((token[2]))) {
      token += 2;
      float r, g, b;
      parseFloat3(r, g, b, token);
      material.diffuse[0] = r;
      material.diffuse[1] = g;
      material.diffuse[2] = b;
      continue;
    }

    
    if (token[0] == 'K' && token[1] == 's' && IS_SPACE((token[2]))) {
      token += 2;
      float r, g, b;
      parseFloat3(r, g, b, token);
      material.specular[0] = r;
      material.specular[1] = g;
      material.specular[2] = b;
      continue;
    }

    
    if (token[0] == 'K' && token[1] == 't' && IS_SPACE((token[2]))) {
      token += 2;
      float r, g, b;
      parseFloat3(r, g, b, token);
      material.transmittance[0] = r;
      material.transmittance[1] = g;
      material.transmittance[2] = b;
      continue;
    }

    
    if (token[0] == 'N' && token[1] == 'i' && IS_SPACE((token[2]))) {
      token += 2;
      material.ior = parseFloat(token);
      continue;
    }

    
    if (token[0] == 'K' && token[1] == 'e' && IS_SPACE(token[2])) {
      token += 2;
      float r, g, b;
      parseFloat3(r, g, b, token);
      material.emission[0] = r;
      material.emission[1] = g;
      material.emission[2] = b;
      continue;
    }

    
    if (token[0] == 'N' && token[1] == 's' && IS_SPACE(token[2])) {
      token += 2;
      material.shininess = parseFloat(token);
      continue;
    }

    
    if (0 == strncmp(token, "illum", 5) && IS_SPACE(token[5])) {
      token += 6;
      material.illum = parseInt(token);
      continue;
    }

    
    if ((token[0] == 'd' && IS_SPACE(token[1]))) {
      token += 1;
      material.dissolve = parseFloat(token);
      continue;
    }
    if (token[0] == 'T' && token[1] == 'r' && IS_SPACE(token[2])) {
      token += 2;
      
      material.dissolve = 1.0f - parseFloat(token);
      continue;
    }

    
    if ((0 == strncmp(token, "map_Ka", 6)) && IS_SPACE(token[6])) {
      token += 7;
      material.ambient_texname = token;
      continue;
    }

    
    if ((0 == strncmp(token, "map_Kd", 6)) && IS_SPACE(token[6])) {
      token += 7;
      material.diffuse_texname = token;
      continue;
    }

    
    if ((0 == strncmp(token, "map_Ks", 6)) && IS_SPACE(token[6])) {
      token += 7;
      material.specular_texname = token;
      continue;
    }

    
    if ((0 == strncmp(token, "map_Ns", 6)) && IS_SPACE(token[6])) {
      token += 7;
      material.specular_highlight_texname = token;
      continue;
    }

    
    if ((0 == strncmp(token, "map_bump", 8)) && IS_SPACE(token[8])) {
      token += 9;
      material.bump_texname = token;
      continue;
    }

    
    if ((0 == strncmp(token, "map_d", 5)) && IS_SPACE(token[5])) {
      token += 6;
      material.alpha_texname = token;
      continue;
    }

    
    if ((0 == strncmp(token, "bump", 4)) && IS_SPACE(token[4])) {
      token += 5;
      material.bump_texname = token;
      continue;
    }

    
    if ((0 == strncmp(token, "disp", 4)) && IS_SPACE(token[4])) {
      token += 5;
      material.displacement_texname = token;
      continue;
    }

    
    const char *_space = strchr(token, ' ');
    if (!_space) {
      _space = strchr(token, '\t');
    }
    if (_space) {
      std::ptrdiff_t len = _space - token;
      std::string key(token, static_cast<size_t>(len));
      std::string value = _space + 1;
      material.unknown_parameter.insert(
          std::pair<std::string, std::string>(key, value));
    }
  }
  
  material_map.insert(std::pair<std::string, int>(
      material.name, static_cast<int>(materials.size())));
  materials.push_back(material);
}

bool MaterialFileReader::operator()(const std::string &matId,
                                    std::vector<material_t> &materials,
                                    std::map<std::string, int> &matMap,
                                    std::string &err) {
  std::string filepath;

  if (!m_mtlBasePath.empty()) {
    filepath = std::string(m_mtlBasePath) + matId;
  } else {
    filepath = matId;
  }

  std::ifstream matIStream(filepath.c_str());
  LoadMtl(matMap, materials, matIStream);
  if (!matIStream) {
    std::stringstream ss;
    ss << "WARN: Material file [ " << filepath
       << " ] not found. Created a default material.";
    err += ss.str();
  }
  return true;
}

bool LoadObj(std::vector<shape_t> &shapes,       
             std::vector<material_t> &materials, 
             std::string &err, const char *filename, const char *mtl_basepath,
             unsigned int flags) {

  shapes.clear();

  std::stringstream errss;

  std::ifstream ifs(filename);
  if (!ifs) {
    errss << "Cannot open file [" << filename << "]" << std::endl;
    err = errss.str();
    return false;
  }

  std::string basePath;
  if (mtl_basepath) {
    basePath = mtl_basepath;
  }
  MaterialFileReader matFileReader(basePath);

  return LoadObj(shapes, materials, err, ifs, matFileReader, flags);
}

bool LoadObj(std::vector<shape_t> &shapes,       
             std::vector<material_t> &materials, 
             std::string &err, std::istream &inStream,
             MaterialReader &readMatFn, unsigned int flags) {

  std::stringstream errss;

  std::vector<float> v;
  std::vector<float> vn;
  std::vector<float> vt;
  std::vector<tag_t> tags;
  std::vector<std::vector<vertex_index> > faceGroup;
  std::string name;

  
  std::map<std::string, int> material_map;
  std::map<vertex_index, unsigned int> vertexCache;
  int material = -1;

  shape_t shape;

  while (inStream.peek() != -1) {
    std::string linebuf;
    safeGetline(inStream, linebuf);

    
    if (linebuf.size() > 0) {
      if (linebuf[linebuf.size() - 1] == '\n')
        linebuf.erase(linebuf.size() - 1);
    }
    if (linebuf.size() > 0) {
      if (linebuf[linebuf.size() - 1] == '\r')
        linebuf.erase(linebuf.size() - 1);
    }

    
    if (linebuf.empty()) {
      continue;
    }

    
    const char *token = linebuf.c_str();
    token += strspn(token, " \t");

    assert(token);
    if (token[0] == '\0')
      continue; 

    if (token[0] == '#')
      continue; 

    
    if (token[0] == 'v' && IS_SPACE((token[1]))) {
      token += 2;
      float x, y, z;
      parseFloat3(x, y, z, token);
      v.push_back(x);
      v.push_back(y);
      v.push_back(z);
      continue;
    }

    
    if (token[0] == 'v' && token[1] == 'n' && IS_SPACE((token[2]))) {
      token += 3;
      float x, y, z;
      parseFloat3(x, y, z, token);
      vn.push_back(x);
      vn.push_back(y);
      vn.push_back(z);
      continue;
    }

    
    if (token[0] == 'v' && token[1] == 't' && IS_SPACE((token[2]))) {
      token += 3;
      float x, y;
      parseFloat2(x, y, token);
      vt.push_back(x);
      vt.push_back(y);
      continue;
    }

    
    if (token[0] == 'f' && IS_SPACE((token[1]))) {
      token += 2;
      token += strspn(token, " \t");

      std::vector<vertex_index> face;
      face.reserve(3);

      while (!IS_NEW_LINE(token[0])) {
        vertex_index vi = parseTriple(token, static_cast<int>(v.size() / 3),
                                      static_cast<int>(vn.size() / 3),
                                      static_cast<int>(vt.size() / 2));
        face.push_back(vi);
        size_t n = strspn(token, " \t\r");
        token += n;
      }

      
      faceGroup.push_back(std::vector<vertex_index>());
      faceGroup[faceGroup.size() - 1].swap(face);

      continue;
    }

    
    if ((0 == strncmp(token, "usemtl", 6)) && IS_SPACE((token[6]))) {

      char namebuf[TINYOBJ_SSCANF_BUFFER_SIZE];
      token += 7;
#ifdef _MSC_VER
      sscanf_s(token, "%s", namebuf, (unsigned)_countof(namebuf));
#else
      sscanf(token, "%s", namebuf);
#endif

      int newMaterialId = -1;
      if (material_map.find(namebuf) != material_map.end()) {
        newMaterialId = material_map[namebuf];
      } else {
        
      }

      if (newMaterialId != material) {
        
        exportFaceGroupToShape(shape, vertexCache, v, vn, vt, faceGroup, tags,
                               material, name, true, flags, err );
        faceGroup.clear();
        material = newMaterialId;
      }

      continue;
    }

    
    if ((0 == strncmp(token, "mtllib", 6)) && IS_SPACE((token[6]))) {
      char namebuf[TINYOBJ_SSCANF_BUFFER_SIZE];
      token += 7;
#ifdef _MSC_VER
      sscanf_s(token, "%s", namebuf, (unsigned)_countof(namebuf));
#else
      sscanf(token, "%s", namebuf);
#endif

      std::string err_mtl;
      bool ok = readMatFn(namebuf, materials, material_map, err_mtl);
      err += err_mtl;

      if (!ok) {
        faceGroup.clear(); 
        return false;
      }

      continue;
    }

    
    if (token[0] == 'g' && IS_SPACE((token[1]))) {

      
      bool ret =
          exportFaceGroupToShape(shape, vertexCache, v, vn, vt, faceGroup, tags,
                                 material, name, true, flags, err );
      if (ret) {
        shapes.push_back(shape);
      }

      shape = shape_t();

      
      faceGroup.clear();

      std::vector<std::string> names;
      names.reserve(2);

      while (!IS_NEW_LINE(token[0])) {
        std::string str = parseString(token);
        names.push_back(str);
        token += strspn(token, " \t\r"); 
      }

      assert(names.size() > 0);

      
      if (names.size() > 1) {
        name = names[1];
      } else {
        name = "";
      }

      continue;
    }

    
    if (token[0] == 'o' && IS_SPACE((token[1]))) {

      
      bool ret =
          exportFaceGroupToShape(shape, vertexCache, v, vn, vt, faceGroup, tags,
                                 material, name, true, flags, err );
      if (ret) {
        shapes.push_back(shape);
      }

      
      faceGroup.clear();
      shape = shape_t();

      
      char namebuf[TINYOBJ_SSCANF_BUFFER_SIZE];
      token += 2;
#ifdef _MSC_VER
      sscanf_s(token, "%s", namebuf, (unsigned)_countof(namebuf));
#else
      sscanf(token, "%s", namebuf);
#endif
      name = std::string(namebuf);

      continue;
    }

    if (token[0] == 't' && IS_SPACE(token[1])) {
      tag_t tag;

      char namebuf[4096];
      token += 2;
#ifdef _MSC_VER
      sscanf_s(token, "%s", namebuf, (unsigned)_countof(namebuf));
#else
      sscanf(token, "%s", namebuf);
#endif
      tag.name = std::string(namebuf);

      token += tag.name.size() + 1;

      tag_sizes ts = parseTagTriple(token);

      tag.intValues.resize(static_cast<size_t>(ts.num_ints));

      for (size_t i = 0; i < static_cast<size_t>(ts.num_ints); ++i) {
        tag.intValues[i] = atoi(token);
        token += strcspn(token, "/ \t\r") + 1;
      }

      tag.floatValues.resize(static_cast<size_t>(ts.num_floats));
      for (size_t i = 0; i < static_cast<size_t>(ts.num_floats); ++i) {
        tag.floatValues[i] = parseFloat(token);
        token += strcspn(token, "/ \t\r") + 1;
      }

      tag.stringValues.resize(static_cast<size_t>(ts.num_strings));
      for (size_t i = 0; i < static_cast<size_t>(ts.num_strings); ++i) {
        char stringValueBuffer[4096];

#ifdef _MSC_VER
        sscanf_s(token, "%s", stringValueBuffer, (unsigned)_countof(stringValueBuffer));
#else
        sscanf(token, "%s", stringValueBuffer);
#endif
        tag.stringValues[i] = stringValueBuffer;
        token += tag.stringValues[i].size() + 1;
      }

      tags.push_back(tag);
    }

    
  }

  bool ret = exportFaceGroupToShape(shape, vertexCache, v, vn, vt, faceGroup,
                                    tags, material, name, true, flags, err );
  if (ret) {
    shapes.push_back(shape);
  }
  faceGroup.clear(); 

  err += errss.str();

  return true;
}

} 

#endif

#endif _