
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

float contour( in float d, in float w )
{
    return smoothstep(0.5 - w, 0.5 + w, d);
}
float samp( in vec2 uv, float w )
{
    return contour( texture(m_texture[0],uv).a, w );
}

void main()
{
    float dist = texture( m_texture[0], texCoord ).a;
    float width = fwidth(dist);

    float alpha = contour( dist, width );

    // ------- (comment this block out to get your original behavior)
    // Supersample, 4 extra points
    float dscale = 0.354 / smoothness; // half of 1/sqrt2; you can play with this
    vec2 duv = dscale * vec2(dFdx(texCoord) + dFdy(texCoord));
    vec4 box = vec4(texCoord-duv, texCoord+duv);

    float asum = samp( box.xy, width )
               + samp( box.zw, width )
               + samp( box.xw, width )
               + samp( box.zy, width );

    // weighted average, with 4 extra points having 0.5 weight each,
    // so 1 + 0.5*4.0 = 3 is the divisor
    alpha = (alpha + 0.5 * asum) / 3.0;
    // -------

   	color.rgb =  texture(m_texture[1], texCoord).rgb * vertexColor.rgb;
	color.a = vertexColor.a * alpha;
}