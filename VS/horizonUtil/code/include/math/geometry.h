#ifndef GEOMETRY_GUARD
#define GEOMETRY_GUARD
#include "math/vec.h"
#include "util/globals_util.h"

bool DLLAPI_UTIL ray_plane_intersection(vec3 start, vec3 dir, vec3 A, vec3 B, vec3 C, vec3 *result);
bool DLLAPI_UTIL ray_plane_intersection(vec3 start, vec3 dir, vec3 pnormal, float poffset, vec3 *result);
vec3 DLLAPI_UTIL baricentric_coords(vec3 A, vec3 B, vec3 C, vec3 point);
vec4 DLLAPI_UTIL baricentric_coords(vec3 A, vec3 B, vec3 C, vec3 D, vec3 point);
bool DLLAPI_UTIL ray_triangle_intersection(vec3 start, vec3 dir, vec3 A, vec3 B, vec3 C, vec3 *result, vec2 *barycentric);
vec3 DLLAPI_UTIL triangle_normal(vec3 A, vec3 B, vec3 C);
float DLLAPI_UTIL point_line_distance(vec3 start, vec3 end, vec3 A);
float DLLAPI_UTIL point_plane_distance(vec3 A, vec3 B, vec3 C, vec3 P);
vec3 DLLAPI_UTIL project(vec3 A, vec3 B);
vec3 DLLAPI_UTIL point_line_closest(vec3 start, vec3 end, vec3 A, bool clip = false);
#endif