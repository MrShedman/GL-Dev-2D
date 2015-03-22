#version 150

//Texture Color
uniform sampler2D m_texture[2]; 

in mat4 model;
in vec3 position;
in vec3 fragNormal;
in vec2 texCoord;
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
	vec3 pos = (model * vec4( position, 1.0 )).xyz;

    vec3 normal = normalize(transpose(inverse(mat3(model))) * fragNormal);
 
    //calculate the vector from this pixels surface to the light source
    vec3 surfaceToLight = light.position - pos;

    //calculate the cosine of the angle of incidence
    float brightness = dot(normal, surfaceToLight) / length(surfaceToLight);
    brightness = clamp(brightness, 0, 1);

    vec4 surfaceColor = vertexColor * texture(m_texture[0], texCoord);
    color = vec4(brightness * light.intensities * surfaceColor.rgb, surfaceColor.a);
}
