
//quaternions
/*
struct quat
{
	double w;
	vector v;
	
	quat operator + (quat B){return {w+B.w, {v.x+B.v.x, v.y+B.v.y, v.z+B.v.z}};}
	//quat operator * (quat B){return {w*B.w-v.x*B.v.x-v.y*B.v.y-v.z*B.v.z, {w*B.v.x+v.x*B.w+v.y*B.v.z-v.z*B.v.y, w*B.v.y-v.x*B.v.z+v.y*B.w+v.z*B.v.x, w*B.v.z+v.x*B.v.y-v.y*B.v.x+v.z*B.w}};}//holy fuck we're still alive
	quat operator * (quat B){return {w*B.w-v.dot(B.v),(B.v*w+v*B.w+v.cross(B.v))};}
	quat conjugate(){return {w, -v};}
	quat norm(){double mag = sqrt(pow(w,2)+pow(v.x,2)+pow(v.y,2)+pow(v.z,2)); return {w/mag,v/mag};}
	quat inverse()
	{
		double div = pow(w,2)+pow(v.x,2)+pow(v.y,2)+pow(v.z,2);
		return {w/div,-v/div};
	}
	quat addRotation(double ang, vector vec)
	{
		quat C = {cos(ang/2), vec*sin(ang/2)};
		return C*(*this)*C.inverse();
	}
	vector RotateVector(vector B)
	{
		return ((*this)*((quat){0,B})*(*this).inverse()).v;
	}
};
*/
double deg2rad = 0.0174532925199432957692;
double rad2deg = 57.295779513082320876798;

struct quat
{
	double w;
	vec v;
	
	quat operator -(){return {w,-v};}		
	quat operator *(double N){return {w*N, v};}
	quat operator *(quat B)
	{
		double lhs1 = w;
		double lhs2 = v.x;
		double lhs3 = v.y;
		double lhs4 = v.z;
		double rhs1 = B.w;
		double rhs2 = B.v.x;
		double rhs3 = B.v.y;
		double rhs4 = B.v.z;
		return{
		lhs1*rhs1-lhs2*rhs2-lhs3*rhs3-lhs4*rhs4,
	   {lhs1*rhs2+lhs2*rhs1+lhs3*rhs4-lhs4*rhs3,
		lhs1*rhs3-lhs2*rhs4+lhs3*rhs1+lhs4*rhs2,
		lhs1*rhs4+lhs2*rhs3-lhs3*rhs2+lhs4*rhs1}
		};
	}
	double abs(){return sqrt(w*w+v.x*v.x+v.y*v.y+v.z*v.z);}
	quat conj(){return {w,-v};}
	quat norm(){return (*this)*(1/abs());}
	/*void operator =(quat B){
		w = B.w;
		v = B.v;
	}*/
	static quat fromAngle(double p, double y, double r)
	{
		p = p*deg2rad*0.5;
		y = y*deg2rad*0.5;
		r = r*deg2rad*0.5;
		quat qr = {cos(r), {sin(r),0,0}};
		quat qp = {cos(p), {0,sin(p),0}};
		quat qy = {cos(y), {0,0,sin(y)}};
		return qy*(qp*qr);
	}
	static quat fromAngleAxis(double w, vec v){return quat::fromAngleAxis(w,v.x,v.y,v.z);}
	static quat fromAngleAxis(double w, double x, double y, double z)
	{
		return {cos(w*deg2rad/2),((vec){x,y,z}).norm()*sin(w*deg2rad/2)};
	}
	quat inv()
	{
		
		double div = w*w+v.x*v.x+v.y*v.y+v.z*v.z;
		return {w/div, -v/div};
		
	}
	quat qmod()
	{
		if(w<0){return {-w,-v};}else{return {w,v};}
	}
	vec getRotationAxis()
	{
		double m = v.x*v.x+v.y*v.y+v.z*v.z;
		if(m==0){return {0,0,1};}
		m = sqrt(m);
		return v/m;
	}
	double getRotationAngle()
	{
		double l = w*w+v.x*v.x+v.y*v.y+v.z*v.z;
		if(l==0){return 0;}
		l = sqrt(l);
		double ang = 2*acos(w/l)*rad2deg;
		if(ang>180){ang = ang-360;}
		return ang;
	}
	quat addRotation(double ang, vec vect)
	{
		vec vec2 = vect.norm();
		double ang2 = ang*deg2rad*0.5;
		quat C = {cos(ang2), vect*sin(ang2)};
		quat D = C*(*this);
		//D.v = D.v;
		return D.norm();
	}
	vec corotateVector(vec vect) //CW
	{	
		vec res = (((*this).inv()*((quat){0,vect}))*(*this)).v; //C++ evaluates left-hand first actually
		return res;
	}
	vec rotateVector(vec vect) //CCW
	{
		vec res = (((*this)*((quat){0,vect}))*((*this).inv())).v; //C++ evaluates left-hand first actually
		return res;
	}
	static quat from2vecs(vec X1, vec Y1) // one vec can be rotated to another, but it's "roll" is not preserved.
	{				 			   // use 4-vector version to define the third degree of freedom.
		return quat::fromAngleAxis(X1.angle(Y1),X1.cross(Y1).norm());
	}
	static quat from4vecs(vec X1, vec Y1, vec X2, vec Y2)
	{
		quat q1 = quat::fromAngleAxis(X1.angle(Y1),X1.cross(Y1).norm());
		vec X4 = q1.rotateVector(X2);
		
		quat q2 = quat::fromAngleAxis(((X4-X4.vectorProjection(Y1)).sgnAngle(Y2-Y2.vectorProjection(Y1),Y1)) ,X1);
		//quat q2 = quat::fromAngleAxis(((X2-X2.vectorProjection(Y1)).sgnAngle(Y2-Y2.vectorProjection(Y1),Y1)) ,Y1);
		//printf("angle = %f\n", (X4-X4.vectorProjection(Y1)).sgnAngle(Y2-Y2.vectorProjection(Y1),Y1));
		//quat q3 = q1; q3.v = q2.corotateVector(q1.v);
		//quat q3 = q2; q3.v = q1.corotateVector(q2.v);
		//return q2*q3;
		return q1*q2;
								//(X2-X2.vectorProjection(X1)) //projection of X2 onto plane of X1 (X2 is now perp to X1
								// same for Y1, and then find the angle on the plane.
	}
	
	quat operator +(quat B)
	{
		//quat q = quat::from4vecs((*this).rotateVector(B.rotateVector({1,0,0}),{1,0,0},B.rotateVector({0,1,0}),{0,1,0});
		// q returns from B to original axis. same as B.corotate?
		//fuck this makes no sense at all
		//there is no meaning to commutative addition of quaternions
		//unless simultaneous
		vec sv1 = v*w;
		vec sv2 = B.v*B.w;
		vec sv3 = sv1+sv2;
		return {sv3.length(), sv3.norm()};
	}
	quat operator -(quat B){return (*this)+(-B);}
	double getX()
	{
		return v.x/sqrt(1-w*w);
	}
	double getY()
	{
		return v.y/sqrt(1-w*w);
	}
	double getZ()
	{
		return v.z/sqrt(1-w*w);
	}
	vec getAxis()
	{
		return (vec){getX(),getY(),getZ()};
	}
	double getAngle()
	{
		return 360*acos(w)/M_PI;
	}
	vec right()
	{
		double this1 = w;
		double this2 = v.x;
		double this3 = v.y;
		double this4 = v.z;
		double t2 = this2*2;
		double t3 = this3*2;
		double t4 = this4*2;
		return {
			this1*this1+this2*this2-this3*this3-this4*this4,
			t3*this2+t4*this1,
			t4*this2-t3*this1
		};
	}
	vec forward()
	{
		double this1 = w;
		double this2 = v.x;
		double this3 = v.y;
		double this4 = v.z;
		double t2 = this2*2;
		double t3 = this3*2;
		double t4 = this4*2;
		return -((vec){
			t4*this1-t2*this3,
			this2*this2-this1*this1+this4*this4-this3*this3,
			-t2*this1-t3*this4
		});
	}
	vec up()
	{
		double this1 = w;
		double this2 = v.x;
		double this3 = v.y;
		double this4 = v.z;
		double t2 = this2*2;
		double t3 = this3*2;
		double t4 = this4*2;
		return {
			t3*this1+t2*this4,
			t3*this4-t2*this1,
			this1*this1-this2*this2-this3*this3+this4*this4
		};
	}
	string toString(){
		string str = "";
		return str+"quat:["+w+"|"+v.x+","+v.y+","+v.z+"]";
	}
};

//BLUH