// returns true: result is set to point of intersection of ray and triangle
// returns false: result is not modified. No intersection exists.
bool ray_triangle_intersection(vec start, vec dir, vec A, vec B, vec C, vec &result){
	//Moller-Trumbore intersection algorythm
	vec e1, e2;
	vec P, Q, T;
	double det, inv_det, u, v;
	float t;
	
	e1 = B-A;
	e2 = C-A;
	P = dir.cross(e2);
	det = e1.dot(P);
	//this auto-fails if the ray is nearly in the triangle's plane
	//if(det > -EPSILON && det < EPSILON){return 0;}
	inv_det = 1.0f / det;
	T = start-A;
	u = T.dot(P) * inv_det;
	//intersection is outside the triangle
	if(u < 0.0f || u > 1.0f){return 0;}
	Q = T.cross(e1);
	v = dir.dot(Q) * inv_det;
	//intersection is outside the triangle
	if(v < 0.0f || u + v > 1.0f){return 0;}
	t = e2.dot(Q) * inv_det;
	
	if(t > 0){//if(t > EPSILON)
		printf("distance = %f\n", t);
		result = start+dir*t;
		return 1;
	}
	return 0;
	// vec N = (B-A).cross(C-A);
	// double d = -(N.x*A.x+N.y*A.y+N.z*A.z);
	// double dist1 = -(start.dot(N)+d)/(dir.dot(N));
	// vec hit1 = start+dir*dist1;
	////inside-outside triangle test
	// if(  !((((B-A).cross(hit1-A)).dot(N)>=0)
		// &&(((C-B).cross(hit1-B)).dot(N)>=0)
		// &&(((A-C).cross(hit1-C)).dot(N)>=0)))
		// {return false;}
		// result = hit1;
		// printf("dist = %f\n",dist1);
		// return true;
}