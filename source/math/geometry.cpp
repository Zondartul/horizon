#include "geometry.h"
#include "stdio.h"
#include "simplemath.h"

bool ray_plane_intersection(vec3 start, vec3 dir, vec3 A, vec3 B, vec3 C, vec3 *result){
	//Moller-Trumbore intersection algorythm
	vec3 e1, e2;
	vec3 P, Q, T;
	double det, inv_det, u, v;
	float t;
	
	e1 = B-A;
	e2 = C-A;
	P = cross(dir,e2);//dir.cross(e2);
	det = dot(e1,P);//e1.dot(P);
	//printf("det = %f\n",det);
	//this auto-fails if the ray is nearly in the triangle's plane
	//if(det > -EPSILON && det < EPSILON){return 0;}
	inv_det = 1.0f / det;
	//printf("inv_det = %f\n",inv_det);
	//skipping the inside-outside check
	T = start-A;
	//u = T.dot(P) * inv_det;
	////intersection is outside the triangle
	//if(u < 0.0f || u > 1.0f){return 0;}
	Q = cross(T,e1);//Q = T.cross(e1);
	//v = dir.dot(Q) * inv_det;
	////intersection is outside the triangle
	//if(v < 0.0f || u + v > 1.0f){return 0;}
	t = dot(e2,Q)*inv_det;//e2.dot(Q) * inv_det;
	//printf("dot(e2,Q) = %f\n",dot(e2,Q));
	if(t > 0){//if(t > EPSILON)
		//printf("distance = %f\n", t);
		if(result){*result = start+dir*t;}
		return 1;
	}
	//printf("t = %f\n",t);
	return 0;
}

vec3 baricentric_coords(vec3 A, vec3 B, vec3 C, vec3 point){
	return vec3(length(A-point),length(B-point),length(C-point));
}

vec4 baricentric_coords(vec3 A, vec3 B, vec3 C, vec3 D, vec3 point){
	return vec4(length(A-point),length(B-point),length(C-point),length(D-point));
}

bool ray_triangle_intersection(vec3 start, vec3 dir, vec3 A, vec3 B, vec3 C, vec3 *result, vec2 *barycentric){
	vec3 R1;
	if(!ray_plane_intersection(start,dir,A,B,C,&R1)){printf("no intersect\n");return false;}
	vec3 bar = baricentric_coords(A,B,C,R1);
	if((bar.x > 0.f && bar.x < 1.f)
	&&(bar.y > 0.f && bar.y < 1.f)
	&&(bar.z > 0.f && bar.z < 1.f)){
		//point is in triangle
		if(result){*result = R1;}
		if(barycentric){*barycentric = bar;}
		return true;
	}else{
		printf("outside triangle\n");
		return false;
	}
}

float point_line_distance(vec3 start, vec3 end, vec3 A){
	return length(cross((A-start),(A-end)))/length(end-start);
}

vec3 plane_normal(vec3 A, vec3 B, vec3 C){
	return normalize(cross(B-A,C-A));
}

float point_plane_distance(vec3 A, vec3 B, vec3 C, vec3 P){
	vec3 n = plane_normal(A, B, C);
	return dot((P-A),n);
}

vec3 project(vec3 A, vec3 B){
	return normalize(B)*dot(A,normalize(B));
}

vec3 point_line_closest(vec3 start, vec3 end, vec3 A, bool clip){
	//copying GLM's implementation
	float len = length(end-start);
	vec3 dir = normalize(end-start);
	vec3 V = A-start;
	float dist = dot(V,dir);
	if(clip && dist < 0){return start;}
	if(clip && dist > len){return end;}
	return start+dir*dist;
}








