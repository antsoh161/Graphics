#version 400
in vec4 Normal;
out vec4 frag_colour;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main () {
	frag_colour = vec4(vec3(0.5,0.5,0.5)+1,0.5);
	frag_colour.r = (Normal.x+1)*0.5;
    frag_colour.g = (Normal.y+1)*0.5;
    frag_colour.b = (Normal.z+1)*0.5;
    
}
