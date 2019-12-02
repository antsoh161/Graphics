#version 440
#define M_PI 3.1415926535897932384626433832795

out vec4 frag_colour;
in vec3 Normal;
in vec3 position;

uniform int smoothness_factor;
uniform int light_count;
uniform vec4 light_position[4]; // can have up to 4 light sources
uniform vec4 light_colour[4];


vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 blinn_phong_brdf(vec3 light_dir, vec3 view_dir, vec3 normal){
  float kL = 0.5;
  float kg = 0.5;
  float f = 1*smoothness_factor;
  
  vec3 H = normalize(light_dir+view_dir);
  float N_dot_H = max(dot(normal,H),0.0);
  float fr = kL/M_PI + (8 + f)/(8*M_PI) * kg * pow(N_dot_H,f);
  return vec3(fr);
}

vec3 cook_torrance_brdf(vec3 light_dir, vec3 view_dir, vec3 normal){
  float roughness = 0.3;
	vec3 H = normalize(light_dir+view_dir);
	
	float V_dot_H = max(dot(view_dir,H),0.0);
  float V_dot_N = max(dot(view_dir,normal),0.0);
  
	float N_dot_H = max(dot(normal,H),0.0);
  float N_dot_V = max(dot(normal,view_dir),0.0);
  
	float L_dot_N = max(dot(light_dir,normal),0.0);
  float L_dot_H = max(dot(light_dir,H),0.0);
  
  if(L_dot_N < 0.0)
    return vec3(0);
  
 
 //New beckmann
  float roughness2 = roughness*roughness;
  float r1 = 1.0 / (M_PI * roughness2 * pow(N_dot_H,4.0));
  float r2 = (N_dot_H * N_dot_H - 1.0) / (roughness2 * N_dot_H * N_dot_H);
  float D = r1 * exp(r2);
 
	//Fresnel term schlick approximation
	float n_1,n_2,R_0; // Index of refraction and reflection coefficient
  n_1 = 1.0f; // 1 = air
  n_2 = 0.46094f; // copper
  R_0 = pow((n_1-n_2)/(n_1+n_2),2);
	float R = R_0 + (1.0-R_0)* pow((1.0f-N_dot_V),5);

  //Fres 2
  float F0 = 0.8;
  float F = pow(1.0 - V_dot_H,5.0);
  F *= (1.0 - F0);
  F += F0;
  //Geometric attenuation
  
  float G = min(1, min((2 * N_dot_H * V_dot_N)/V_dot_H, (2 * N_dot_H * L_dot_N)/V_dot_H));
  
  
  float k_spec = (D*F*G)/(M_PI*V_dot_N*L_dot_N);
  float k_diff = 1/M_PI;
  return vec3(k_diff+k_spec);
}

void main () {
  vec3 objectColour = normalize(vec3(212,175,55));
  vec3 ambient =  0.25*objectColour;  
  vec3 view_dir = normalize(-position.xyz);
  vec3 color_sum = vec3(0);
  vec3 norm = normalize(Normal.xyz);
  
  for (int l = 0; l < light_count;++l){
    vec3 light_dir = normalize(light_position[l].xyz - position.xyz);
    vec3 brdf = cook_torrance_brdf(light_dir, view_dir, norm);
    color_sum += light_colour[l].xyz * brdf * max(dot(light_dir, norm),0.0);    
  }
  frag_colour = vec4(vec3(ambient),1) + vec4(vec3(color_sum),1);
}

