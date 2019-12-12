#version 400
in vec3 vp;
uniform vec2 position_offset;

void main () {
	//should be based on input from keyboard
		
    gl_Position = vec4(vp+vec3(position_offset,0),1.0);

};
  