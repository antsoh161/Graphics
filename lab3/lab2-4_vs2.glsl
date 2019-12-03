#version 400
layout(location=0) in vec4 vp;
layout(location=1) in vec4 normals;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 Normal;
out vec3 world_pos;
out vec3 view_pos;

void main(){
	world_pos = (model * vp).xyz;
	
  vec4 v_pos = model*vp;
  	world_pos = vec3(v_pos.xyz);
	Normal = (model*normals).xyz;
	view_pos = world_pos;
	
	gl_Position = projection*view*model*vp;
}

