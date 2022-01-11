#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
// Values that stay constant for the whole mesh.
uniform mat4 MVP;
// Notice that the "1" here equals the "1" in glVertexAttribPointer
//layout(location = 1) in vec3 vertexColor;
//out vec3 fragmentColor;

void main(){
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	//fragmentColor = vertexColor;
	UV = vertexUV;
}