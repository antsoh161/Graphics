#version 400
in vec3 vp;
out float zcolor;

void main () {
  zcolor = vp.z;
  gl_Position = vec4 (vec3(vp), 1.0);
};
  