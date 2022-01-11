#ifndef VEC_GUARD
#define VEC_GUARD
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtc/quaternion.hpp"
#include <string>
using std::string;
using glm::mat4;
using glm::mat3;
using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::quat;

/*
    documentation of GLM vec functions
    struct vec3{
        float x,y,z;
        operators [] + etc
        vec3 cross(vec3 A, vec3 B); //vector cross product
        float distance(vec3 A, vec3 B); //length(A-B)
        float dot(vec3 A, vec3 B); //vector dot product
        vec3 faceforward(vec3 N, vec3 I, vec3 Nref); //if dot(Nref,I) < 0, return N, otherwise return -N
        float length(vec3 A);// sqrt(x^2+y^2+z^2);
        vec3 normalize(vec3 A);
        vec3 reflect(vec3 I, vec3 N); //for vector I and surface normal N, returns reflection I - 2*dot(N,I)*N;
        vec3 refract(vec3 I, vec3 N, float eta); //for vector I and normal N, and refractive ratio eta, return the refraction vector.
    }

    struct quat{
        quat angleAxis(float degrees, vec3 axis);
        quat quat(vec3 EulerAngles);
        quat quat(float w, x, y, z); where w = cos(angle/2), x = x*sin(angle/2), etc.
        float dot(quat q1, quat q2); //cosine of angle between two quats (their final orientations, that is)
        //vec3 point_rotated = quat * point
        quat RotationBetweenVectors(vec3 v1, vec3 v2); //does inverse of this:
                                        //dot(p1,p2) = cos(quat.angle), cross(p1,p2) = quat.axis
        //quat interpolated_quat = quat::mix(quat1, quat2, 0.5f);
        //quat combined_rotation = second_rotation * first_rotation
        quat slerp(quat Q1, quat Q2, float factor); //spherical linear interpolation at const speed, factor between 0.0 and 1.0
    }
*/
string toString(quat q);
quat toQuat(vec3 angleVec);
quat slerpAngle(quat Q1, quat Q2, float angle); //moves at most <angle> radians from Q1 to Q2.

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
vec2 toVec2(vec3 A);
vec3 toVec3(vec2 A,float z);

vec3 setX(vec3 A, float x);
vec3 setY(vec3 A, float y);
vec3 setZ(vec3 A, float z);

float cross(vec2 A, vec2 B);
#define nodiscard gnu::warn_unused_result
vec3 toVec3Angle(quat q);
vec3 toVec3Angle(vec3 fwd);


struct rect{
	vec2 start;
	vec2 end;
	vec2 size;
	rect();
	rect(int width, int height);
	rect(vec2 size);
	rect(vec2 start, vec2 end);
	rect(vec2 start, vec2 end, vec2 size);
	//[[nodiscard]] is needed so you don't try to modify the rect with these -
	//they don't.
	[[nodiscard]]rect setStart(vec2 start); //end stays
	[[nodiscard]]rect setEnd(vec2 end); //start stays
	[[nodiscard]]rect setSize(vec2 size); //start stays
	[[nodiscard]]rect moveTo(vec2 start); //size stays
	[[nodiscard]]rect moveBy(vec2 relative); //size stays
	[[nodiscard]]rect repair(); //put start in the top-left, put end in the bottom-right
	[[nodiscard]]vec2 clamp(vec2 V);
	[[nodiscard]]rect clamp(rect R);
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
	bool contains(vec3 point);
};

//builds 3x3 rotation matrices for rotating a vec3 by ang RADIANS around basic axes.
mat3 xrotr(float ang);
mat3 yrotr(float ang);
mat3 zrotr(float ang);

//vec3 toVec3(vec3 V);
//vec2 toVec2(vec2 V);
//vec3 tovec3(vec3 V);
//vec2 tovec2(vec2 V);
#define vec3_min(V1,V2) vec3(min((V1).x,(V2).x),min((V1).y,(V2).y),min((V1).z,(V2).z))
#define vec3_max(V1,V2) vec3(max((V1).x,(V2).x),max((V1).y,(V2).y),max((V1).z,(V2).z))


//old stuff

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
#endif
