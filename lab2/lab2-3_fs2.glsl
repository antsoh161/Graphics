#version 440
#define M_PI 3.1415926535897932384626433832795

out vec4 frag_colour;
in vec3 Normal;
in vec3 world_pos;
in vec3 view_pos;

uniform float smoothness_factor;
uniform int light_count;
uniform vec4 light_position[4]; // can have up to 4 light sources
uniform vec4 light_colour[4];

vec3 blinn_phong_brdf(vec3 light_dir, vec3 view_dir, vec3 normal){
  float kL = 0.1;
  float kg = 0.9;
  float f = 10*smoothness_factor;
  
  vec3 H = normalize(light_dir+view_dir);
  float N_dot_H = max(dot(normal,H),0.0);
  float fr = kL/M_PI + (8 + f)/(8*M_PI) * kg * pow(N_dot_H,f);
  return vec3(fr);
}


void main(){
	vec3 object_colour = vec3(0.5,0.5,0.5);
	vec3 ambient = 0.25*object_colour;
	vec3 V = normalize(view_pos - world_pos);
	vec3 N = normalize(Normal);
		
	vec3 light_out = vec3(0.0);
	for(int l = 0; l < light_count; ++l){
		vec3 L = normalize(light_position[l].xyz - world_pos);
		vec3 brdf = blinn_phong_brdf(L,V,N);
		light_out += light_colour[l].xyz * brdf * max(dot(L,N),0.0);
	}
	frag_colour = vec4(ambient+light_out,1.0);
}