#ifndef VEC_GUARD
#define VEC_GUARD
#pragma warning(push, 0)
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtc/quaternion.hpp"
#pragma warning(pop)
#include "globals.h"
#include <string>
using std::string;
using glm::mat4;
using glm::mat3;
using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::quat;
string toString(quat q);
quat toQuat(vec3 angleVec);
quat slerpAngle(quat Q1, quat Q2, float angle); 
#ifndef oldNormalize
#define normalize(x) normalizeSafe(x)
#endif
vec3 rotate(vec3 V, vec3 Angle);
vec3 clamp(vec3 A, vec3 min, vec3 max);
vec3 operator/(vec3 A, float N);
vec3 operator*(vec3 A, float N);
vec2 clamp(vec2 A, vec2 min, vec2 max);
vec2 operator/(vec2 A, float N);
vec2 operator*(vec2 A, float N);
vec2 normalizeSafe(vec2 V);
vec3 normalizeSafe(vec3 V);
vec2 toVec2(vec3 A);
vec3 toVec3(vec2 A,float z = 0.f);
vec3 setX(vec3 A, float x);
vec3 setY(vec3 A, float y);
vec3 setZ(vec3 A, float z);
float cross(vec2 A, vec2 B);
vec3 toVec3Angle(quat q);
vec3 toVec3Angle(vec3 fwd);
struct DLLAPI rect{
	vec2 start;
	vec2 end;
	vec2 size;
	rect();
	rect(int width, int height);
	rect(vec2 size);
	rect(vec2 start, vec2 end);
	rect(vec2 start, vec2 end, vec2 size);
	[[nodiscard]]rect setStart(vec2 start); 
	[[nodiscard]]rect setEnd(vec2 end); 
	[[nodiscard]]rect setSize(vec2 size); 
	[[nodiscard]]rect moveTo(vec2 start); 
	[[nodiscard]]rect moveBy(vec2 relative); 
	[[nodiscard]]rect repair(); 
	[[nodiscard]]vec2 clamp(vec2 V);
	[[nodiscard]]rect clamp(rect R);
	bool contains(vec2 V);
	vec2 topLeftCorner();
	vec2 topRightCorner();
	vec2 bottomLeftCorner();
	vec2 bottomRightCorner();
	vec2 center();
};
rect operator*(rect A, float N);
bool operator==(rect A, rect B);
struct AABB{
	vec3 start;
	vec3 end;
	vec3 size;
	AABB();
	AABB(int length, int width, int height);
	AABB(vec3 size);
	AABB(vec3 start, vec3 end);
	AABB(vec3 start, vec3 end, vec3 size);
	AABB setStart(vec3 start); 
	AABB setEnd(vec3 end); 
	AABB setSize(vec3 size); 
	AABB moveTo(vec3 start); 
	AABB moveBy(vec3 relative); 
	bool contains(vec3 point);
};
mat3 xrotr(float ang);
mat3 yrotr(float ang);
mat3 zrotr(float ang);
#define vec3_min(V1,V2) vec3(min((V1).x,(V2).x),min((V1).y,(V2).y),min((V1).z,(V2).z))
#define vec3_max(V1,V2) vec3(max((V1).x,(V2).x),max((V1).y,(V2).y),max((V1).z,(V2).z))
#endif
