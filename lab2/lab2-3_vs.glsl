#version 400
layout(location=0) in vec4 vp;
layout(location=1) in vec4 normals;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec4 Normal;
out vec4 position;
out vec4 viewPosition;

/*
void main () {
	
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Apply the model, view and projection transform to vertex positions and forward the position to the fragment shader using an appropriate "out" variable
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  Normal = model*normals;
  position = model*vp;
  viewPosition = view*model*vp;
  gl_Position = projection*view*model*vp;
};
 */
 
 void main() {
 	gl_Position = projection*view*model*vp;
	position = view*vp;
  position = position;
	Normal = model*normals;
 }