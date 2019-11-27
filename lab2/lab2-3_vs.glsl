#version 400
layout(location=0) in vec4 vp;
layout(location=1) in vec4 normals;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec4 Normal;
out vec4 position;

/*
void main () {
	
  Normal = model*normals;
  position = model*vp;
  viewPosition = model*vp;
  gl_Position = projection*view*model*vp;
  
};
 */
 
 void main() {
 	gl_Position = projection*view*model*vp;
  position = view*model*vp;
	Normal = model*normals;
  
 }