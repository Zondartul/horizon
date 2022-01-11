class renderable{
	public:
	int lifetime;
	renderable(){
		lifetime = -1;
	}
	virtual void render(){}
};

class point: public renderable{
	public:
	vec pos;
	vec3i color;
	float thickness;
	point():renderable(){
		pos = {0,0,0};
		color = {0,0,0};
		thickness = 3;
	}
	point(vec newpos):renderable(){
		pos = newpos;
		color = {0,0,0};
		thickness = 3;
	}
	point(vec newpos, int time):renderable(){
		pos = newpos;
		color = {0,0,0};
		thickness = 3;
		lifetime = time;
	}
	void render(){
		glPointSize(thickness);
		glBegin(GL_POINTS);
			glColor3ub(color.x, color.y, color.z);
			glVertex3d(pos.x, pos.y, pos.z);
		glEnd();
		glPointSize(1);
	}
};

class line: public renderable{
	public:
	vec end1;
	vec end2;
	float thickness;
	bool infinite;
	vec3i color1;
	vec3i color2;
	line():renderable(){
		end1 = {0,0,0};
		end2 = {0,1,0};
		color1 = {0,0,0};
		color2 = {0,0,0};
		thickness = 2;
		infinite = false;
	}
	line(vec newend1, vec newend2):renderable(){
		end1 = newend1;
		end2 = newend2;
		color1 = {0,0,0};
		color2 = {0,0,0};
		thickness = 2;
		infinite = false;
	}
	line(vec newend1, vec newend2, int time):renderable(){
		end1 = newend1;
		end2 = newend2;
		color1 = {0,0,0};
		color2 = {0,0,0};
		thickness = 2;
		lifetime = time;
		infinite = false;
	}
	bool find_frustum_intersections(vec &endA, vec &endB){
		frustum f = camera.getFrustum();
		vec dir1 = end2 - end1;
		vec dir2 = -dir1;
		for(int I = 0; I<12; I++){
			if(ray_triangle_intersection(end1, dir1, f.getTri(I,0),f.getTri(I,1),f.getTri(I,2),endA)){
				for(int J = I+1; J<12; J++){
					if(ray_triangle_intersection(end1, dir1, f.getTri(J,0),f.getTri(J,1),f.getTri(J,2),endB)){
						//printf("< +I+J: %d, %d >\n",I,J);
						return true;}
				}
				for(int J = I; J<12; J++){
					if(ray_triangle_intersection(end1, dir2, f.getTri(J,0),f.getTri(J,1),f.getTri(J,2),endB)){
						//printf("< +I-J: %d, %d >\n",I,J);
						return true;}
				}
			}
		}
		for(int I = 0; I<12; I++){
			if(ray_triangle_intersection(end1, dir2, f.getTri(I,0),f.getTri(I,1),f.getTri(I,2),endA)){
				for(int J = I; J<12; J++){
					if(ray_triangle_intersection(end1, dir1, f.getTri(J,0),f.getTri(J,1),f.getTri(J,2),endB)){
						//printf("< -I+J: %d, %d >\n",I,J);
						return true;}
				}
				for(int J = I; J<12; J++){
					if(ray_triangle_intersection(end1, dir2, f.getTri(J,0),f.getTri(J,1),f.getTri(J,2),endB)){
						//printf("< -I-J: %d, %d >\n",I,J);
						return true;}
				}
			}
		}
		/*
		if(ray_triangle_intersection(end1, dir1, f.C, f.A, f.B, endA)){if(endB == vec()){endB = endA;}else return true;}
		if(ray_triangle_intersection(end1, dir1, f.D, f.A, f.C, endA)){if(endB == vec()){endB = endA;}else return true;}
		if(ray_triangle_intersection(end1, dir1, f.D, f.E, f.A, endA)){if(endB == vec()){endB = endA;}else return true;}
		if(ray_triangle_intersection(end1, dir1, f.H, f.E, f.D, endA)){if(endB == vec()){endB = endA;}else return true;}
		if(ray_triangle_intersection(end1, dir1, f.H, f.F, f.E, endA)){if(endB == vec()){endB = endA;}else return true;}
		if(ray_triangle_intersection(end1, dir1, f.G, f.F, f.H, endA)){if(endB == vec()){endB = endA;}else return true;}
		if(ray_triangle_intersection(end1, dir1, f.B, f.F, f.G, endA)){if(endB == vec()){endB = endA;}else return true;}
		if(ray_triangle_intersection(end1, dir1, f.G, f.C, f.B, endA)){if(endB == vec()){endB = endA;}else return true;}
		if(ray_triangle_intersection(end1, dir2, f.C, f.A, f.B, endA)){if(endB == vec()){endB = endA;}else return true;}
		if(ray_triangle_intersection(end1, dir2, f.D, f.A, f.C, endA)){if(endB == vec()){endB = endA;}else return true;}
		if(ray_triangle_intersection(end1, dir2, f.D, f.E, f.A, endA)){if(endB == vec()){endB = endA;}else return true;}
		if(ray_triangle_intersection(end1, dir2, f.H, f.E, f.D, endA)){if(endB == vec()){endB = endA;}else return true;}
		if(ray_triangle_intersection(end1, dir2, f.H, f.F, f.E, endA)){if(endB == vec()){endB = endA;}else return true;}
		if(ray_triangle_intersection(end1, dir2, f.G, f.F, f.H, endA)){if(endB == vec()){endB = endA;}else return true;}
		if(ray_triangle_intersection(end1, dir2, f.B, f.F, f.G, endA)){if(endB == vec()){endB = endA;}else return true;}
		if(ray_triangle_intersection(end1, dir2, f.G, f.C, f.B, endA)){if(endB == vec()){endB = endA;}else return true;}
		*/
		return false;
	}
	void render(){
		glLineWidth(thickness);
		if(!infinite){
			glBegin(GL_LINES);
				glColor3ub(color1.x, color1.y, color1.z);
				glVertex3d(end1.x, end1.y, end1.z);
				glColor3ub(color2.x, color2.y, color2.z);
				glVertex3d(end2.x, end2.y, end2.z);
			glEnd();
		}else{
			
			vec endA = vec();
			vec endB = vec();
			if(find_frustum_intersections(endA, endB)){
				glBegin(GL_LINES);
					glColor3ub(color1.x, color1.y, color1.z);
					glVertex3d(endA.x, endA.y, endA.z);
					glColor3ub(color2.x, color2.y, color2.z);
					glVertex3d(endB.x, endB.y, endB.z);
				glEnd();
			}			
		}
		glLineWidth(1);
	}
};

class rtriangle:public renderable{
	public:
	vec A;
	vec3i colA;
	vec B;
	vec3i colB;
	vec C;
	vec3i colC;
	int type;
	rtriangle():renderable(){
		A = vec(0,1,0);
		B = vec(0,0.87,-0.5);
		C = vec(0,-0.87,-0.5);
		colA = {255,0,0};
		colB = {0,255,0};
		colC = {0,0,255};
		type = 0;
	}
	rtriangle(vec vA, vec vB, vec vC):renderable(){
		A = vA;
		B = vB;
		C = vC;
		colA = {255,0,0};
		colB = {0,255,0};
		colC = {0,0,255};
		type = 0;
	}
	rtriangle(vec vA, vec vB, vec vC, int time):renderable(){
		A = vA;
		B = vB;
		C = vC;
		colA = {255,0,0};
		colB = {0,255,0};
		colC = {0,0,255};
		lifetime = time;
		type = 0;
	}
	rtriangle(triangle tri, vector<vec> points, int time):renderable(){
		A = points[tri[0]];
		B = points[tri[1]];
		C = points[tri[2]];
		colA = {255,0,0};
		colB = {0,255,0};
		colC = {0,0,255};
		lifetime = time;
		type = 0;
	}
	void render(){
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
		if(type == 0){
		glBegin(GL_TRIANGLES);
			glColor3ub(colA.x,colA.y,colA.z);
			glVertex3d(A.x,A.y,A.z);
			glColor3ub(colB.x,colB.y,colB.z);
			glVertex3d(B.x,B.y,B.z);
			glColor3ub(colC.x,colC.y,colC.z);
			glVertex3d(C.x,C.y,C.z);
		glEnd();
		}else{
		glBegin(GL_TRIANGLES);
			glColor3ub(colA.x,colA.y,colA.z);
			glVertex3d(A.x,A.y,A.z);
			glColor3ub(colA.x,colA.y,colA.z);
			glVertex3d(B.x,B.y,B.z);
			glColor3ub(colA.x,colA.y,colA.z);
			glVertex3d(C.x,C.y,C.z);
		glEnd();
		glBegin(GL_LINE_LOOP);
			glColor3ub(colB.x,colB.y,colB.z);
			glVertex3d(A.x,A.y,A.z);
			glVertex3d(B.x,B.y,B.z);
			glVertex3d(C.x,C.y,C.z);
		glEnd();
		}
		glDisable(GL_CULL_FACE);
	}
};

class floatingtext: public renderable{
	public:
	string text;
	vec2i screen_offset;
	vec pos;
	floatingtext():renderable(){
		text = "";
		screen_offset = {0,0};
		pos = {0,0,0};
	}
	floatingtext(vec newpos, string newtext):renderable(){
		text = newtext;
		screen_offset = {0,0};
		pos = newpos;
	}
	floatingtext(vec newpos, string newtext, int time):renderable(){
		text = newtext;
		screen_offset = {0,0};
		pos = newpos;
		lifetime = time;
	}
	void render(){
		vec c = camera.worldtoscreen(pos);
		c.x += screen_offset.x;
		c.y += screen_offset.y;
		//cout << "text: " << c.toString() << "\n";
		printvals(toString(this), c.toString());
		if(camera.screencoordsvisible(c)){
			camera.go2D();
			printw((int)(c.x),(int)(c.y),-1,-1,"%s",text.c_str());
			camera.go3D();
		}
	}
};