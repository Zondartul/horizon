//vectors.h

struct vec
{
	double x;
	double y;
	double z;
	vec(double x1,double y1,double z1){x=x1;y=y1;z=z1;}
	vec(){}
	vec operator + (vec B){return {x+B.x, y+B.y, z+B.z};}
	vec operator - (vec B){return {x-B.x, y-B.y, z-B.z};}
	vec operator - (){return {-x,-y,-z};}
	vec operator * (double b){return {x*b,y*b,z*b};}
	vec operator * (vec B){return {x*B.x,y*B.y,z*B.z};}
	vec operator / (double b){return {x/b,y/b,z/b};}
	double dot(vec B){return {x*B.x+y*B.y+z*B.z};}
	vec cross(vec B){return {y*B.z-z*B.y, z*B.x-x*B.z, x*B.y-y*B.x};}
	double length(){return sqrt(pow(x,2)+pow(y,2)+pow(z,2));}
	vec norm(){return *this/length();}
	double scalarProjection(vec B){return dot(B)/B.length();}
	vec vectorProjection(vec B){return B.norm()*scalarProjection(B);}
	double angle(vec B){return acos(dot(B)/(length()*B.length()))*180/M_PI;}
	double sgnAngle(vec B, vec ref) // sign only possible when you know axis of rotation
	{return sgn(ref.dot(cross(B)))*acos(dot(B)/(length()*B.length()))*180/M_PI;}
};

vec operator * (double b, vec a){return a*b;}