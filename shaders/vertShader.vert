#version 400
in vec3 VertexPosition;
in vec3 VertexColor;
in vec3 VertexNormal;
in vec2 VertexUV;


out vec3 Position;
out vec3 Normal;
out vec2 UV;
out vec3 Color;

uniform mat4 MVP;

void main(){
	gl_Position = MVP*vec4(VertexPosition,1.0);
	Position = (MVP*vec4(VertexPosition,1.0)).xyz;
	Normal = normalize(MVP*vec4(VertexNormal,1.0)).xyz;
	UV = VertexUV;
	Color = VertexColor;
}