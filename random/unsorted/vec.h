#ifndef VEC_GUARD
#define VEC_GUARD

struct vec3f{
	float x;
	float y;
	float z;
	float dot(vec3f B);
	vec3f proj(vec3f B);
	vec3f cross(vec3f B);
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

struct vec3ub{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};
bool operator==(vec3ub A, vec3ub B);
vec3ub operator*(vec3ub A, float N);
vec3ub operator/(vec3ub A, float N);
vec3ub operator*(float N, vec3ub A);
vec3ub operator/(float N, vec3ub A);
vec3ub operator+(vec3ub A, vec3ub B);
vec3ub operator-(vec3ub A, vec3ub B);

struct vec2f;
struct vec2i{
	int x;
	int y;
	operator vec2f();
};
vec2i operator-(vec2i A);
vec2i clamp(vec2i A, vec2i min, vec2i max);
bool operator==(vec2i A, vec2i B);
vec2i operator*(vec2i A, float N);
vec2i operator/(vec2i A, float N);
vec2i operator*(float N, vec2i A);
vec2i operator/(float N, vec2i A);
vec2i operator+(vec2i A, vec2i B);
vec2i operator-(vec2i A, vec2i B);

struct vec2f{
	float x;
	float y;
	operator vec2i();
};
vec2f clamp(vec2f A, vec2f min, vec2f max);
bool operator==(vec2f A, vec2f B);
vec2f operator*(vec2f A, float N);
vec2f operator/(vec2f A, float N);
vec2f operator*(float N, vec2f A);
vec2f operator/(float N, vec2f A);
vec2f operator+(vec2f A, vec2f B);
vec2f operator-(vec2f A, vec2f B);

struct rect{
	vec2i start;
	vec2i end;
	vec2i size;
	rect();
	rect(int width, int height);
	rect(vec2i size);
	rect(vec2i start, vec2i end);
	rect(vec2i start, vec2i end, vec2i size);
	rect setStart(vec2i start); //end stays
	rect setEnd(vec2i end); //start stays
	rect setSize(vec2i size); //start stays
	rect moveTo(vec2i start); //size stays
	rect moveBy(vec2i relative); //size stays
	vec2i clamp(vec2i V);
	rect clamp(rect R);
	bool contains(vec2i V);
	//const char *toString();
};
rect operator*(rect A, float N);
bool operator==(rect A, rect B);

struct rectf{
	vec2f start;
	vec2f end;
	vec2f size;
	rectf();
	rectf(float width, float height);
	rectf(vec2f size);
	rectf(vec2f start, vec2f end);
	rectf(vec2f start, vec2f end, vec2f size);
	rectf setStart(vec2f start); //end stays
	rectf setEnd(vec2f end); //start stays
	rectf setSize(vec2f size); //start stays
	rectf moveTo(vec2f start); //size stays
	rectf moveBy(vec2f relative); //size stays
	vec2f clamp(vec2f V);
	rectf clamp(rectf R);
	bool contains(vec2f V);
	//const char *toString();
};
rectf operator*(rectf A, float N);

struct AABB{
	vec3f start;
	vec3f end;
	vec3f size;
	AABB();
	AABB(int length, int width, int height);
	AABB(vec3f size);
	AABB(vec3f start, vec3f end);
	AABB(vec3f start, vec3f end, vec3f size);
	AABB setStart(vec3f start); //end stays
	AABB setEnd(vec3f end); //start stays
	AABB setSize(vec3f size); //start stays
	AABB moveTo(vec3f start); //size stays
	AABB moveBy(vec3f relative); //size stays
};
#endif