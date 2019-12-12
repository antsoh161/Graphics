#ifndef MY_SHADER_H
#define MY_SHADER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h> 

#include <cstlib>
#include <iostream>
#include <readfile.hpp>

//Simple shader class holding fragment and vertice shader
//read from .glsl-files using readfile.hpp

class my_shader{
public:
		GLuint shader_program;

        my_shader();
		~my_shader();
        void update();

};

//Constructor reads from file, compiles and use the shaders
void my_shader::my_shader(){
	shader_program = glCreateProgram();
	
}

void my_shader::update(){
	std::string vertex_shader_str = readFile("../lab1-3_vs.glsl");
	std::string fragment_shader_str = readFile("../lab1-3_fs.glsl");
	const char *vertex_shader_src = vertex_shader_str.c_str();
	const char *fragment_shader_src = fragment_shader_str.c_str();
	GLuintvs = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vs,1,&vertex_shader_src,NULL);
	glCompileShader (vs);
	GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fs, 1, &fragment_shader_src, NULL);
	glCompileShader (fs);
	glAttachShader (shader_program, fs);
	glAttachShader (shader_program, vs);
	glLinkProgram (shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);
}

