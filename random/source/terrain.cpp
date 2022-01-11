#include "terrain.h"
#include "globals.h"
#include "textureLoader.h"
#include "selfaware.h"

model genSquare(float hA, float hB, float hC, float hD, vec2f start, vec2f end){
	vec3f A = {start.x,start.y,hA};
	vec3f B = {end.x,start.y,hB};
	vec3f C = {end.x,end.y,hC};
	vec3f D = {start.x,end.y,hD};
	vec2f uvA = {A.x,A.y};//{0,0};
	vec2f uvB = {B.x,B.y};//{0,1};
	vec2f uvC = {C.x,C.y};//{1,1};
	vec2f uvD = {D.x,D.y};//{1,0};
	vec3f norm = {0,0,1};
	vertex vA = {A,norm,uvA};
	vertex vB = {B,norm,uvB};
	vertex vC = {C,norm,uvC};
	vertex vD = {D,norm,uvD};
	triangle T1 = {vA,vB,vC};
	triangle T2 = {vC,vD,vA};
	model M;
	M.tris.push_back(T1);
	M.tris.push_back(T2);
	return M;
}

model generateTerrain(string name, string heightmapname, string texturename, vec3f scale, vec3f pos){
	string heightmap_path = locateResource("texture",heightmapname.c_str());
	bitmap heightmap = loadImage(heightmap_path.c_str());
	heightmap = heightmap.changeFormat(TL_LUMINANCE);
	
	model M;
	for(int x = 0; x < heightmap.height-1; x++){
		for(int y = 0; y < heightmap.width-1; y++){
			pixel P;
			P = heightmap.read_pixel({x,y});
			float hA = scale.z*(P.R) / (255.0) + pos.z;
			P = heightmap.read_pixel({x+1,y});
			float hB = scale.z*(P.R) / (255.0) + pos.z;
			P = heightmap.read_pixel({x+1,y+1});
			float hC = scale.z*(P.R) / (255.0) + pos.z;
			P = heightmap.read_pixel({x,y+1});
			float hD = scale.z*(P.R) / (255.0) + pos.z;
			M.add(genSquare(hA,hB,hC,hD,
					{(x+0.5)*scale.x/heightmap.height-scale.x/2.0 + pos.x,(y+0.5)*scale.y/heightmap.width-scale.y/2.0 + pos.y},
					{(x+1.5)*scale.x/heightmap.height-scale.x/2.0 + pos.x,(y+1.5)*scale.y/heightmap.width-scale.y/2.0 + pos.y}));
		}
	}
	M.recalculateNormals();
	M.t = getTexture(texturename);
	modelCache[name] = M;
	return M;
}