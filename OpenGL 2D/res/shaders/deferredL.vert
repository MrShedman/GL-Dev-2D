#version 330

uniform mat4 LProjectionMatrix;
uniform mat4 LModelViewMatrix;

layout (location = 0) in vec3 LVertexPos; 

void main()
{
	gl_Position = vec4( LVertexPos, 1.0 );
}