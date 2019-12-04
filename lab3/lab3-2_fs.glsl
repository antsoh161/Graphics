#version 400
#define M_PI 3.1415926535897932384626433832795
in vec3 world_pos;
in vec3 Normal;
in vec2 UV_coords;

uniform samplerCube env_sampler;
uniform vec3 view_pos;
uniform float roughness_factor;
uniform int light_count;
uniform vec4 light_position[4]; // can have up to 4 light sources
uniform vec4 light_colour[4];

out vec4 frag_colour;

vec3 blinn_phong_brdf(vec3 light_dir, vec3 view_dir, vec3 normal){
  float kL = 0.1;
  float kg = 0.9;
  float f = 10*roughness_factor;
  
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

float beckmannDistribution(vec3 normal, vec3 H, float roughness){
  float NdotH = max(dot(normal,H),0.0);
  float cos2r = NdotH * NdotH;
  float tan2r = (cos2r-1.0)/cos2r;
  float r2 = roughness*roughness;
  float denom = M_PI * r2 * cos2r * cos2r;
  return exp(tan2r/r2)/denom;
  
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
/*
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
*/
  
vec3 cook_torrance_brdf(vec3 L, vec3 V, vec3 N){
  vec3 albedo = normalize(vec3(253,227,78)); // Albedo of gold
  float metallic = 0.0;
  vec3 F0 = vec3(0.04);
  F0 = mix(F0,albedo,metallic);
  vec3 H = normalize(V+L);
  float NDF = DistributionGGX(N,H,roughness_factor);
  float G = GeometrySmith(N,V,L,roughness_factor);
  //float G = beckmannDistribution(N,H,roughness_factor);
  vec3 F = fresnelSchlick(max(dot(H,V),0.0),F0);
  
  vec3 nom = NDF*G*F;
  float denom = 4*max(dot(N,V),0.0) * max(dot(N,L),0.0) + 0.001; //Prevent divide by 0
  vec3 k_spec = nom / denom;
  
  vec3 kg = F;
  vec3 kL = vec3(1.0) - kg; // kg + kL <= 1
  
  kL *= 1.0 - metallic; // metals do not have diffuse lightning
  
  return (kL * albedo/M_PI + k_spec);
}


void main () {
    
  vec3 objectColour = vec3(1,1,1);
  /*
	objectColour.r = (Normal.x+1)*0.5;
  objectColour.g = (Normal.y+1)*0.5;
  objectColour.b = (Normal.z+1)*0.5;
  */
  
  vec3 ambient =  0.5*objectColour; 
	vec3 V = normalize(view_pos-world_pos);
	vec3 N = normalize(Normal);
	vec3 ref_V = reflect(-V,N);
  vec4 tex = texture(env_sampler, ref_V);
  
	vec3 light_out = vec3(0.0);
	for(int l = 0; l < light_count; ++l){
		vec3 L = normalize(light_position[l].xyz - world_pos);
		vec3 brdf = cook_torrance_brdf(L,V,N);
		light_out += light_colour[l].xyz * brdf * max(dot(L,N),0.0);
	}
  
  float r = 0.5; 
	vec4 lab2_colour = vec4(light_out,1.0);
  
  frag_colour = r*tex + (1-r)*lab2_colour;
	
    
}
