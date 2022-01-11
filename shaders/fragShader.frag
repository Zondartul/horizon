#version 400
in vec3 Position;
in vec3 Normal;
in vec2 UV;
in vec3 Color;

uniform sampler2D Tex;

uniform bool texturingOn;
uniform bool coloringOn;
uniform bool debuggingOn;
uniform bool transparencyOn;

out vec4 FragColor;

void main(){
	vec4 texColor = texture(Tex, UV);
	//if(!transparencyOn){texColor.a = 1.0f;}
	vec4 color = vec4(Color,1.0);
	FragColor = vec4(1.0,1.0,1.0,1.0);
	if(coloringOn){
		FragColor = FragColor*color;
	}
	if(texturingOn){
		FragColor = FragColor*texColor;
	}
	if(debuggingOn){
		FragColor = vec4(0.25*FragColor.r+0.75*UV.r,0.25*FragColor.g+0.75*UV.g,FragColor.b,1.0);
	}
	//FragColor = color*texColor;//vec4(UV,0.0,0.0)*0.5+texColor*0.5;
}