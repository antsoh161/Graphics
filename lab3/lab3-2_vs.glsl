#version 400
layout(location=0) in vec4 vp;
layout(location=1) in vec4 normals;
layout(location=2) in vec2 uv_coords;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 world_pos;
out vec3 Normal;
out vec2 UV_coords;

void main () {
  world_pos = (model*vp).xyz;
	Normal = (model*normals).xyz;
	UV_coords = uv_coords;
	gl_Position = projection*view*model*vp;
  
  
};
  