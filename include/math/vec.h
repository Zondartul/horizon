#ifndef VEC_GUARD
#define VEC_GUARD
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
using glm::mat4;
using glm::vec4;
using glm::vec3;
using glm::vec2;

/*
struct vec3f{
	float x;
	float y;
	float z;
	float dot(vec3 B);
	vec3f proj(vec3 B);
	vec3f cross(vec3 B);
	vec3f norm();
	float length();
	vec3f rotate(vec3f axis, float angle); //rotate around axis, by angle in degrees
	vec3f rotate(vec3f angle); //rotate along each of the 3 axes using default order
};

bool operator==(vec3f A, vec3f B);
vec3f operator*(vec3f A, float N);
vec3f operator/(vec3f A, float N);
vec3f operator*(float N, vec3f A);
vec3f operator/(float N, vec3f A);
vec3f operator+(vec3f A, vec3f B);
vec3f operator-(vec3f A, vec3f B);

struct vec3f{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};
bool operator==(vec3f A, vec3f B);
vec3f operator*(vec3f A, float N);
vec3f operator/(vec3f A, float N);
vec3f operator*(float N, vec3f A);
vec3f operator/(float N, vec3f A);
vec3f operator+(vec3f A, vec3f B);
vec3f operator-(vec3f A, vec3f B);

struct vec2i;
struct vec2f{
	int x;
	int y;
	operator vec2i();
};
vec2f operator-(vec2 A);
vec2f clamp(vec2 A, vec2 min, vec2 fmax);
bool operator==(vec2f A, vec2f B);
vec2f operator*(vec2f A, float N);
vec2f operator/(vec2f A, float N);
vec2f operator*(float N, vec2f A);
vec2f operator/(float N, vec2f A);
vec2f operator+(vec2f A, vec2f B);
vec2f operator-(vec2f A, vec2f B);

struct vec2i{
	float x;
	float y;
	operator vec2f();
};
vec2i clamp(vec2i A, vec2i min, vec2i max);
bool operator==(vec2i A, vec2i B);
vec2i operator*(vec2i A, float N);
vec2i operator/(vec2i A, float N);
vec2i operator*(float N, vec2i A);
vec2i operator/(float N, vec2i A);
vec2i operator+(vec2i A, vec2i B);
vec2i operator-(vec2i A, vec2i B);
*/

//vanilla glm normalize(vec3) evaluates to NaN if length(vec3) is 0
//for that reason, overriding normalize to normalizeSafe
//though conflict with non-vec3 normalize functions is possible
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

struct rect{
	vec2 start;
	vec2 end;
	vec2 size;
	rect();
	rect(int width, int height);
	rect(vec2 size);
	rect(vec2 start, vec2 end);
	rect(vec2 start, vec2 end, vec2 size);
	rect setStart(vec2 start); //end stays
	rect setEnd(vec2 end); //start stays
	rect setSize(vec2 size); //start stays
	rect moveTo(vec2 start); //size stays
	rect moveBy(vec2 relative); //size stays
	rect repair(); //put start in the top-left, put end in the bottom-right
	vec2 clamp(vec2 V);
	rect clamp(rect R);
	bool contains(vec2 V);
	vec2 topLeftCorner();
	vec2 topRightCorner();
	vec2 bottomLeftCorner();
	vec2 bottomRightCorner();
	vec2 center();
	//const char *toString();
};
rect operator*(rect A, float N);
bool operator==(rect A, rect B);

/*
struct rectf{
	vec2 start;
	vec2 end;
	vec2 size;
	rectf();
	rectf(float width, float height);
	rectf(vec2 size);
	rectf(vec2 start, vec2 end);
	rectf(vec2 start, vec2 end, vec2 size);
	rectf setStart(vec2 start); //end stays
	rectf setEnd(vec2 end); //start stays
	rectf setSize(vec2 size); //start stays
	rectf moveTo(vec2 start); //size stays
	rectf moveBy(vec2 relative); //size stays
	vec2 clamp(vec2 V);
	rectf clamp(rectf R);
	bool contains(vec2 V);
	//const char *toString();
};
rectf operator*(rectf A, float N);
*/

struct AABB{
	vec3 start;
	vec3 end;
	vec3 size;
	AABB();
	AABB(int length, int width, int height);
	AABB(vec3 size);
	AABB(vec3 start, vec3 end);
	AABB(vec3 start, vec3 end, vec3 size);
	AABB setStart(vec3 start); //end stays
	AABB setEnd(vec3 end); //start stays
	AABB setSize(vec3 size); //start stays
	AABB moveTo(vec3 start); //size stays
	AABB moveBy(vec3 relative); //size stays
};


//vec3 toVec3(vec3 V);
//vec2 toVec2(vec2 V);
//vec3 tovec3(vec3 V);
//vec2 tovec2(vec2 V);
#endif