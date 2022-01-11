//quaternions

struct quat
{
	double w;
	vector v;
	
	quat operator + (quat B){return {w+B.w, {v.x+B.v.x, v.y+B.v.y, v.z+B.v.z}};}
	//quat operator * (quat B){return {w*B.w-v.x*B.v.x-v.y*B.v.y-v.z*B.v.z, {w*B.v.x+v.x*B.w+v.y*B.v.z-v.z*B.v.y, w*B.v.y-v.x*B.v.z+v.y*B.w+v.z*B.v.x, w*B.v.z+v.x*B.v.y-v.y*B.v.x+v.z*B.w}};}//holy fuck we're still alive
	quat operator * (quat B){return {w*B.w-v.dot(B.v),(B.v*w+v*B.w+v.cross(B.v))};}
	quat conjugate(){return {w, -v};}
	quat inverse()
	{
		double div = pow(w,2)+pow(v.x,2)+pow(v.y,2)+pow(v.z,2);
		return {w/div,v/div};
	}
	quat addRotation(double ang, vector vec)
	{
		quat C = {cos(ang/2), vec*sin(ang/2)};
		return C*(*this)*C.inverse();
	}
};