#version 400
out vec4 frag_colour;
uniform float modifier;

void main () {

  //should be based on uniform variable input
  
  frag_colour = vec4(vec3(0.5,0.2,0.7)*modifier, 1.0);
}
