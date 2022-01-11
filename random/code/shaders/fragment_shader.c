#version 330 core
in vec2 UV;
out vec3 color;
// Interpolated values from the vertex shaders
//in vec3 fragmentColor;
uniform sampler2D myTextureSampler;
void main(){
  //color = fragmentColor;
  color = texture( myTextureSampler, UV ).rgb;
}