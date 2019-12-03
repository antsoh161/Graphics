#version 440
#define M_PI 3.1415926535897932384626433832795

out vec4 frag_colour;
in vec3 Normal;
in vec3 position;
in vec3 view_pos;

uniform float roughness_factor;
uniform int light_count;
uniform vec4 light_position[4]; // can have up to 4 light sources
uniform vec4 light_colour[4];



vec3 blinn_phong_brdf(vec3 light_dir, vec3 view_dir, vec3 normal){
  float kL = 0.5;
  float kg = 0.5;
  float f = 100*roughness_factor;
  
  vec3 H = normalize(light_dir+view_dir);
  float N_dot_H = max(dot(normal,H),0.0);
  float fr = kL/M_PI + (8 + f)/(8*M_PI) * kg * pow(N_dot_H,f);
  return vec3(fr);
}

vec3 fresnelSchlick(float V_dot_H, vec3 F0){
  return F0 + (1.0 - F0) * pow(1.0 - V_dot_H,5.0);
}

float DistributionGGX(vec3 normal, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(normal, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = M_PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 cook_torrance_brdf(vec3 light_dir, vec3 view_dir, vec3 normal){
  float roughness = roughness_factor;
  float metallic = 1.0;
	vec3 H = normalize(light_dir+view_dir);
	
	float V_dot_H = max(dot(view_dir,H),0.0);
  float V_dot_N = max(dot(view_dir,normal),0.0);
  
	float N_dot_H = max(dot(normal,H),0.0);
  float N_dot_V = max(dot(normal,view_dir),0.0);
  
	float L_dot_N = max(dot(light_dir,normal),0.0);
  float L_dot_H = max(dot(light_dir,H),0.0);
  
  
 
 //New beckmann
  float roughness2 = roughness*roughness;
  float r1 = 1.0 / (M_PI * roughness2 * pow(N_dot_H,4.0));
  float r2 = (N_dot_H * N_dot_H - 1.0) / (roughness2 * N_dot_H * N_dot_H);
  float D = r1 * exp(r2);

  D = DistributionGGX(normal,H,roughness_factor);
 
	//Fresnel term schlick approximation
	float n_1,n_2,R_0; // Index of refraction and reflection coefficient
  n_1 = 1.0; // 1 = air
  n_2 = 0.46094; // copper
  R_0 = pow((n_1-n_2)/(n_1+n_2),2);
	float R = R_0 + (1.0-R_0)* pow((1.0f-N_dot_V),5);

  //Fres 2
  float F0 = 0.8;
  float F = pow(1.0 - V_dot_H,5.0);
  F *= (1.0 - F0);
  F += F0;
  
  vec3 Fvec = fresnelSchlick(V_dot_H,vec3(1.0,0.71,0.29));
  
  
  //Geometric attenuation
  float g_1 = 2.0*N_dot_H*V_dot_N;
  g_1 = g_1/(V_dot_H);
  float g_2 = (2.0*N_dot_H*L_dot_N);
  g_2 = g_2/V_dot_H;
  float G = min(1.0, min(g_1,g_2));
  
  G = GeometrySmith(normal,view_dir,light_dir,roughness_factor);


  float k_spec = (D*F*G)/(4*V_dot_N*L_dot_N);
  float k_diff = 0.1;
  float kg = 0.9;
  float kL = (1-kg)*(1-metallic);
  
  float fr = kL/M_PI + kg * k_spec;
  
  return vec3(fr);
}

void main () {
  vec3 objectColour = normalize(vec3(212,175,55));
  vec3 ambient =  objectColour;  
  vec3 view_dir = normalize(-view_pos);
  vec3 color_sum = vec3(0);
  vec3 norm = normalize(Normal.xyz);
  vec3 brdf;
  
  for (int l = 0; l < light_count;++l){
    vec3 light_dir = normalize(light_position[l].xyz - position.xyz);
    brdf = blinn_phong_brdf(light_dir, view_dir, norm);
    color_sum += light_colour[l].xyz * brdf * max(dot(light_dir, norm),0.0);    
  }
  frag_colour = vec4(vec3(ambient),1) + vec4(vec3(color_sum),1);
}

