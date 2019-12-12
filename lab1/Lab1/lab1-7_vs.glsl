#version 400
layout(location=0) in vec4 vp;
out float zcolor;
uniform mat4 transform;

void main () {
	
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Apply the model, view and projection transform to vertex positions and forward the position to the fragment shader using an appropriate "out" variable
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  
  gl_Position = transform*vp;
	zcolor = gl_Position.z;
};
  