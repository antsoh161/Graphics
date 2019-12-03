#version 400
layout(location=0) in vec4 vp;
layout(location=1) in vec4 normals;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 Normal;
out vec3 position;
out vec3 view_pos;

 
 void main() {
  vec4 v_view_pos = view*model*vec4(vec3(vp.xyz),1);
  view_pos = vec3(v_view_pos.xyz);
  
  vec4 v_pos = model*vp;
  position = vec3(v_pos.xyz);
   
  vec4 v_Normal = model*vec4(vec3(normals.xyz),0);
  Normal = v_Normal.xyz;
  gl_Position = projection*view*model*vp;
 }