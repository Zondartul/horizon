#include "geometry.h"
#include "stdio.h"
#include "simplemath.h"

bool ray_plane_intersection(vec3 start, vec3 dir, vec3 A, vec3 B, vec3 C, vec3 *result){
	
	vec3 e1, e2;
	vec3 P, Q, T;
	double det, inv_det;
	float t;
	
	e1 = B-A;
	e2 = C-A;
	P = cross(dir,e2);
	det = dot(e1,P);
	
	inv_det = 1.0f / det;
	
	T = start-A;
	Q = cross(T,e1);
	t = dot(e2,Q)*inv_det;
	if(t > 0){
		if(result){*result = start+dir*t;}
		return 1;
	}
	return 0;
}

bool ray_plane_intersection(vec3 start, vec3 dir, vec3 normal, float offset, vec3 *result){
	
	
	
	
	
	
	
	
	
	
	
	float dirness = dot(dir, normal);
	if(dirness){
		float t = (offset - dot(start, normal)) / dirness;
		if(t > 0){
			*result = start + t*dir;
			return true;
		}
	}
	return false;
}

vec3 baricentric_coords(vec3 a, vec3 b, vec3 c, vec3 p){
	
	vec3 res;
	vec3 v0 = b - a;
	vec3 v1 = c - a;
	vec3 v2 = p - a;
    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    res.x = (d11 * d20 - d01 * d21) / denom;
    res.y = (d00 * d21 - d01 * d20) / denom;
    res.z = 1.0f - res.x - res.y;
	return res;
}

vec4 baricentric_coords(vec3 A, vec3 B, vec3 C, vec3 D, vec3 point){
	return vec4(length(A-point),length(B-point),length(C-point),length(D-point));
}

bool ray_triangle_intersection(vec3 start, vec3 dir, vec3 A, vec3 B, vec3 C, vec3 *result, vec2 *barycentric){
	vec3 R1;
	if(!ray_plane_intersection(start,dir,A,B,C,&R1)){return false;}
	vec3 bar = baricentric_coords(A,B,C,R1);
	if((bar.x > 0.f && bar.x < 1.f)
	&&(bar.y > 0.f && bar.y < 1.f)
	&&(bar.z > 0.f && bar.z < 1.f)){
		
		if(result){*result = R1;}
		if(barycentric){*barycentric = bar;}
		return true;
	}else{
		return false;
	}
}

vec3 triangle_normal(vec3 A, vec3 B, vec3 C){
	return normalizeSafe(cross((B-C),(A-C)));
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
	
	float len = length(end-start);
	vec3 dir = normalize(end-start);
	vec3 V = A-start;
	float dist = dot(V,dir);
	if(clip && dist < 0){return start;}
	if(clip && dist > len){return end;}
	return start+dir*dist;
}








