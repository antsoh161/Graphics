#version 440
#define M_PI 3.1415926535897932384626433832795

out vec4 frag_colour;
in vec4 Normal;
in vec4 position;
in vec4 viewPosition;

uniform int light_count;
uniform vec4 light_position[4]; // can have up to 4 light sources
uniform vec4 light_colour[4];


vec4 blinn_phong_brdf(vec4 in_direction, vec4 out_direction, vec4 normal){
  float kL = 0.1;
  float kg = 0.9;
  float f = 10;
  float N_dot_H = max(dot(normalize(Normal), out_direction),0.0);
  float fr = kL/M_PI + (8 + f)/(8*M_PI)* kg * pow(N_dot_H,f);
  return vec4(fr);
}

void main () {
  vec4 objectColour = vec4(1, 0.8, 0.5, 0);
  vec4 ambient = 0.2 * objectColour;  
  vec4 viewDir = normalize(-position);
  vec4 outSum;
  
  for (int l = 0; l < light_count;++l){
    vec4 lightDir = normalize(light_position[l] - position);
    vec4 H = normalize(lightDir + viewDir);
    vec4 bp = blinn_phong_brdf(lightDir, H, Normal);
  
    outSum += light_colour[l] * bp * max(dot( normalize(Normal),lightDir),0.0);
  }
  frag_colour = ambient + outSum;
}

