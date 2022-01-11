#include "display/renderable.h"
#include "util/globals.h"
#include "math/vectors.h"
#include "game/camera.h"
#include "resource/models.h"
#include "gui/valscreen.h"
#include "resource/fonts.h"

//derp
renderable::renderable(){
	lifetime = -1;
	color = {0,0,0,255};
	thickness = 1;
	renderflags = RENDER_DEFAULT | COLOR_DEFAULT | LIGHT_DEFAULT | TRANSPARENCY_DEFAULT;
}
void renderable::render(){}
void renderable::render2(uint32_t flags){
	uint32_t oldflags = renderflags;
	renderflags = flags;
	render();
	renderflags = oldflags;
}
void renderable::setrenderflags(uint32_t flags, bool recursive){
	renderflags = flags;
}
point::point():renderable(){
	pos = {0,0,0};
	color = {0,0,0,255};
	thickness = 3;
}
point::point(vec newpos):renderable(){
	pos = newpos;
	color = {0,0,0,255};
	thickness = 3;
}
point::point(vec newpos, int time):renderable(){
	pos = newpos;
	color = {0,0,0,255};
	thickness = 3;
	lifetime = time;
}
void point::render(){
	if(renderflags & RENDER_NONE){return;}
	//printf("rendering %s\n",toString(this).c_str());
	glPointSize(thickness);
	glBegin(GL_POINTS);
		glColor4ub(color.r, color.g, color.b, color.a);
		glVertex3d(pos.x, pos.y, pos.z);
	glEnd();
	glPointSize(1);
}

line::line():renderable(){
	end1.pos = {0,0,0};
	end2.pos = {0,1,0};
	end1.thickness = 2;
	end2.thickness = 2;
	linetype = LINE_SEGMENT;
}
line::line(vec newend1, vec newend2):renderable(){
	end1.pos = newend1;
	end2.pos = newend2;
	end1.thickness = 2;
	end2.thickness = 2;
	linetype = LINE_SEGMENT;
}
line::line(vec newend1, vec newend2, int time):renderable(){
	end1 = newend1;
	end2 = newend2;
	end1.thickness = 2;
	end2.thickness = 2;
	lifetime = time;
	linetype = LINE_SEGMENT;
}

//check if this works in infinite/not infinite cases.
//shouldn't this go to the general geometry file?
bool line::find_frustum_intersections(vec &endA, vec &endB){
	frustum f = camera.getFrustum();
	vec dir1 = end2.pos - end1.pos;
	vec dir2 = -dir1;
	for(int I = 0; I<12; I++){
		if(ray_triangle_intersection(end1.pos, dir1, f.getTri(I,0),f.getTri(I,1),f.getTri(I,2),endA)){
			for(int J = I+1; J<12; J++){
				if(ray_triangle_intersection(end1.pos, dir1, f.getTri(J,0),f.getTri(J,1),f.getTri(J,2),endB)){
					//printf("< +I+J: %d, %d >\n",I,J);
					return true;}
			}
			for(int J = I; J<12; J++){
				if(ray_triangle_intersection(end1.pos, dir2, f.getTri(J,0),f.getTri(J,1),f.getTri(J,2),endB)){
					//printf("< +I-J: %d, %d >\n",I,J);
					return true;}
			}
		}
	}
	for(int I = 0; I<12; I++){
		if(ray_triangle_intersection(end1.pos, dir2, f.getTri(I,0),f.getTri(I,1),f.getTri(I,2),endA)){
			for(int J = I; J<12; J++){
				if(ray_triangle_intersection(end1.pos, dir1, f.getTri(J,0),f.getTri(J,1),f.getTri(J,2),endB)){
					//printf("< -I+J: %d, %d >\n",I,J);
					return true;}
			}
			for(int J = I; J<12; J++){
				if(ray_triangle_intersection(end1.pos, dir2, f.getTri(J,0),f.getTri(J,1),f.getTri(J,2),endB)){
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

void line::render(){
	if(renderflags & RENDER_NONE){return;}
	//set colors
	color4i col1 = color;
	color4i col2 = color;
	if(renderflags & (COLOR_DEFAULT | COLOR_BLEND)){
		col1 = end1.color;
		col2 = end2.color;
	}
	//render the line
	if(renderflags & (RENDER_DEFAULT | RENDER_EDGES)){
		vec endA = end1.pos;
		vec endB = end2.pos;
		//vec frustA;
		//vec frustB;
		if(linetype != LINE_SEGMENT){
			//need frustrum magics here!
			printf("infinite rlines not implemented!\n");
		}
		glLineWidth(thickness);
			glBegin(GL_LINES);
				glColor4ub(col1.r, col1.g, col1.b, col1.a);
				glVertex3d(endA.x, endA.y, endA.z);
				glColor4ub(col2.r, col2.g, col2.b, col2.a);
				glVertex3d(endB.x, endB.y, endB.z);
			glEnd();
		glLineWidth(1);
	}
	//render the endpoints
	if(renderflags & RENDER_VERTICES){		
		if(renderflags & RENDER_OVERRIDE){
			end1.render2(renderflags);
			end2.render2(renderflags);
		}else{
			end1.render();
			end2.render();
		}
	}
}
void line::setrenderflags(uint32_t flags, bool recursive){
	renderflags = flags;
	if(recursive){
		end1.setrenderflags(flags,recursive);
		end2.setrenderflags(flags,recursive);
	}
}

rtriangle::rtriangle():renderable(){
	A.pos = vec(0,1,0);
	B.pos = vec(0,0.87,-0.5);
	C.pos = vec(0,-0.87,-0.5);
	A.color = {255,0,0,255};
	B.color = {0,255,0,255};
	C.color = {0,0,255,255};
}
rtriangle::rtriangle(vec vA, vec vB, vec vC):renderable(){
	A.pos = vA;
	B.pos = vB;
	C.pos = vC;
	A.color = {255,0,0,255};
	B.color = {0,255,0,255};
	C.color = {0,0,255,255};
}
rtriangle::rtriangle(vec vA, vec vB, vec vC, int time):renderable(){
	A.pos = vA;
	B.pos = vB;
	C.pos = vC;
	A.color = {255,0,0,255};
	B.color = {0,255,0,255};
	C.color = {0,0,255,255};
	lifetime = time;
}
rtriangle::rtriangle(idtriangle tri, vector<vec> *points, int time):renderable(){
	A.pos = (*points)[tri[0]];
	B.pos = (*points)[tri[1]];
	C.pos = (*points)[tri[2]];
	A.color = {255,0,0,255};
	B.color = {0,255,0,255};
	C.color = {0,0,255,255};
	lifetime = time;
}
void rtriangle::render(){
	//printf("rtriangle %p\n",this);
	if(renderflags & RENDER_NONE){return;}
	//prepare transparency
	int Aolda = A.color.a;
	int Bolda = B.color.a;
	int Colda = C.color.a;
	if(!(renderflags & (TRANSPARENCY_DEFAULT | TRANSPARENCY_NONE))){
		if(renderflags & TRANSPARENCY_UNIFORM){
			glEnable(GL_BLEND);
			glDepthMask(GL_FALSE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			A.color.a = A.color.a * color.a / 255;
			B.color.a = B.color.a * color.a / 255;
			C.color.a = C.color.a * color.a / 255;
		}
	}
	//prepare colors
	color4i colA = color;
	color4i colB = color;
	color4i colC = color;
	if(renderflags & (COLOR_DEFAULT | COLOR_BLEND)){
		colA = {A.color.r,A.color.g,A.color.b,A.color.a};
		colB = {B.color.r,B.color.g,B.color.b,B.color.a};
		colC = {C.color.r,C.color.g,C.color.b,C.color.a};
	}
	//render the triangle face
	if(renderflags & (RENDER_DEFAULT | RENDER_FACES | RENDER_BACKFACES)){
		//do we need to cull at all?
		if(!((renderflags & RENDER_FACES) && (renderflags & RENDER_BACKFACES))){
			glEnable(GL_CULL_FACE);
			//do we cull clockwise?
			if(renderflags & (RENDER_DEFAULT | RENDER_FACES)){
				glFrontFace(GL_CW);
			//or anti-clockwise?
			}else if(renderflags & (RENDER_BACKFACES)){
				glFrontFace(GL_CCW);
			}
		}
		glBegin(GL_TRIANGLES);
			glColor4ub(colA.r,colA.g,colA.b,colA.a);
			glVertex3d(A.pos.x,A.pos.y,A.pos.z);
			glColor4ub(colB.r,colB.g,colB.b,colB.a);
			glVertex3d(B.pos.x,B.pos.y,B.pos.z);
			glColor4ub(colC.r,colC.g,colC.b,colC.a);
			glVertex3d(C.pos.x,C.pos.y,C.pos.z);
		glEnd();
	}
	//render the triangle edges
	if(renderflags & RENDER_EDGES){
		glBegin(GL_LINE_LOOP);
			glColor4ub(colA.r,colA.g,colA.b,colA.a);
			glVertex3d(A.pos.x,A.pos.y,A.pos.z);
			glColor4ub(colB.r,colB.g,colB.b,colB.a);
			glVertex3d(B.pos.x,B.pos.y,B.pos.z);
			glColor4ub(colC.r,colC.g,colC.b,colC.a);
			glVertex3d(C.pos.x,C.pos.y,C.pos.z);
		glEnd();	
	}
	//render the triangle vertices
	if(renderflags & RENDER_VERTICES){	
		if(renderflags & RENDER_OVERRIDE){
			A.render2(renderflags);
			B.render2(renderflags);
			C.render2(renderflags);
		}else{
			A.render();
			B.render();
			C.render();
		}
	}
	//restore settings
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	A.color.a = Aolda;
	B.color.a = Bolda;
	C.color.a = Colda;
}
void rtriangle::setrenderflags(uint32_t flags, bool recursive){
	renderflags = flags;
	if(recursive){
		A.setrenderflags(flags, recursive);
		B.setrenderflags(flags, recursive);
		C.setrenderflags(flags, recursive);
	}
}

floatingtext::floatingtext():renderable(){
	text = "";
	screen_offset = {0,0};
	pos = {0,0,0};
}
floatingtext::floatingtext(vec newpos, string newtext):renderable(){
	text = newtext;
	screen_offset = {0,0};
	pos = newpos;
}
floatingtext::floatingtext(vec newpos, string newtext, int time):renderable(){
	text = newtext;
	screen_offset = {0,0};
	pos = newpos;
	lifetime = time;
}
void floatingtext::render(){
	if(renderflags & RENDER_NONE){return;}
	//printf("rendering %s\n",toString(this).c_str());
	vec c = camera.worldtoscreen(pos);
	c.x += screen_offset.x;
	c.y += screen_offset.y;
	//cout << "text: " << c.toString() << "\n";
	printvals(toString(this), c.toString());
	if(camera.screencoordsvisible(c)){
		glColor4ub(color.r,color.g,color.b,color.a);
		camera.go2D();
		printw((int)(c.x),(int)(c.y),-1,-1,"%s",text.c_str());
		camera.go3D();
	}
}
rmodel::rmodel(model *m){
	M = m;
	color = {255,255,255,255};
}

void rmodel::render(){
	if(renderflags & RENDER_NONE){return;}
	//printf("rmodel %p / %p / %d\n",this, M, M->vertices.size());
	//render triangles
	for(int I = 0; I < triangles.size(); I++){
		rtriangle &RT = triangles[I];
		int oldA = RT.color.a;
		if(!(renderflags & (TRANSPARENCY_DEFAULT | TRANSPARENCY_NONE))){
			if(renderflags & TRANSPARENCY_UNIFORM){
				RT.color.a = RT.color.a * color.a / 255;
			}
		}
		if(renderflags & RENDER_OVERRIDE){
			RT.render2(renderflags);
		}else{
			RT.render();
		}
		RT.color.a = oldA;
	}
	// for(int I = 0; I < M->mesh.size(); I++){
		// idtriangle T = M->mesh[I];
		// rtriangle RT(T, &M->vertices, -1);
		// int oldA = RT.color.a;
		// if(!(renderflags & (TRANSPARENCY_DEFAULT | TRANSPARENCY_NONE))){
			// if(renderflags & TRANSPARENCY_UNIFORM){
				// RT.color.a = RT.color.a * color.a / 255;
			// }
		// }
		// if(renderflags & RENDER_OVERRIDE){
			// RT.render2(renderflags);
		// }else{
			// RT.render();
		// }
		// RT.color.a = oldA;
	// }
}

void rmodel::regenerate(){
	triangles.clear();
	for(int I = 0; I < M->mesh.size(); I++){
		idtriangle T = M->mesh[I];
		rtriangle RT(T, &M->vertices, -1);
		triangles.push_back(RT);
	}
}

void rmodel::setrenderflags(uint32_t flags, bool recursive){
	renderflags = flags;
	if(recursive){
		for(int I = 0; I < triangles.size(); I++){
			triangles[I].setrenderflags(flags,recursive);
		}
	}
}