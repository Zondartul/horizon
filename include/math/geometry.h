#ifndef GEOMETRY_GUARD
#define GEOMETRY_GUARD
#include "vec.h"

//true if the ray intersects the plane, false otherwise.
//output <result> - intersection point
bool ray_plane_intersection(vec3 start, vec3 dir, vec3 A, vec3 B, vec3 C, vec3 *result);

//returns barycentric coordinates (x=dist to A,y=dist to B,z=dist to C) of <point>.
//pick two for UV coords.
//<point> has to be in plane with the triangle.
vec3 baricentric_coords(vec3 A, vec3 B, vec3 C, vec3 point);

//same, but coords are (x to A,y to B,z to C, w to D).
vec4 baricentric_coords(vec3 A, vec3 B, vec3 C, vec3 D, vec3 point);

//true if the ray intersects the plane, and the intersection is inside the triangle.
//output <result> - intersection point
//output <barycentric> - berycentric coords of the point.
bool ray_triangle_intersection(vec3 start, vec3 dir, vec3 A, vec3 B, vec3 C, vec3 *result, vec2 *barycentric);

//returns the distance from point A to the line.
float point_line_distance(vec3 start, vec3 end, vec3 A);

//returns the distance from point P to the plane.
//negative distance corresponds to the point being on the other side of the plane.
float point_plane_distance(vec3 A, vec3 B, vec3 C, vec3 P);

//project line OA onto line OB to get line OC, where O is (0,0,0)
vec3 project(vec3 A, vec3 B);

//returns the point on line that is closest to the point A.
//if <clip> is set, the closest point is limited to the line segment.
vec3 point_line_closest(vec3 start, vec3 end, vec3 A, bool clip = false);


#endif