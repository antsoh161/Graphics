#version 400
layout(location=0) in vec4 vp;
layout(location=1) in vec4 normals;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 Normal;
out vec3 position;
out vec3 viewPos;

 
 void main() {

  vec4 v_position = view*model*vec4(vec3(vp.xyz),1);
  position = vec3(v_position.xyz);
  
  vec4 v_Normal = view*model*vec4(vec3(normals.xyz),0);
  Normal = v_Normal.xyz;
  gl_Position = projection*view*model*vp;
 }