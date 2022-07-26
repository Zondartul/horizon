#version 330
//was 400
in vec3 Position;
in vec3 Normal;
in vec2 UV;
in vec3 Color;
in vec3 WorldNormal;
in vec3 WorldPosition;

uniform sampler2D Tex;

uniform bool texturingOn;
uniform bool coloringOn;
uniform bool uvColoringOn;
uniform bool normalColoringOn;
uniform bool debuggingOn;
uniform bool transparencyOn;
uniform bool scissoringOn;
uniform vec3 globalColor;
uniform float globalAlpha;
uniform vec4 scissor;
uniform bool lightingOn;
uniform vec3 sunPos;
uniform vec3 sunColor;
uniform vec3 ambientColor;

uniform vec3 WorldCamPos;

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
	//FragColor = vec4(100.0*WorldCamPos,1.0f);
	//if((Position.y > 0)&&(Position.y < 0.5)){FragColor = vec4(1,0,0,1);}
	//if((Position.y > 0)&&(Position.y > 0.5)){FragColor = vec4(0,1,0,1);}
	//if((Position.y < 0)){FragColor = vec4(0,0,1,1);}
	
	vec4 texColor = texture(Tex, UV);
	vec4 color = vec4(Color,1.0);
	FragColor = FragColor*vec4(globalColor,1.0);
	
	vec2 UVmod = mod(UV,vec2(1.0,1.0));
	
	if(transparencyOn){
		FragColor = FragColor*vec4(1.0,1.0,1.0,globalAlpha);
	}
	if(coloringOn){
		FragColor = FragColor*color;
	}
	if(uvColoringOn){
		FragColor = FragColor*vec4(UVmod.x,UVmod.y,1.0-(UVmod.x+UVmod.y)/2.0,1.0);
		if((int(UVmod.x*256)%16) < 1.0){FragColor = vec4(0,0,0,1.0);}
		if((int(UVmod.y*256)%16) < 1.0){FragColor = vec4(0,0,0,1.0);}
		if((UVmod.x == 0) && (UVmod.y == 0)){FragColor = vec4(1.0,1.0,1.0,1.0);}
	}
	if(normalColoringOn){
		vec3 normColor = WorldNormal*0.5+0.5;
		FragColor = FragColor*vec4(normColor.x,normColor.y,normColor.z,1.0);
		
	}
	if(texturingOn){
		FragColor = FragColor*texColor;
	}
	if(lightingOn){
		float attenuation = 1.0;
		float diffuseCoeff = max(0.0,dot(WorldNormal,sunPos));
		float specularExp = 38.4f;
		vec3 surfaceToLight = normalize(sunPos); //actually sundir
		//vec3 surfaceToLight = normalize(sunPos-WorldPosition);
		vec3 surfaceToCamera = normalize(WorldCamPos-WorldPosition);
		vec3 reflectedDir = reflect(-surfaceToLight,WorldNormal);
		vec3 specularColor = vec3(0.5,0.5,0.5);
		float specularCoeff = pow(max(0.0,dot(surfaceToCamera, reflectedDir)),specularExp);
		//0 <= max(0,dot) <= 1 so (0..1)^n always < 1
		FragColor = FragColor*vec4(
			ambientColor+
			sunColor*diffuseCoeff+
			sunColor*specularColor*specularCoeff,
		1.0);
	}
	if(debuggingOn){
		FragColor = vec4(0.25*FragColor.r+0.75*UV.r,0.25*FragColor.g+0.75*UV.g,FragColor.b,1.0);
	}
	if(FragColor.a == 0.0){ discard; }
	
}