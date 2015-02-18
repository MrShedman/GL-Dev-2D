#version 150

uniform mat4 model;
//Texture Color
uniform sampler2D m_texture[2]; 

in vec3 normal;

//Texture coordinate
in vec2 texCoord;

//Color
in vec4 vertexColor;

//Final color
out vec4 color;

uniform struct Light 
{
   vec3 position;
   vec3 intensities; //a.k.a the color of the light
} light;

void main()
{
	//final color
	color = vertexColor * texture( m_texture[0], texCoord );
}
