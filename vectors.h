//vectors.h

struct vec
{
	double x;
	double y;
	double z;
	vec operator + (vec B){return {x+B.x, y+B.y, z+B.z};}
	vec operator - (vec B){return {x-B.x, y-B.y, z-B.z};}
	vec operator - (){return {-x,-y,-z};}
	vec operator * (double b){return {x*b,y*b,z*b};}
	vec operator / (double b){return {x/b,y/b,z/b};}
	double dot(vec B){return {x*B.x+y*B.y+z*B.z};}
	vec cross(vec B){return {y*B.z-z*B.y, z*B.x-x*B.z, x*B.y-y*B.x};}
	double length(){return sqrt(pow(x,2)+pow(y,2)+pow(z,2));}
	vec norm(){return *this/length();}
	double scalarProjection(vec B){return dot(B)/B.length();}
	vec vectorProjection(vec B){return B.norm()*scalarProjection(B);}
};