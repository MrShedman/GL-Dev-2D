#version 150

//Texture Color
uniform sampler2D m_texture[2]; 

//Texture coordinate
in vec2 texCoord;

//Color
in vec4 vertexColor;

//Final color
out vec4 color;

void main()
{
	//final color
	color = vertexColor * texture( m_texture[0], texCoord );
}
