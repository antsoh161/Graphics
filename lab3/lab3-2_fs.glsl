#version 400
#define M_PI 3.1415926535897932384626433832795
in vec3 Normal;
in vec2 UV_coords;
in vec3 position;
uniform samplerCube env_sampler;
out vec4 frag_colour;

uniform int smoothness_factor;
uniform int light_count;
uniform vec4 light_position[4]; // can have up to 4 light sources
uniform vec4 light_colour[4];


vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


vec3 blinn_phong_brdf(vec3 light_dir, vec3 view_direction, vec3 normal){
  float kL = 0.5;
  float kg = 0.5;
  float f = 1*smoothness_factor;
  
  vec3 H = normalize(light_dir+view_direction);
  float N_dot_H = max(dot(normal,H),0.0);
  float fr = kL/M_PI + (8 + f)/(8*M_PI) * kg * pow(N_dot_H,f);
  return vec3(fr);
}

void main () {
  vec3 view_direction = normalize(-position.xyz);
  
  vec3 color_sum = vec3(0);
  vec3 norm = normalize(Normal.xyz);
  vec3 ref_view_dir = reflect(view_direction,norm);
  float r = 0.2;    

  //vec4 tex = texture(env_sampler, UV_coords);
  for (int l = 0; l < light_count;++l){
    vec3 lightDir = normalize(light_position[l].xyz - position.xyz);
    vec3 brdf = blinn_phong_brdf(lightDir, view_direction, norm);
    
    color_sum += light_colour[l].xyz * brdf * max(dot(lightDir, norm),0.0);
  }
  frag_colour =  vec4(color_sum.xyz,0);
    
}

