
#version 150

uniform sampler2D   m_texture[2];

//Texture coordinate
in vec2 texCoord;

//Color
in vec4 vertexColor;

//Final color
out vec4 color;

const float smoothness = 64.0;

const float gamma = 2.2;

const bool glow = false;

const bool shadow = false;

const vec2 shadowOffset = vec2(0.001, 0.001) * 2;
const vec2 shadowAlphaRange = vec2(0.28, 0.54);
const vec2 alphaRange = vec2(0.49, 0.54);
void main()
{
	float sdf = texture(m_texture[0], texCoord).r;

	// perform adaptive anti-aliasing of the edges
	float w = clamp( smoothness * (abs(dFdx(texCoord.x)) + abs(dFdy(texCoord.y))), 0.0, 0.5);
	float a = smoothstep(0.5-w, 0.5+w, sdf);

	// gamma correction for linear attenuation
	a = pow(a, 1.0/gamma);
	
	float min = 0.0;
	float max = 0.5;
	
	if (shadow)
	{
		float alpha = smoothstep(alphaRange.x, alphaRange.y, sdf);

		vec2 shadowUV = texCoord - shadowOffset;//vec2(shadowOffset.x * texCoord.x, shadowOffset.y * texCoord.y);
		vec4 shadowtexel = texture(m_texture[0], shadowUV);
		float p = 1.0;
		if (shadowtexel.w < 0.5)
		{
			p = 0.0;
		}
		
		float shadowAlpha = smoothstep(shadowAlphaRange.x, shadowAlphaRange.y, shadowtexel.w);
		vec4 shadow = vec4(0.0, 0.0, 0.0, p) * shadowAlpha;

		color = mix(shadow, vertexColor, alpha);
			
		return;
	}
	
	if (sdf >= min && sdf <= max && glow)
	{
		float alpha = smoothstep(min, max, sdf);
		vec4 outer = vec4(0.0, 0.5, 1.0, alpha);
		color = mix(outer, color, color.a);
	}
	else
	{
		// final color
		color.rgb =  texture(m_texture[1], texCoord).rgb * vertexColor.rgb;
		color.a = vertexColor.a * a;
	}
};