#version 330

in vec2 texCoord; 
in vec3 fragNormal; 
in vec3 position;
in vec4 vertexColor;

layout (location = 0) out vec3 WorldPosOut; 
layout (location = 1) out vec3 DiffuseOut; 
layout (location = 2) out vec3 NormalOut; 
layout (location = 3) out vec3 TexCoordOut;  

uniform sampler2D gColorMap; 

void main()	
{	
	WorldPosOut     = position;					
	DiffuseOut      = vertexColor.xyz * texture(gColorMap, texCoord).xyz;	
	NormalOut       = normalize(fragNormal);					
	TexCoordOut     = vec3(texCoord, 0.0);	
}
