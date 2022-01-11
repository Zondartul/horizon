static int physMaxId = 1; //not really important for anything
#define PHYSBODY_MAX_CHILDREN 11
class physBody
{
	public:
	model *mdl;
	vec pos;
	vec vel;
	vec acc;
	quat orient;
	double mass;
	double scale;
	vec3i color;
	unsigned char alpha;
	vec BScenter;//bounding sphere
	double BSradius;
	vec AABBmin;
	vec AABBmax;
	vec restAABBsize;
	vec restAABBoffset;
	char tracegroup; // 0 = no traces, 1 = phys traces
	ptrSniper<physBody> ptrs;
	int id;
	string name;
	int collisionCount;
	void (*onThink)(void*);
	physBody *parent;
	physBody *children[PHYSBODY_MAX_CHILDREN];
	void zero()
	{
		mdl = NULL;
		pos = vel = acc = {0,0,0};
		orient = quat::fromAngleAxis(0,0,0,1);
		color = {255,255,255};
		alpha = 255;
		BScenter = {0,0,0};
		scale = 1;
		mass = 1;
		BSradius = 0.0f;
		tracegroup = 1;
		ptrs.upperThis = this;
		onThink = NULL;
		name = "unnamed";
		parent = NULL;
		collisionCount = 0;
		AABBmin = AABBmax = restAABBsize = restAABBoffset = {0,0,0};
		for(int i = 0;i<PHYSBODY_MAX_CHILDREN;i++){children[i]=0;}
	}
	physBody()
	{
		zero();
		id = physMaxId++;
	}
	physBody(model *M)
	{
		zero();
		mdl = M;
		generateBoundingSphere();
		id = physMaxId++;
	}
	void setPos(vec newpos)
	{
		for(int i = 0;i<PHYSBODY_MAX_CHILDREN;i++)
		{
			if(!children[i]){continue;}
			children[i]->setPos(children[i]->pos+newpos-pos);
		}
		pos = newpos;
		updateAABB();
	}
	void setOrient(quat neworient)
	{
		quat diff = orient.inv()*neworient;
		for(int i = 0;i<PHYSBODY_MAX_CHILDREN;i++)
		{
			if(!children[i]){continue;}
			children[i]->setPos(diff.rotateVector(children[i]->pos));
			children[i]->orient = children[i]->orient*diff;
		}
		orient = neworient;
		updateAABB();
	}
	void setScale(double newscale)
	{
		for(int i = 0;i<PHYSBODY_MAX_CHILDREN;i++)
		{
			if(!children[i]){continue;}
			children[i]->setPos(children[i]->pos+(newscale/scale)*(children[i]->pos-pos));
			children[i]->setScale(children[i]->scale*(newscale/scale));
		}
		scale = newscale;
		updateAABB();
	}
	void setParent(physBody* prnt)
	{
		printf("\na");
		if(parent)
		{
			for(int i = 0;i<PHYSBODY_MAX_CHILDREN;i++)
			{printf("b");if(parent->children[i]==this){parent->children[i]=0;}}
		}
		printf("c");
		if(prnt)
		{
			for(int i = 0;i<PHYSBODY_MAX_CHILDREN;i++)
			{printf("d");if(prnt->children[i]==this){prnt->children[i]=0;}} //delete all copies
			printf("e");
			for(int i = 0;i<PHYSBODY_MAX_CHILDREN;i++)
			{printf("d");if(prnt->children[i]==0){prnt->children[i]=this; break;}} //insert
		}
		parent = prnt;
		printf("f");
	}
	void generateBoundingSphere()
	{
		if(mdl==NULL){printf("Physbody.gBSS: no model\n"); return;}
		BScenter = {0,0,0};
		int n = mdl->numtris;
		for(int i = 0; i<n;i++) //center is geometric mean of all points
		{
			BScenter = BScenter + mdl->mesh[i].v[0]/(3*n);
			BScenter = BScenter + mdl->mesh[i].v[1]/(3*n);
			BScenter = BScenter + mdl->mesh[i].v[2]/(3*n);
		}
		BSradius = 0;
		vec restAABBmin = {0,0,0};
		vec restAABBmax = {0,0,0};
		for(int i = 0; i<n;i++)
		{
			BSradius = max(BSradius, (mdl->mesh[i].v[0]-BScenter).length());
			BSradius = max(BSradius, (mdl->mesh[i].v[1]-BScenter).length());
			BSradius = max(BSradius, (mdl->mesh[i].v[2]-BScenter).length());
			//...
			for(int t = 0;t<3;t++)
			{
				restAABBmin.x = min(restAABBmin.x,mdl->mesh[i].v[t].x);
				restAABBmin.y = min(restAABBmin.y,mdl->mesh[i].v[t].y);
				restAABBmin.z = min(restAABBmin.z,mdl->mesh[i].v[t].z);
				restAABBmax.x = max(restAABBmax.x,mdl->mesh[i].v[t].x);
				restAABBmax.y = max(restAABBmax.y,mdl->mesh[i].v[t].y);
				restAABBmax.z = max(restAABBmax.z,mdl->mesh[i].v[t].z); //AABB calculation
			}
		}
		restAABBsize = (restAABBmax-restAABBmin);
		restAABBoffset = (restAABBmax+restAABBmin)/2;
		//AABBmin = restAABBoffset-restAABBsize/2;
		//AABBmax = restAABBoffset+restAABBsize/2;
		updateAABB();
		printf("BScenter: {%f,%f,%f}, BSradius: %f\n", BScenter.x,BScenter.y,BScenter.z,BSradius);
	}
	void updateAABB()
	{
		vec box[8] = {
		{0.5,-0.5,0.5},{-0.5,-0.5,0.5},
		{0.5,0.5,0.5},{-0.5,0.5,0.5},
		{0.5,-0.5,-0.5},{-0.5,-0.5,-0.5},
		{0.5,0.5,-0.5},{-0.5,0.5,-0.5},};
		AABBmin = pos;
		AABBmax = pos;
		for(int i = 0;i<8;i++)
		{
			vec A = pos+scale*orient.rotateVector(box[i]*restAABBsize+restAABBoffset);
			AABBmin.x = min(AABBmin.x,A.x);
			AABBmin.y = min(AABBmin.y,A.y);
			AABBmin.z = min(AABBmin.z,A.z);
			AABBmax.x = max(AABBmax.x,A.x);
			AABBmax.y = max(AABBmax.y,A.y);
			AABBmax.z = max(AABBmax.z,A.z); //AABB calculation	
		}
	}
	void applyForce(vec force)
	{
		vel = vel+force/mass;
	}
	
	~physBody(); // will declare outside
	
};
physBody* EntLookAt;
vector<physBody*> AllPhysBodies; // you herped the derp, now fix it

physBody::~physBody()
{
	for(int i = 0;i<AllPhysBodies.size();i++)
	{
		if(AllPhysBodies[i]==this){AllPhysBodies.erase(AllPhysBodies.begin()+i);printf("~physBody: erased at %d\n",i);}
	}
}
/*
typedef map<int, physBody*> bodymap; 
bodymap AllPhysBodies;

int bodymap::put(physBody *val)
{
	int key = 1;
	while((*this)[key]!=NULL){key++;}
	(*this)[key]=val;
	return key;
}
*/

class trace
{
	public:
	vec start;
	vec dir;
	bool hit;
	double dist;
	vec hitpos;
	physBody *hitObj;
	char tracegroup;
	trace()
	{
		start = {0,0,0};
		dir = {0,1,0};
		hit = false;
		hitpos = {0,0,0};
		hitObj = 0;
		dist = -1;
		tracegroup = 1;
	}
	void scan()
	{
		//int triNum = -1;//temp - debug triangle index
		vec initStart = start;
		vec initDir = dir;
		for(int i = 0; i<AllPhysBodies.size(); i++)
		{
			//ray-sphere intersection
			physBody *Bod = AllPhysBodies[i];
			if(!Bod){continue;}
			if(Bod->tracegroup==0){continue;}
			//move the ray, not every single vertex
			start = (1/Bod->scale)*Bod->orient.corotateVector(initStart-Bod->pos);
			dir = Bod->orient.corotateVector(initDir);
			vec O = Bod->BScenter;//+Bod->pos;
			double r = Bod->BSradius;
			vec L = O-start;
			double tca = L.dot(dir);
			if(tca<0){start = start+Bod->pos;continue;}//no hit
			double d2 = L.dot(L) - pow(tca,2);
			if(d2 > pow(r,2)){start = start+Bod->pos;continue;}//no hit
			//passed the sphere test, but did we hit the object itself?
			
			triangle* myMesh = Bod->mdl->mesh; // premature optimisation huh?
			int numtris = Bod->mdl->numtris;   // if so, I have sinned
			for(int t = 0;t<numtris;t++)
			{
				//ray-triangle intersection
				//we need at least one
				//or closest one
				vec A = myMesh[t].v[0];//+Bod->pos;
				vec B = myMesh[t].v[1];//+Bod->pos;
				vec C = myMesh[t].v[2];//+Bod->pos;
				//construct plane
				vec N = (B-A).cross(C-A);
				double d = -(N.x*A.x+N.y*A.y+N.z*A.z);
				
				double dist1 = -(start.dot(N)+d)/(dir.dot(N));
				vec hit1 = start+dir*dist1;
				//inside-outside triangle test
				if(  !((((B-A).cross(hit1-A)).dot(N)>=0)
					&&(((C-B).cross(hit1-B)).dot(N)>=0)
					&&(((A-C).cross(hit1-C)).dot(N)>=0)))
					{continue;}
				
				if((dist<0)|(dist1*Bod->scale<dist))
				{
					dist = dist1*Bod->scale;
					hitpos = initStart+initDir*dist;//hit1;
					hitObj = AllPhysBodies[i];
					hit = true;
					//triNum = t;
				}
			}
			/*
			double thc = sqrt(pow(r,2)-d2);
			double dist1 = tca-thc;
				//dist2 = tca+thc;
			vec hit1 = start+dir*dist;
				//vec hit2 = start+dir*dist2;
				//printf("hit");
			if((dist<0)|(dist1<dist))
			{
				dist = dist1;
				hitpos = hit1;
				hitObj = AllPhysBodies[i];
			}
			*/
			//start = start+Bod->pos; // re-setting ray position for next object
		}
		//printf("|%d",triNum);
	}
};

bool GJKcollision(physBody *A, physBody *B);

void PhysicsTick()
{
	double timestep = 1.0/60;
	
	//static int prevObj = -1;
	trace ray;
	ray.start = SomeVec1;
	if(mouseCapture){ray.dir = CamAngle.rotateVector({0,1,0});}
	else
	{
		vec v1,v2,v3,worldVec;
		v1 = v2 = worldVec = {0,0,0};
		
		GLdouble modelMatrix[16];
		GLdouble projMatrix[16];
		int viewport[4];
		
		glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
		glGetIntegerv(GL_VIEWPORT, viewport);
		
		gluUnProject(mousePos.x,mousePos.y,0,modelMatrix,projMatrix,viewport,&v1.x,&v1.y,&v1.z);
		gluUnProject(mousePos.x,mousePos.y,1,modelMatrix,projMatrix,viewport,&v2.x,&v2.y,&v2.z);
		//if(gluUnProject(mousePos.x,mousePos.y,1,modelMatrix,projMatrix,viewport,&v2.x,&v2.y,&v2.z)==GL_TRUE)
		//{printf("true, {%f,%f,%f}\n",v2.x,v2.y,v2.z);}else{printf("false, {%f,%f,%f}\n",v2.x,v2.y,v2.z);}
		
		v3 = v2-v1;
		worldVec.x = v3.x;
		worldVec.y =-v3.z;
		worldVec.z =-v3.y;
		ray.dir = CamAngle.rotateVector(worldVec.norm());
	}
	ray.scan();
	//if(prevObj!=-1){AllPhysBodies[prevObj].color = {255,255,255};}
	if(ray.hit)
	{
		//printf("hit: %f | %d\n | {%f,%f,%f}", ray.dist, ray.hitObj, ray.hitpos.x,ray.hitpos.y,ray.hitpos.z);
		EntLookAt = ray.hitObj;
		//AllPhysBodies[ray.hitObj].color = {255,0,0};
		//prevObj = ray.hitObj;
	}
	else
	{
		EntLookAt = 0;
	}
	//printf("-frame-\n");
	
	//init physics tick
	for(int i = 0;i<AllPhysBodies.size();i++)
	{
		physBody *A = AllPhysBodies[i];
		A->collisionCount = 0;
		A->acc = {0,0,0};
	}
	
	//collision detection,
	for(int i = 0;i<AllPhysBodies.size();i++)
	{
		//first, O(n^2) pairwise AABB checks
		physBody *A = AllPhysBodies[i];
		if(A->tracegroup!=1){continue;}
		for(int j = 0;j<AllPhysBodies.size();j++)
		{
			if(i>=j){continue;}//half the checks are unnecessary
			physBody *B = AllPhysBodies[j];
			if(B->tracegroup!=1){continue;}
			// [A--B] [D--C], (D>A, D<B)|(C>A,C<B)
			//printf("check %d vs %d\n",i+1,j+1);
			double xA = A->AABBmin.x; double xB = A->AABBmax.x; double xD = B->AABBmin.x; double xC = B->AABBmax.x;
			double yA = A->AABBmin.y; double yB = A->AABBmax.y; double yD = B->AABBmin.y; double yC = B->AABBmax.y;
			double zA = A->AABBmin.z; double zB = A->AABBmax.z; double zD = B->AABBmin.z; double zC = B->AABBmax.z;
			if(( ( (xC>=xA)&&(xC<=xB) )||( (xD>=xA)&&(xD<=xB) ))
			 &&( ( (yC>=yA)&&(yC<=yB) )||( (yD>=yA)&&(yD<=yB) ))
			 &&( ( (zC>=zA)&&(zC<=zB) )||( (zD>=zA)&&(zD<=zB) )))
			{
				//yay, AABB intersection!
				
				A->collisionCount++;
				B->collisionCount++;
				if(GJKcollision(A,B))
				{
					A->collisionCount+=10;
					B->collisionCount+=10;
					vec push;
					if((A->pos-B->pos).length()==0){push = vec(random(-1,1),random(-1,1),random(-1,1));}
					else{push = A->pos-B->pos;}
					push = push.norm();
					double restitution = 0.95;
					double pushmag = (1+restitution)*(B->vel-A->vel).dot(push)/(1/A->mass+1/B->mass);
					A->applyForce(push*pushmag);
					B->applyForce(-push*pushmag);
				}
			}
			/*
			if(i==0&&j==1){printf("{C:%d|%d:%d,%d,%d,%d,%d,%d}{%d,%d,%d}-{%d,%d,%d}\n",i,j,
				( (xC>=xA)&&(xC<=xB) ),( (xD>=xA)&&(xD<=xB) ),
				( (yC>=yA)&&(yC<=yB) ),( (yD>=yA)&&(yD<=yB) ),
				( (zC>=zA)&&(zC<=zB) ),( (zD>=zA)&&(zD<=zB) ),
				(int)A->AABBmin.x,(int)A->AABBmin.y,(int)A->AABBmin.z,
				(int)B->AABBmin.x,(int)B->AABBmin.y,(int)B->AABBmin.z)
				;}*/
		}
	}
	// velocity loop
	for(int i = 0;i<AllPhysBodies.size();i++)
	{
		//first, O(n^2) pairwise AABB checks
		physBody *A = AllPhysBodies[i];
		A->vel = A->vel+A->acc;
		A->setPos(A->pos+A->vel*timestep);
		A->vel = A->vel*(1-timestep/3);
	}
	//printf("\n");
}

//GJK collision detection

vec GJKsupport(physBody *A, physBody *B, vec d)
{
	//get furthest point in direction for mesh A
	vec p1 = A->mdl->mesh[0].v[1];
	for(int i = 0; i<A->mdl->numtris;i++)
	{
		for(int t = 0; t<3;t++)
		{
			vec p = A->mdl->mesh[i].v[t];
			if(p.dot(d)>p1.dot(d)){p1 = p;}
		}
	}
	//same for mesh B
	vec p2 = B->mdl->mesh[0].v[1];
	for(int i = 0; i<B->mdl->numtris;i++)
	{
		for(int t = 0; t<3;t++)
		{
			vec p = B->mdl->mesh[i].v[t];
			if(p.dot(-d)>p2.dot(-d)){p2 = p;}
		}
	}
	return (p1+A->pos)-(p2+B->pos);
}

bool GJKcollision(physBody *A, physBody *B)
{
	// tetrahedron simplex;
	vec d1 = { 1,  1,  1};  //   d3--d1
	vec d2 = {-1, -1,  1};  //    |./|
	vec d3 = {-1,  1, -1};  //    |/.|   
	vec d4 = { 1, -1, -1};  //   d2--d4

	vec p1 = GJKsupport(A,B,d1);if(p1.dot(d1)<=0){return false;} //if even one point does not "pass the origin",
	vec p2 = GJKsupport(A,B,d2);if(p2.dot(d2)<=0){return false;} //then the minkowski sum cannot contain the origin,
	vec p3 = GJKsupport(A,B,d3);if(p3.dot(d3)<=0){return false;} //meaning shapes do not intersect.
	vec p4 = GJKsupport(A,B,d4);if(p4.dot(d4)<=0){return false;}
	
	int rec = 0;
	bool insideOut = false;
	while(true) //always terminates because geometry
	{
	//printf("~%d.",rec++);
	vec dir1 = (p2-p1).cross(p3-p1); // anti p4
	vec dir2 = (p1-p2).cross(p4-p2); // anti p3
	vec dir3 = (p1-p4).cross(p3-p4); // anti p2
	vec dir4 = (p2-p3).cross(p4-p3); // anti p1
	if(insideOut){dir1=-dir1;dir2=-dir2;dir3=-dir3;dir4=-dir4;}
	
	if(p1.dot(dir1)<0) // is origin outside first plane?
	{
		//printf("1");
		insideOut = !insideOut;
		p4 = GJKsupport(A,B,dir1);
		if(p4.dot(dir1)<0){return false;}
		continue;
	}
	else if(p1.dot(dir2)<0) //is origin outside second plane?
	{
		//printf("2");
		insideOut = !insideOut;
		p3 = GJKsupport(A,B,dir2);
		if(p3.dot(dir2)<0){return false;}
		continue;
	}
	else if(p1.dot(dir3)<0) 
	{
		//printf("3");
		insideOut = !insideOut;
		p2 = GJKsupport(A,B,dir3);
		if(p2.dot(dir3)<0){return false;}
		continue;
	}
	else if(p2.dot(dir4)<0)
	{
		//printf("4");
		insideOut = !insideOut;
		p1 = GJKsupport(A,B,dir4);
		if(p1.dot(dir4)<0){return false;}
		continue;
	}
	else //origin inside all planes.
	{
		//printf("5");
		return true;
	}
	}
}