class physBody
{
	public:
	model *mdl;
	vec pos;
	quat orient;
	double scale;
	vec3i color;
	vec BScenter;//bounding sphere
	double BSradius;
	char tracegroup; // 0 = no traces, 1 = phys traces
	ptrSniper<physBody> ptrs;
	void (*onThink)(void*);
	void zero()
	{
		mdl = NULL;
		pos = {0,0,0};
		orient = quat::fromAngleAxis(0,0,0,1);
		color = {255,255,255};
		BScenter = {0,0,0};
		scale = 1;
		BSradius = 0.0f;
		tracegroup = 1;
		ptrs.upperThis = this;
		onThink = NULL;
	}
	physBody()
	{
		zero();
	}
	physBody(model *M)
	{
		zero();
		mdl = M;
		generateBoundingSphere();
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
		for(int i = 0; i<n;i++)
		{
			BSradius = max(BSradius, (mdl->mesh[i].v[0]-BScenter).length());
			BSradius = max(BSradius, (mdl->mesh[i].v[1]-BScenter).length());
			BSradius = max(BSradius, (mdl->mesh[i].v[2]-BScenter).length());
		}
		printf("BScenter: {%f,%f,%f}, BSradius: %f\n", BScenter.x,BScenter.y,BScenter.z,BSradius);
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
		for(int i = 0; i<AllPhysBodies.size(); i++)
		{
			
			//ray-sphere intersection
			physBody *Bod = AllPhysBodies[i];
			if(!Bod){continue;}
			if(Bod->tracegroup==0){continue;}
			//move the ray, not every single vertex
			start = start-Bod->pos;
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
				
				if((dist<0)|(dist1<dist))
				{
					dist = dist1;
					hitpos = hit1;
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
			start = start+Bod->pos; // re-setting ray position for next object
		}
		//printf("|%d",triNum);
	}
};

void PhysicsTick()
{
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
}