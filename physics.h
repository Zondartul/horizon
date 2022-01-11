class physBody
{
	public:
	model *mdl;
	vec pos;
	quat orient;
	vec3i color;
	vec BScenter;//bounding sphere
	double BSradius;
	void zero()
	{
		mdl = NULL;
		pos = {0,0,0};
		orient = quat::fromAngleAxis(0,0,0,1);
		color = {255,255,255};
		BScenter = {0,0,0};
		BSradius = 0.0f;
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
};

vector<physBody> AllPhysBodies;

class trace
{
	public:
	vec start;
	vec dir;
	bool hit;
	double dist;
	vec hitpos;
	int hitObj;
	trace()
	{
		start = {0,0,0};
		dir = {0,1,0};
		hit = false;
		hitpos = {0,0,0};
		hitObj = 0;
		dist = -1;
	}
	void scan()
	{
		for(int i = 0; i<AllPhysBodies.size(); i++)
		{
			//ray-sphere intersection
			physBody Bod = AllPhysBodies[i];
			vec O = Bod.BScenter;
			double r = Bod.BSradius;
			vec L = O-start;
			double tca = L.dot(dir);
			if(tca<0){continue;}//no hit
			double d2 = L.dot(L) - pow(tca,2);
			if(d2 > pow(r,2)){continue;}//no hit
			//passed the sphere test, but did we hit the object itself?
			for(int t = 0;t<Bod.mdl->numtris;t++)
			{
				//ray-triangle intersection
				//we need at least one
				//or closest one
				vec A = Bod.mdl->mesh[t].v[0];
				vec B = Bod.mdl->mesh[t].v[1];
				vec C = Bod.mdl->mesh[t].v[2];
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
					hitObj = i;
					hit = true;
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
				hitObj = i;
			}
			*/
		}
	}
};

void PhysicsTick()
{
	static int prevObj = -1;
	trace ray;
	ray.start = SomeVec1;
	ray.dir = CamAngle.rotateVector({0,1,0});
	ray.scan();
	if(prevObj!=-1){AllPhysBodies[prevObj].color = {255,255,255};}
	if(ray.hit)
	{
		//printf("hit: %f | %d\n | {%f,%f,%f}", ray.dist, ray.hitObj, ray.hitpos.x,ray.hitpos.y,ray.hitpos.z);
		AllPhysBodies[ray.hitObj].color = {255,0,0};
		prevObj = ray.hitObj;
	}
	//printf("-frame-\n");
}