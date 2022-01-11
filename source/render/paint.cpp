#include "paint.h"
#include "stdio.h"
#include "globals.h"

void setTexture(texture t){
	renderCmd(RCMD::TEXTURE_SELECT,i=t.handle.t);
}


void drawTexturedRectUV(rect area,rectf UV){
	rmodel rm;
	vec3 A = vec3(area.start.x,area.start.y,0.0f)*0.05f;
	vec3 B = vec3(area.start.x,area.end.y,0.0f)*0.05f;
	vec3 C = vec3(area.end.x,area.start.y,0.0f)*0.05f;
	vec3 D = vec3(area.end.x,area.end.y,0.0f)*0.05f;
	vec2 UVA = vec2(UV.start.x,UV.start.y);//*0.1f;
	vec2 UVB = vec2(UV.start.x,UV.end.y);//*0.1f;
	vec2 UVC = vec2(UV.end.x,UV.start.y);//*0.1f;
	vec2 UVD = vec2(UV.end.x,UV.end.y);//*0.1f;
	rm.vertices->push_back(A);
	rm.vertices->push_back(B);
	rm.vertices->push_back(C);
	rm.vertices->push_back(B);
	rm.vertices->push_back(C);
	rm.vertices->push_back(D);
	
	rm.uvs->push_back(UVA);
	rm.uvs->push_back(UVB);
	rm.uvs->push_back(UVC);
	rm.uvs->push_back(UVB);
	rm.uvs->push_back(UVC);
	rm.uvs->push_back(UVD);
	
	
	rm.colors->push_back(vec3(1,0,0));
	rm.colors->push_back(vec3(0,1,0));
	rm.colors->push_back(vec3(0,0,1));
	rm.colors->push_back(vec3(1,0,0));
	rm.colors->push_back(vec3(0,1,0));
	rm.colors->push_back(vec3(0,0,1));
	
	once(printf("A = (%f,%f,%f)\n",A.x,A.y,A.z));
	once(printf("B = (%f,%f,%f)\n",B.x,B.y,B.z));
	once(printf("C = (%f,%f,%f)\n",C.x,C.y,C.z));
	once(printf("D = (%f,%f,%f)\n",D.x,D.y,D.z));
	
	once(printf("UVA = (%f,%f)\n",UVA.x,UVA.y));
	once(printf("UVB = (%f,%f)\n",UVB.x,UVB.y));
	once(printf("UVC = (%f,%f)\n",UVC.x,UVC.y));
	once(printf("UVD = (%f,%f)\n",UVD.x,UVD.y));
	
	uploadModel(&rm);
	
	renderCmd(RCMD::COLORING,b=false);
	renderCmd(RCMD::TEXTURING,b=true);
	renderModel(RCMD::DRAW_TRIANGLES,&rm);
	//unloadModel(&rm);
}