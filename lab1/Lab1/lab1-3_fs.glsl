#version 400
out vec4 frag_colour;
in vec3 pos2color;


void main () {
  frag_colour = vec4 (pos2color+0.5, 1.0);
}
