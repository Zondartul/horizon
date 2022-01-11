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
uniform bool scissoringOn;
uniform vec3 globalColor;
uniform float globalAlpha;
uniform vec4 scissor;

out vec4 FragColor;

void main(){
	if(scissoringOn){
		if((Position.x < scissor.x)
		||(Position.y < scissor.y)
		||(Position.x > scissor.z)
		||(Position.y > scissor.w)){
			discard;
		}
	}
	FragColor = vec4(1.0,1.0,1.0,1.0);
	//if((Position.y > 0)&&(Position.y < 0.5)){FragColor = vec4(1,0,0,1);}
	//if((Position.y > 0)&&(Position.y > 0.5)){FragColor = vec4(0,1,0,1);}
	//if((Position.y < 0)){FragColor = vec4(0,0,1,1);}
	
	vec4 texColor = texture(Tex, UV);
	vec4 color = vec4(Color,1.0);
	FragColor = FragColor*vec4(globalColor,1.0);
	if(transparencyOn){
		FragColor = FragColor*vec4(1.0,1.0,1.0,globalAlpha);
	}
	if(coloringOn){
		FragColor = FragColor*color;
	}
	if(texturingOn){
		FragColor = FragColor*texColor;
	}
	if(debuggingOn){
		FragColor = vec4(0.25*FragColor.r+0.75*UV.r,0.25*FragColor.g+0.75*UV.g,FragColor.b,1.0);
	}
	
}