#version 400
out vec4 o_fragment_color; // The final colour

// User input:
uniform vec2 i_window_size;
uniform float i_global_time;
uniform vec4 i_mouse_state;

// Camera parameters:
uniform vec3 i_position;
uniform vec3 i_up;
uniform vec3 i_right;
uniform vec3 i_dir;
uniform float i_focal_dist;
uniform int brdf_swapper;

#define M_PI 3.1415926535897932384626433832795
#define MAX_DEPTH 10
#define NUM_SPHERES 5
#define NUM_TRIANGLES 4
#define MAX_SAMPLES 1

struct Ray { vec3 origin, dir;float weight; };
Ray ray_stack[MAX_DEPTH];
int ray_stack_size = 0;

void push(Ray ray){
  if(ray_stack_size < MAX_DEPTH)
  {
    ray_stack[ray_stack_size] = ray;
    ray_stack_size++;
  }
}

Ray pop(){
  if(ray_stack_size > 0)
  {
    ray_stack_size--;
    return ray_stack[ray_stack_size];
  }
}

struct Material{
  vec3 color_emission;
  vec3 color_diffuse;
  vec3 color_glossy;
  vec3 color_brdf;

  float roughness;
  float alpha;
  float metallic;
  float reflection;
  float transmission;
  float ior;
};

struct Intersection
{
  bool hit;
  float t;
	vec3 point;
	vec3 normal;
  vec3 light_dir;
  vec3 view_dir;
	Material material;
};

struct Sphere {
  float radius;
  vec3 center;
  Material material;
};

struct Plane {
  float offset;
  vec3 normal;
  Material material;
};

struct Triangle {
  vec3 normal;
  vec3 p0;
  vec3 p1;
  vec3 p2;
  vec3 point;
  Material material;
};

struct Scene {
  Sphere spheres[NUM_SPHERES];
  Plane ground_plane[1];
  Triangle triangles[NUM_TRIANGLES];
	vec3 sun_position;
  float sun_brightness;
};





Scene scene;
int test = 0;
void build_tetra(float side_len = 1, vec3 center = vec3(0)){
  /*
  vec3 v0 = vec3(0,0,1.0f)*side_len;
  vec3 v1 = vec3(-1.0f,0,0)*side_len;
  vec3 v2 = vec3(1.0f,0,0)*side_len;
  vec3 v3 = vec3(0,1.0f,0)*side_len;
  */
  vec3 v0 = vec3(1.0, 0.0, -1.0/sqrt(2.0))*side_len;
  vec3 v1 = vec3(-1.0, 0.0, -1.0/sqrt(2.0))*side_len;
  vec3 v2 = vec3(0.0, 1.0, 1.0/sqrt(2.0))*side_len;
  vec3 v3 = vec3(0.0,-1.0, 1.0/sqrt(2.0))*side_len;
  /*
  vec3 v0 = vec3(-1.0, -1.0, -1.0);
  vec3 v1 = vec3(0, 1.0, -1.0);
  vec3 v2 = vec3(1.0, -1.0, -1.0);
  vec3 v3 = vec3(0, 0, 1.0);
  */
  /*
  //Left side
  scene.triangles[0].p0 = v0 + center;
  scene.triangles[0].p1 = v1 + center;
  scene.triangles[0].p2 = v3 + center;
  //Right side
  scene.triangles[1].p0 = v0 + center;
  scene.triangles[1].p1 = v2 + center;
  scene.triangles[1].p2 = v3 + center;
  //Front side
  scene.triangles[2].p0 = v0 + center;
  scene.triangles[2].p1 = v1 + center;
  scene.triangles[2].p2 = v2 + center;
  //Bottom side
  scene.triangles[3].p0 = v1 + center;
  scene.triangles[3].p1 = v2 + center;
  scene.triangles[3].p2 = v3 + center;
  */
  //Left side
  scene.triangles[0].p0 = v0+center;
  scene.triangles[0].p1 = v1+center;
  scene.triangles[0].p2 = v2+center;
  //Right side
  scene.triangles[1].p0 = v0+center;
  scene.triangles[1].p1 = v3+center;
  scene.triangles[1].p2 = v1+center;
  //Front side
  scene.triangles[2].p0 = v1+center;
  scene.triangles[2].p1 = v3+center;
  scene.triangles[2].p2 = v2+center;
  //Bottom side
  scene.triangles[3].p0 = v0+center;
  scene.triangles[3].p1 = v2+center;
  scene.triangles[3].p2 = v3+center;
}

void init( float sun_bright )
{
	// Hard-coded single point light source
	scene.sun_brightness = 10;
	scene.sun_position = vec3(6e3,  1e4, 1e4);

	// Initialise 5 spheres and a ground plane
  //Red medium
	scene.spheres[0].center = vec3(0, 0.3, 0.5) ;
	scene.spheres[0].radius = 0.3;
	scene.spheres[0].material.color_diffuse = vec3( 0.3, 0.1, 0.1 );
	scene.spheres[0].material.color_glossy = vec3( 1 );
	scene.spheres[0].material.color_emission = vec3( 0 );
	scene.spheres[0].material.roughness = 100;
	scene.spheres[0].material.reflection = 1;
	scene.spheres[0].material.transmission = 0;
	scene.spheres[0].material.ior = 1;
  scene.spheres[0].material.color_brdf = vec3(0);
  scene.spheres[0].material.alpha = 0.3;
  scene.spheres[0].material.metallic = 0.0;
  //Green medium
	scene.spheres[1].center = vec3(0.8, 0.3, 0.8);
	scene.spheres[1].radius = 0.3;
	scene.spheres[1].material.color_diffuse = 0.1 * vec3( 0.0, 1.0, 0.0 );
	scene.spheres[1].material.color_glossy = vec3( 1 );
	scene.spheres[1].material.roughness = 5000;
	scene.spheres[1].material.color_emission = vec3( 0 );
	scene.spheres[1].material.reflection = 0;
	scene.spheres[1].material.transmission = 1;
	scene.spheres[1].material.ior = 0.7;
  scene.spheres[1].material.color_brdf = vec3(0);
  scene.spheres[1].material.alpha = 0.8;
  scene.spheres[1].material.metallic = 0.0;
  //Red small
	scene.spheres[2].center = vec3(0.55, 0.1, 0.2) ;
	scene.spheres[2].radius = 0.1;
	scene.spheres[2].material.color_diffuse = 0.8 * vec3( 0.0, 0.0, 0.0 );
	scene.spheres[2].material.color_glossy = vec3( 1 );
	scene.spheres[2].material.roughness = 1000;
	scene.spheres[2].material.color_emission = vec3( 3.6, 0, 0 );
	scene.spheres[2].material.reflection = 0.0;
	scene.spheres[2].material.transmission = 0.0;
	scene.spheres[2].material.ior = 1.4;
  scene.spheres[2].material.color_brdf = vec3(0);
  scene.spheres[2].material.alpha = 0.1;
  scene.spheres[2].material.metallic = 0.0;
  //Largest
	scene.spheres[3].center = vec3(0.7, 0.8, -0.5) ;
	scene.spheres[3].radius = 0.8;
	scene.spheres[3].material.color_diffuse = 0.5 * vec3( 0.25, 0.2, 0.15 );
	scene.spheres[3].material.color_glossy = vec3( 1 );
	scene.spheres[3].material.roughness = 10;
	scene.spheres[3].material.color_emission = vec3( 0 );
	scene.spheres[3].material.reflection = 0.0;
	scene.spheres[3].material.transmission = 0.0;
	scene.spheres[3].material.ior = 1.0;
  scene.spheres[3].material.color_brdf = vec3(0);
  scene.spheres[3].material.alpha = 0.50;
  scene.spheres[3].material.metallic = 0.0;
  //Second largest
	scene.spheres[4].center = vec3(-0.65, 0.6, -1) ;
	scene.spheres[4].radius = 0.6;
	scene.spheres[4].material.color_diffuse = 0.5 * vec3( 0.5, 0.4, 0.25 );
	scene.spheres[4].material.color_glossy = vec3( 1 );
	scene.spheres[4].material.roughness = 1000;
	scene.spheres[4].material.color_emission = vec3( 0 );
	scene.spheres[4].material.reflection = 0.0;
	scene.spheres[4].material.transmission = 0;
	scene.spheres[4].material.ior = 1;
  scene.spheres[4].material.color_brdf = vec3(0);
  scene.spheres[4].material.alpha = 0.5;
  scene.spheres[4].material.metallic = 1.0;

  // tetrahedron /\

  build_tetra(0.5,vec3(-2,1,0));
  for(int i = 0; i < NUM_TRIANGLES; i++){
    scene.triangles[i].normal = normalize(cross(scene.triangles[i].p1 - scene.triangles[i].p0, scene.triangles[i].p2 - scene.triangles[i].p0));
    scene.triangles[i].material.color_diffuse = 10*vec3(0.2);
    scene.triangles[i].material.color_glossy = vec3(1);
    scene.triangles[i].material.roughness = 100;
    scene.triangles[i].material.color_emission = vec3(0);
    scene.triangles[i].material.reflection = 1.0;
    scene.triangles[i].material.transmission = 0.0;
    scene.triangles[i].material.ior = 1.0;
    scene.triangles[i].material.color_brdf = vec3(0);
    scene.triangles[i].material.alpha = 0.50;
    scene.triangles[i].material.metallic = 0.0;
  }
  /*
    scene.triangles[0].normal = cross(scene.triangles[0].p1 - scene.triangles[0].p0, scene.triangles[0].p2 - scene.triangles[0].p0);
    scene.triangles[0].material.color_diffuse = vec3(0,1,1);
    scene.triangles[0].material.color_glossy = vec3(1);
    scene.triangles[0].material.roughness = 100;
    scene.triangles[0].material.color_emission = vec3(1);
    scene.triangles[0].material.reflection = 0.0;
    scene.triangles[0].material.transmission = 0.0;
    scene.triangles[0].material.ior = 0.0;
    scene.triangles[0].material.color_brdf = vec3(0);
    scene.triangles[0].material.alpha = 0.50;
    scene.triangles[0].material.metallic = 0.0;
    scene.triangles[0].material.color_diffuse = vec3(0,1,1);
    scene.triangles[0].material.color_diffuse = vec3(0,1,0);
		*/

	scene.ground_plane[0].normal = vec3(0,1,0);
	scene.ground_plane[0].offset = 0;
	scene.ground_plane[0].material.color_diffuse = 1.0 * vec3( 0.6 );
	scene.ground_plane[0].material.color_glossy = vec3( 0 );
	scene.ground_plane[0].material.roughness = 0;
	scene.ground_plane[0].material.color_emission = vec3( 0 );
	scene.ground_plane[0].material.reflection = 0.0;
	scene.ground_plane[0].material.transmission = 0;
	scene.ground_plane[0].material.ior = 1;
  scene.ground_plane[0].material.color_brdf = vec3(0);
  scene.ground_plane[0].material.alpha = 0;
  scene.ground_plane[0].material.metallic = 0;

}
/*
vec3 blinn_phong_brdf(vec3 light_dir, vec3 view_dir, vec3 normal){
  float roughness_factor = 5;
  float kL = 0.5;
  float kg = 0.5;
  float f = 10*roughness_factor;
  vec3 H = normalize(light_dir+view_dir);
  float N_dot_H = max(dot(normal,H),0.0);
  float fr = kL/M_PI + (8 + f)/(8*M_PI) * kg * pow(N_dot_H,f);

  return vec3(fr);
}*/

vec3 blinn_phong_brdf(vec3 light_dir, vec3 view_dir, vec3 normal, Material m){
  float roughness_factor = m.alpha;
  vec3 kL = m.color_diffuse;
  vec3 kg = m.color_glossy;
  float f = m.roughness;
  vec3 H = normalize(light_dir+view_dir);
  float N_dot_H = max(dot(normal,H),0.0);
  vec3 fr = kL/M_PI + (8 + f)/(8*M_PI) * kg * pow(N_dot_H,f);

  return fr;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

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
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
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
vec3 fresnelSchlick(float V_dot_H, vec3 F0){
  return F0 + (1.0 - F0) * pow(1.0 - V_dot_H,5.0);
}
float refraction_fresnel(vec3 L, vec3 N, const float ior){
  float cosI = clamp(-1,1,dot(L,N));
  float etai = 1;
  float etat = ior;
  if(cosI > 0)  {
    float tmp = etat;
    etat = etai;
    etai = tmp;
  }
  //Snell's law
  float kr;
  float sint = etai / etat * sqrt(max(0.0, 1-cosI*cosI));
  if(sint >= 1)
    kr = 1;
   else{
      float cost = sqrt(max(0.0, 1-sint*sint));
      cosI = abs(cosI);
      float rs = ((etai*cosI)-(etai*cost))/((etat*cosI)+(etai*cost));
      float rp = ((etai*cosI)-(etat*cost))/((etat*cosI)+(etat*cost));
      kr = (rs*rs+rp*rp)/2.0;
   }
   return kr;
}


vec3 cook_torrance_brdf(vec3 L, vec3 V, vec3 N, Material m){
  float roughness_factor = m.alpha; //Smoothness to roughness
  vec3 F0 = vec3(0.04); // Changed to look more like blinn-phong
  F0 = mix(F0,m.color_diffuse,m.metallic);
  vec3 H = normalize(V+L);
  float NDF = DistributionGGX(N,H,roughness_factor);
  float G = GeometrySmith(N,V,L,roughness_factor);
  //float G = GeometrySchlickGGX(max(dot(N,V),0.0),roughness_factor);
  //float G = beckmannDistribution(N,H,m.roughness);
  vec3 F = fresnelSchlick(max(dot(H,V),0.0),F0);
  vec3 nom = NDF*G*F;
  float denom = 4*max(dot(N,V),0.0) * max(dot(N,L),0.0) + 0.001; //Prevent divide by 0
  vec3 k_spec = nom / denom;
  vec3 kg = F;
  vec3 kL = vec3(1.0) - kg; // kg + kL <= 1
  kL = vec3(0.5);
  kL *= 1.0 - m.metallic; // metals do not have diffuse lightning
  return (kL * m.color_diffuse/M_PI + k_spec);
}


// This function computes a nice-looking sky sphere, with a sun.
vec3 simple_sky(vec3 direction)
{
	float emission_sky = 1e-1*scene.sun_brightness;
	float emission_sun = 1.0*scene.sun_brightness*scene.sun_brightness;
  vec3 sky_color = vec3(0.35, 0.65, 0.85);
  vec3 haze_color = vec3(0.8, 0.85, 0.9);
  vec3 light_color = vec3(1);

  float sun_spread = 2500.0;
  float haze_spread = 1.3;
  float elevation = acos(direction.y);

  float angle = abs(dot(direction, normalize(scene.sun_position)));
  float response_sun = pow(angle, sun_spread);
  float response_haze = pow(elevation, haze_spread);

  vec3 sun_component = mix(emission_sky*sky_color, emission_sun*light_color,response_sun);
  vec3 haze_component = mix(vec3(0),  emission_sky*haze_color,response_haze);

  return (sun_component+haze_component);
}


float intersect(Ray ray, Sphere s)
{
  float t = -1;
  vec3 oc = ray.origin - s.center;
  float a = dot(ray.dir,ray.dir);
  float b = 2.0*dot(oc,ray.dir);
  float c = dot(oc,oc) - s.radius * s.radius;
  float discriminant = b*b - 4.0*a*c;
  if(discriminant < 0)
    t = -1.0;
  else{
    t = (-b - sqrt(discriminant))/(2.0*a);
  }

  return t;
  // YOUR CODE GOES HERE.
  // Return closest distance t for a ray/sphere intersection.
}

float intersect(Ray ray, Plane p)
{
  vec3 norm = normalize(p.normal);
  float denom = dot(norm,ray.dir);
  vec3 p0l0 =  p.offset - ray.origin;
  float t = dot(p0l0,norm)/denom;
  return t;
  //YOUR CODE GOES HERE.
  // Return closest distance t for a ray/plane intersection.
}

float intersect(Ray ray, Triangle tri){
  const float eps = 0.000001;
  vec3 e1 = tri.p1 - tri.p0;
  vec3 e2 = tri.p2 - tri.p0;
  vec3 h = cross(ray.dir,e2);
  float a = dot(e1,h);
  if(a > -eps && a < eps)
    return -1; //Paralell triangle
  float f = 1.0/a;
  vec3 s = ray.origin - tri.p0;
  float u = f*dot(s,h);
  if( u < 0.0 || u > 1.0)
    return -1;
  vec3 q = cross(s,e1);
  float v = f * dot(ray.dir,q);
  if( v < 0.0 || ((u+v) > 1.0))
    return -1;
  float t = f*dot(e2,q);
	if(t > 0.0)
		return t;
}


Intersection intersect( Ray ray)
{
  Intersection I;
  I.hit = false;
  I.t = 1e32;
  int id = -1;

  //Check for intersection with spheres
  for (int i = 0; i < NUM_SPHERES; ++i) {
    float d = intersect(ray, scene.spheres[i]);
    if (d>0 && d<=  I.t) {
      I.t = d;
      id = i;
      I.hit = true;
      I.point = ray.origin+ray.dir*I.t;
      I.light_dir = normalize(scene.sun_position - I.point);
      I.view_dir = normalize(ray.origin - I.point);
      I.normal = normalize(I.point - scene.spheres[id].center);
      I.material.color_diffuse = scene.spheres[id].material.color_diffuse;
      I.material.color_glossy = scene.spheres[id].material.color_glossy;
      I.material.color_emission = scene.spheres[id].material.color_emission;
      I.material.reflection = scene.spheres[id].material.reflection;
      I.material.transmission = scene.spheres[id].material.transmission;
      I.material.ior = scene.spheres[id].material.ior;
      float incident_angle = max(dot(I.light_dir,I.normal),0.0);


      if(brdf_swapper == 0) //Lambert
        I.material.color_brdf = vec3(I.material.color_diffuse*1.0/M_PI * incident_angle);
      else if(brdf_swapper == 1) //blinn-phong
        I.material.color_brdf = blinn_phong_brdf(I.light_dir,I.view_dir,I.normal, scene.spheres[id].material) * incident_angle;
      else if(brdf_swapper == 2) //Cook-torrance
        I.material.color_brdf = cook_torrance_brdf(I.light_dir,I.view_dir,I.normal,scene.spheres[id].material) * incident_angle*10;
      }
    }


  //Check for intersection with planes
  {
    float d = intersect(ray,scene.ground_plane[0]);
    if (d>0 && d<=I.t){
      I.t=d;

      // YOUR CODE GOES HERE
      // Populate I with all the relevant data.
      I.point = ray.origin+ray.dir*I.t;
      I.light_dir = normalize(scene.sun_position - I.point);
      I.view_dir = normalize(ray.origin - I.point);
      I.normal = scene.ground_plane[0].normal;
      I.material.reflection = scene.ground_plane[0].material.reflection;
      I.material.transmission = scene.ground_plane[0].material.transmission;
      I.material.ior = scene.ground_plane[0].material.ior;
      float incident_angle = max(dot(I.light_dir,I.normal),0.0);
      I.hit = true;

		// Adding a procedural checkerboard texture:
			I.material.color_diffuse = (mod(floor(I.point.x) + floor(I.point.z),2.0) == 0.0) ?
			scene.ground_plane[0].material.color_diffuse :
			vec3(1.0) - scene.ground_plane[0].material.color_diffuse;
			scene.ground_plane[0].material.color_diffuse = I.material.color_diffuse;
			//Press Y to swap brdf
			if(brdf_swapper == 0) //Lambert
				I.material.color_brdf = vec3(I.material.color_diffuse*1.0/M_PI * incident_angle);
			else if(brdf_swapper == 1) //blinn-phong
				I.material.color_brdf = blinn_phong_brdf(I.light_dir,I.view_dir,I.normal, scene.ground_plane[0].material) * incident_angle;
			else if(brdf_swapper == 2) //Cook-torrance
				I.material.color_brdf = cook_torrance_brdf(I.light_dir,I.view_dir,I.normal, scene.ground_plane[0].material) * incident_angle;
    }
  }

	    //Check for intersection with triangles
   {
   for (int i = 0; i < NUM_TRIANGLES; ++i) {
      float d = intersect(ray, scene.triangles[i]);
      if (d>0 && d<=I.t) {
        I.t = d;
        id = i;
        I.hit = true;
        I.point = ray.origin+ray.dir*I.t;
        I.light_dir = normalize(scene.sun_position - I.point);
        I.view_dir = normalize(ray.origin - I.point);
        I.normal = normalize(scene.triangles[id].normal);
        I.material.color_diffuse = scene.triangles[0].material.color_diffuse;
        I.material.color_glossy = scene.triangles[id].material.color_glossy;
        I.material.color_emission = scene.triangles[id].material.color_emission;
        I.material.reflection = scene.triangles[id].material.reflection;
        I.material.transmission = scene.triangles[id].material.transmission;
        I.material.ior = scene.triangles[id].material.ior;
        float incident_angle = max(dot(I.light_dir,I.normal),0.0);


        //Press Y to swap brdf
        if(brdf_swapper == 0) //Lambert
          I.material.color_brdf = vec3(I.material.color_diffuse*1.0/M_PI * incident_angle);
        else if(brdf_swapper == 1) //blinn-phong
          I.material.color_brdf = blinn_phong_brdf(I.light_dir,I.view_dir,I.normal, scene.triangles[i].material) * incident_angle;
        else if(brdf_swapper == 2) //Cook-torrance
          I.material.color_brdf = cook_torrance_brdf(I.light_dir,I.view_dir,I.normal, scene.triangles[i].material) * incident_angle;
      }

    }
  }

  //If no sphere or plane hit, we hit the sky instead
  if (I.t>1e20){
    I.point = ray.dir*I.t;
    I.normal = -ray.dir;
    vec3 sky = simple_sky(ray.dir); // pick color from sky function

    // Sky is all emission, no diffuse or glossy shading:
    I.material.color_diffuse = 0 * sky;
    I.material.color_glossy = 0.0 * vec3( 1 );
    I.material.roughness = 1;
    I.material.color_emission = 0.3 * sky;
    I.material.reflection = 0.0;
    I.material.transmission = 0;
    I.material.ior = 1;
    I.material.color_brdf = vec3(0);
    I.hit = false;
  }

  return I;
}
// Casts 1 depth rays for reflection/refraction
// For more depth it should have recursion
vec3 raycast()
{
  vec3 color = vec3(0.5);
  for(int ray_stack_pos=0; ray_stack_pos < ray_stack_size; ++ray_stack_pos){
    if(ray_stack_size > 3)
      break;
    Ray ray = ray_stack[ray_stack_pos];
    Intersection isec = intersect(ray);

    if(isec.hit){
      Ray shadow_feeler;
      shadow_feeler.origin = ray.origin + ray.dir*isec.t + 0.0001;
      shadow_feeler.dir = normalize(scene.sun_position - shadow_feeler.origin);
      Intersection shadow_isec = intersect(shadow_feeler);
      if(shadow_isec.hit){
        color *= 0.1;
      }
    }
    if(isec.material.reflection > 0.0) {
      Ray refl;
      refl.dir = normalize(reflect(ray.dir,isec.normal));
      refl.origin = dot(refl.dir,isec.normal) < 0.0 ? isec.point - isec.normal*1e-3 : isec.point + isec.normal*1e-3;
      //Intersection ref_isec = intersect(refl);
      float tmp = isec.material.ior;
      refl.weight = tmp - tmp/2.0;
      push(refl);
    }

    if(isec.material.transmission > 0.0){
      Ray refr;
      //If inside the object, invert normal and refraction indices
      refr.dir = dot(ray.dir,isec.normal) < 0.0 ? normalize(refract(ray.dir,isec.normal,1.0/isec.material.ior)) : normalize(refract(ray.dir,-isec.normal, isec.material.ior/1.0));
      refr.origin = dot(ray.dir,isec.normal) < 0.0 ? isec.point - isec.normal*1e-3 : isec.point + isec.normal*1e-3;
      float tmp = isec.material.ior;
      refr.weight = tmp - tmp/2.0;
      push(refr);
    }
    //ray.weight=1;
    color *= ray.weight*(isec.material.color_brdf + isec.material.color_emission);

  }
  return 5*color;
}

void main() {
	vec2 uv =  gl_FragCoord.xy - 0.5*i_window_size.xy;

	//init(i_light_position, 10.0 ); //i_mouse_state.y);
	init( 10.0 );

	int num_samples=1;


	//crude zooming by pressing right mouse button
	float f_dist = i_focal_dist + i_focal_dist*i_mouse_state.w;

	//basis for defining the image plane
	vec3 cx = i_right;
	vec3 cy = i_up;
	vec3 cz = i_dir;

	Ray ray;
	ray.origin = i_position;
  ray.dir = normalize(cx*gl_FragCoord.x + cy*gl_FragCoord.y + cz*f_dist);
  ray.weight = 1;
  push(ray);

  vec3 color = raycast();
	//linear blend, will look terrible
	// o_fragment_color =  vec4((color),1);

	//gamma correction
	o_fragment_color = vec4( pow ( clamp(color.xyz/num_samples, 0., 1.), vec3(1./2.2)), 1.);
	///\todo REMOVE THIS LINE after you have the triangles set up
}
