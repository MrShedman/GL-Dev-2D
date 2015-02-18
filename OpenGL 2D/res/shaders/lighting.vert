#version 150

//Transformation Matrices
uniform mat4 LProjectionMatrix;
uniform mat4 LModelViewMatrix;

//Vertex position attribute
in vec3 LVertexPos;
in vec3 LNormal;
in vec2 LTexCoord;
in vec4 LVertexColor;

out vec3 position;
out vec3 fragNormal;
out vec2 texCoord;
out vec4 vertexColor;

void main()
{
	position = (LModelViewMatrix * vec4( LVertexPos, 1.0 )).xyz;

    fragNormal = normalize(transpose(inverse(mat3(LModelViewMatrix))) * LNormal);

	//Process texCoord
	texCoord = LTexCoord;
	
	//Process color
	vertexColor = LVertexColor;
			
	gl_Position = (LProjectionMatrix * LModelViewMatrix) * vec4( LVertexPos, 1.0 );

}