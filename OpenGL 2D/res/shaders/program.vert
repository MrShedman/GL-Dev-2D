#version 150

//Transformation Matrices
uniform mat4 LProjectionMatrix;
uniform mat4 LModelViewMatrix;

//Vertex position attribute
in vec3 LVertexPos;
in vec2 LTexCoord;
in vec4 LVertexColor;

out vec2 texCoord;
out vec4 vertexColor;

void main()
{
	//Process texCoord
	texCoord = LTexCoord;
	
	//Process color
	vertexColor = LVertexColor;
			
	gl_Position = (LProjectionMatrix * LModelViewMatrix) * vec4( LVertexPos, 1.0 );

}