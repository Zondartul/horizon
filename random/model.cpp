#include "model.h"

editmodel::editmodel(){
	material = 0;
	
	vec A = {0,0,0};
	vec B = {1,0,0};
	vec C = {0,-1,0};
	vec D = {1,-1,0};
	vec E = {0,0,-1};
	vec F = {1,0,-1};
	vec G = {0,-1,-1};
	vec H = {1,-1,-1};
	vec UV0 = {0,0,0};
	vec col0 = {1,1,1};
	positions.push_back(A);
	positions.push_back(B);
	positions.push_back(C);
	positions.push_back(D);
	positions.push_back(E);
	positions.push_back(F);
	positions.push_back(G);
	positions.push_back(H);
	UVs.push_back(UV0);
	colors.push_back(col0);
	triangles.push_back({3,1,2});
	triangles.push_back({3,2,4});
	triangles.push_back({4,2,6});
	triangles.push_back({4,6,8});
	triangles.push_back({8,6,5});
	triangles.push_back({8,5,6});
	triangles.push_back({7,5,1});
	triangles.push_back({6,1,3});
	triangles.push_back({1,5,8});
	// triangles.push_back({});
	// triangles.push_back({});
	// triangles.push_back({});
	// triangles.push_back({});
	// triangles.push_back({});
	// triangles.push_back({});
	// triangles.push_back({});
}

editmodel::~editmodel(){
}

void rendermodel::load(){
	numVerts = vertices.size();
	numEdges = edges.size();
	numTris = triangles.size();
	for(auto I = M->vertices.begin(); I != M->vertices.end(); I++){
		buffer_vertex(*I);
	}
	for(auto I = M->edges.begin(); I != M->edges.end(); I++){
		buffer_edge(*I);
	}
	for(auto I = M->triangles.begin(); I != M->triangles.end(); I++){
		buffer_vertex(*I);
	}
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, buffer.size()*sizeof(GLfloat), buffer.data(), GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//position
	glVertexAttribPointer(
	   0,                  // attribute ID
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   (void*)0            // array buffer offset
	);
	//color
	glVertexAttribPointer(
	   1,                  // attribute ID
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   (void*)0            // array buffer offset
	);
	//UV
	glVertexAttribPointer(
	   2,                  // attribute ID
	   2,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   (void*)0            // array buffer offset
	);
	
	
	// Get a handle for our "MVP" uniform
	//MatrixID = glGetUniformLocation(programID, "MVP");
	
	//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
}

void rendermodel::unload(){
}

void rendermodel::render(renderOptions O){
	auto shader_pos = glGetUniformLocation(programID, "position");
	glUniform3f(shader_pos, O.pos.x, O.pos.y, O.pos.z);
		
	glDrawArrays(GL_POINTS, 0, numVerts);
	glDrawArrays(GL_LINES, numVerts, 2*numEdges);
	glDrawArrays(GL_TRIANGLES, numVerts+2*numEdges, 3*numTris);
}

void buffer_vertex(int pos_id, int col_id, int uv_id){
	buffer_position(M->positions[pos_id]);
	buffer_color(M->colors[col_id]);
	buffer_UV(M->UVs[uv_id]);
}
void buffer_edge(m_edge E){
	buffer_vertex(M->position[E.vA], M->colors[E.cA], 0);
	buffer_vertex(M->position[E.vB], M->colors[E.cB], 0);
}
void buffer_triangle(m_triangle T){
	buffer_vertex(M->position[T.vA], M->colors[T.cA], M->UVs[T.UVA]);
	buffer_vertex(M->position[T.vB], M->colors[T.cB], M->UVs[T.UVB]);
	buffer_vertex(M->position[T.vC], M->colors[T.cC], M->UVs[T.UVC]);
}

void buffer_position(vec V){
	buffer.push_back(V.x);
	buffer.push_back(V.y);
	buffer.push_back(V.z);
}
void buffer_UV(vec UV){
	buffer.push_back(UV.x);
	buffer.push_back(UV.y);
}
void buffer_color(vec C){
	buffer.push_back(C.x);
	buffer.push_back(C.y);
	buffer.push_back(C.z);
}


