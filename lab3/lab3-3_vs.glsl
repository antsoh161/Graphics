#version 400
layout(location=0) in vec4 vp;
layout(location=1) in vec4 normals;
layout(location=2) in vec2 uv_coords;
layout(location=3) in vec4 tangent;
layout(location=4) in vec4 bitangent;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 world_pos;
out vec3 Normal;
out vec2 UV_coords;
out vec3 tang_pos;
out vec3 bitang_pos;
out mat3 TBN;

void main () {
  world_pos = (model*vp).xyz;
	Normal = (model*normals).xyz;
	UV_coords = uv_coords;
    
  //mat4 TBN = mat4(tangent,bitangent,normals,vec4(0,0,0,1));
  mat3 TBN = transpose(mat3(normalize(model*tangent),normalize(model*bitangent),normalize(model*normals)));
  
  
	gl_Position = projection*view*model*vp;
  
  
};
  