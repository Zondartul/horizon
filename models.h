//models.h

struct triangle
{
	vec v[3];
	vec getNormal()
	{
		return (v[2]-v[0]).cross(v[1]-v[0]).norm();
	}
};

enum renderflags
{
	R_NODRAW		=1,
	R_UNLIT			=2,
	R_TEXTURE		=4,
	R_LIGHTEN		=8,  //transparency: 0 = opaque, LIGHTEN+DARKEN = normal
	R_DARKEN		=16, 
	R_MIRROR		=32, //same texture index = same mirror plane.
	R_DEBUG			=64, //overdraws debug data
	R_MINDFUCK		=128//acid
};

struct textriangle
{
	vec2f v[3];
	int texid;
	renderflags flags;
	/*
	textriangle()
	{
		v[0]=v[1]=v[2] = {0,0};
	}*/
};
/*
struct texture
{
	int texturetype; //normal, animated, dynamic, etc.
	char* texname;
	void *glTexbind; //or something
};
*/

struct model
{
	triangle *mesh;			int numtris;
	triangle *normals;
	textriangle *texmap;	
	texture *textures;		int numtextures;
	vec min,max;
	char blendmode; //0 = opaque, 1 = alpha test only, 2 = object-ordered blending, 3 = shader blending
	model()
	{
		numtris = 0; numtextures = 0;
		mesh = NULL;
		normals = NULL;
		texmap = NULL;
		textures = NULL;
		min = max = {0,0,0};
		blendmode = 1;
	}
	void applyRenderFlags(int R)
	{
		// is texmap missing?
		for(int i = 0;i<numtris;i++)
		{
			texmap[i].flags = (renderflags)R;
		}
	}
	void expandTriangle(vec point, int tri) //no texture support yet
	{
		numtris += 2;
		mesh = (triangle*)realloc((void*)mesh, numtris*sizeof(triangle*));
		triangle t = mesh[tri];
		
		mesh[tri] 		= (triangle){t.v[0],t.v[1],point};
		mesh[numtris-2] = (triangle){point,t.v[1],t.v[2]};
		mesh[numtris-1] = (triangle){t.v[2],t.v[0],point};
	}
	vec getNormalFlat(int tri)
	{
		return (mesh[tri].v[1]-mesh[tri].v[0]).cross(mesh[tri].v[2]-mesh[tri].v[0]).norm();
	}
	vector<int> getAdjacentTris(int tri, int vert)
	{
		vector<int> Adj;
		vec v = mesh[tri].v[vert];
		for(int i = 0;i<numtris;i++)
		{
			if((mesh[i].v[0]==v)||
				(mesh[i].v[1]==v)||
				(mesh[i].v[2]==v))
			{
				Adj.push_back(i);
			}
		}
		return Adj;
	}
};
