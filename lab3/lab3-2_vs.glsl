#version 400
layout(location=0) in vec4 vp;
layout(location=1) in vec4 normals;
layout(location=2) in vec2 uv_coords;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 Normal;
out vec2 UV_coords;
out vec3 position;
void main () {
	
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Apply the model, view and projection transform to vertex positions and forward the position to the fragment shader using an appropriate "out" variable
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  vec4 v_Normal = view*model*normals;
  Normal = v_Normal.xyz;
  vec4 v_position = view*model*vec4(vec3(vp.xyz),1);
  position = vec3(v_position.xyz);
  UV_coords = uv_coords;
  gl_Position = projection*view*model*vp;
};
  