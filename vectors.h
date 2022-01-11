//vectors.h

struct vector
{
	double x;
	double y;
	double z;
	vector operator + (vector B){return {x+B.x, y+B.y, z+B.z};}
	vector operator - (vector B){return {x-B.x, y-B.y, z-B.z};}
	vector operator - (){return {-x,-y,-z};}
	vector operator * (double b){return {x*b,y*b,z*b};}
	vector operator / (double b){return {x/b,y/b,z/b};}
	double dot(vector B){return {x*B.x+y*B.y+z*B.z};}
	vector cross(vector B){return {y*B.z-z*B.y, z*B.x-x*B.z, x*B.y-y*B.x};}
	double length(){return sqrt(pow(x,2)+pow(y,2)+pow(z,2));}
	vector norm(){return *this/length();}
	double scalarProjection(vector B){return dot(B)/B.length();}
	vector vectorProjection(vector B){return B.norm()*scalarProjection(B);}
};