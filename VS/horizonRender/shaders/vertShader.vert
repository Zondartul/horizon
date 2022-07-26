#version 330
//was 400
in vec3 VertexPosition;
in vec3 VertexColor;
in vec3 VertexNormal;
in vec2 VertexUV;
uniform vec2 TexOffset;
uniform vec2 TexSize;
uniform vec3 globalColor;
uniform vec2 TexPos;
uniform vec2 TexScale;

out vec3 Position;
out vec3 WorldPosition;
out vec3 WorldNormal;
out vec3 Normal;
out vec2 UV;
out vec3 Color;

uniform mat4 VP;
uniform mat4 M;
uniform mat4 Mrot;

void main(){
	mat4 MVP = VP*M;
	gl_Position = MVP*vec4(VertexPosition,1.0);
	WorldPosition = (M*vec4(VertexPosition,1.0)).xyz;
	Position = (MVP*vec4(VertexPosition,1.0)).xyz;
	Normal = normalize(MVP*vec4(VertexNormal,1.0)).xyz;
	WorldNormal = normalize((Mrot*vec4(VertexNormal,1.0)).xyz);
	UV = (VertexUV*TexSize + TexPos)*TexScale+TexOffset;
	Color = VertexColor;
}