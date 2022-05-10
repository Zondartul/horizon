#ifndef GEOMETRY_GUARD
#define GEOMETRY_GUARD
#include "vec.h"
bool ray_plane_intersection(vec3 start, vec3 dir, vec3 A, vec3 B, vec3 C, vec3 *result);
bool ray_plane_intersection(vec3 start, vec3 dir, vec3 pnormal, float poffset, vec3 *result);
vec3 baricentric_coords(vec3 A, vec3 B, vec3 C, vec3 point);
vec4 baricentric_coords(vec3 A, vec3 B, vec3 C, vec3 D, vec3 point);
bool ray_triangle_intersection(vec3 start, vec3 dir, vec3 A, vec3 B, vec3 C, vec3 *result, vec2 *barycentric);
vec3 triangle_normal(vec3 A, vec3 B, vec3 C);
float point_line_distance(vec3 start, vec3 end, vec3 A);
float point_plane_distance(vec3 A, vec3 B, vec3 C, vec3 P);
vec3 project(vec3 A, vec3 B);
vec3 point_line_closest(vec3 start, vec3 end, vec3 A, bool clip = false);
#endif