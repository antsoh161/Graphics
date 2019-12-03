#version 440
#define M_PI 3.1415926535897932384626433832795

out vec4 frag_colour;
in vec3 Normal;
in vec3 world_pos;
in vec3 view_pos;

uniform float roughness_factor;
uniform int light_count;
uniform vec4 light_position[4]; // can have up to 4 light sources
uniform vec4 light_colour[4];

void main(){
	frag_colour = vec4(0.5);
	
}