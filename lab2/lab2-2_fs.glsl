#version 440
#define M_PI 3.1415926535897932384626433832795

out vec4 frag_colour;
in vec4 Normal;
in vec4 position;

uniform int light_count;
uniform vec4 light_position[4]; // can have up to 4 light sources
uniform vec4 light_colour[4];


vec4 subsurface_albedo = vec4(1,1,1,0); //Diffuse coloring c_diff 

vec4 lambertian_brdf( vec4 in_direction, vec4 out_direction, vec4 normal )
{ 
  return subsurface_albedo/M_PI;
}


void main () {
  vec4 objectColour = vec4(0.5,0.5,0.0,5);
  vec4 lightSum = vec4(0);
  for (int l = 0; l < light_count; ++l )
{     
      lightSum +=  light_colour[l] * lambertian_brdf(light_position[l],vec4(0),Normal) * (max(dot( normalize(Normal), normalize(light_position[l] - position)),0.0));
    // Implement Equation 1 from the lab instructions: (incoming light colour) * (brdf) * (cosine)
  }
  frag_colour = objectColour + lightSum;
}
