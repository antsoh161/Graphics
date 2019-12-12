#version 400
in vec3 vp;
out vec3 pos2color;

void main () {
  gl_Position = vec4 (vp,1);
  pos2color = vp;
 };
  